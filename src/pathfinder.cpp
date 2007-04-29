/***************************************************************************
						pathfinder.cpp  -  description
							-------------------
	begin                : may 17th, 2004
	copyright            : (C) 2004-2007 by Duong-Khang NGUYEN
	email                : neoneurone @ users sourceforge net

	$Id$
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

// Useful enumerations
#include "opencity_direction.h"
#include "opencity_structure_type.h"

// OpenCity headers
#include "buildinglayer.h"
#include "destination.h"
#include "pathstructure.h"
#include "map.h"
#include "pathfinder.h"

// Standard headers
#include <algorithm>

// Local defines
#define OC_PATHFINDER_MAX_LENGTH	0xFFFF0000


//========================================================================
/** This is an utility structure used during the pathfinding process
*/
struct PathFinderNode {
	int iOwnLinear;				///< The OC linear coordinates of this node.
	int iFatherLinear;			///< The OC linear coordinates of the precedent node, = -1 for none
	uint uiEvaluation;			///< An evaluation to destination
	PathStructure* ppath;		///< The PathStructure of this node
};


//========================================================================
/** This is a functor, it's used for sorting node
*/
static bool
pathfinderCompareTraffic( const PathFinderNode & rcA, const PathFinderNode & rcB )
{
	assert( rcA.ppath != NULL );
	assert( rcB.ppath != NULL );

	return rcA.ppath->GetLength() > rcB.ppath->GetLength();
/*
	uint uiA = rcA.ppath->GetLength();
	uint uiB = rcB.ppath->GetLength();

// Same length
	if ( uiA == uiB ) {
		if ( rcA.uiEvaluation < rcB.uiEvaluation) {
			return false;
		}
		else {
			return true;
		}
	} else
	if ( uiA < uiB) {
		return false;
	}
	else {
// Greater length, then put it at the beginning
		return true;
	}
*/

// Impossible here
	assert( 0 );
}


   //========================================================================
   /** This is a functor, it's used for sorting node
   */
static bool
pathfinderCompareDistance( const PathFinderNode& rcA, const PathFinderNode& rcB )
{
	return rcA.uiEvaluation > rcB.uiEvaluation;
/*
	assert( rcA.ppath != NULL );
	assert( rcB.ppath != NULL );

	uint uiA = rcA.ppath->GetLength();
	uint uiB = rcB.ppath->GetLength();

// Same length
	if ( rcA.uiEvaluation == rcB.uiEvaluation ) {
		if ( uiA < uiB ) {
			return false;
		}
		else {
			return true;
		}
	} else
	if ( rcA.uiEvaluation < rcB.uiEvaluation ) {
		return false;
	}
	else {
// Greater length, then put it at the beginning
		return true;
	}
*/

// Impossible here
	assert( 0 );
}


   //========================================================================
   /** This is an evaluating function. It calculates the euclidian
       distance square between two coordinates
   */
inline const uint
pathfinderEvaluate(
	const uint & rcuiW1,
	const uint & rcuiH1,
	const uint & rcuiW2,
	const uint & rcuiH2)
{
	return ((rcuiW2-rcuiW1)*(rcuiW2-rcuiW1) + (rcuiH2-rcuiH1)*(rcuiH2-rcuiH1));
}


   /*=====================================================================*/
PathFinder::PathFinder(
	SDL_mutex* const mutex,
	BuildingLayer* const pblayer,
	Map* const map,
	const uint & rcuiCityWidth,
	const uint & rcuiCityHeight ):
pmutex( mutex ),
pbuildlayer( pblayer ),
pmap( map ),
uiWidth( rcuiCityWidth ),
uiHeight( rcuiCityHeight )
{
	OPENCITY_DEBUG( "ctor" );

// some assert for programming errors checking
	assert( mutex != NULL );
	assert( pblayer != NULL );
	assert( map != NULL );
}


   /*=====================================================================*/
PathFinder::~PathFinder()
{
	OPENCITY_DEBUG( "dtor" );
}

   //========================================================================
	/** Find the shortest path given the OpenCity W1, H1
		and W2, H2 coordinates. The algorithm is adapted from the famous
		DIJKSTRA algorithm.
	\n
	<b>Algorithm</b>\n
	<pre>
		1) Initialize all the OC_ROAD structures
		2) Insert the starting point to the "processing" list
		3) WHILE the processing list is not empty and not boolFound DO
			3a)Sort the processing list
			3b)Pop the node with the minimum length (and not marked)
			3c)IF the node is the arrival point THEN
				boolFound = TRUE
				FI
			3d)FOR each neighbour OC_ROAD DO
				newLength = the length of the current node
				+ the traffic of this neighbour;
				IF newLength is less than the length of the neighbour THEN
					i)the length of the neighbour is = newLength
					ii)father of the neighbour is = this node
					iii)insert the neighbour to the processing list
				FI
				DONE
		4) Mark the current node as processed
	</pre>

	\param rcuiW1, rcuiH1, rcuiW2, rcuiH2 = must be the valid
			OC W,H coordinates and there must be PathStructure at
			the designated W1,H1 and W2, H2 coordinates
	\param rvdest = the vector which contains the path requested if it exists
	\param enumType = do we look for shortest path in term of distance or traffic ?
	\param uiMaxLength = limit the number of loops in the algorithm to uiMaxLength.
It means that the maximum length of the returned path is uiMaxLength units long.
And the most important thing is that the returned path may _not_ be the optimal path.
	\return false IF error, true otherwise
   */
const bool
PathFinder::findShortestPath(
	const uint & rcuiW1,
	const uint & rcuiH1,
	const uint & rcuiW2,
	const uint & rcuiH2,
	std::vector<Destination> & rvdest,
	const PATH_TYPE & enumType,
	uint uiMaxLength )
{
	uint uiLinear;
	uint uiW, uiH;
	uint uiWN, uiHN;		// N as Neighbour
	uint uiLength;
	uint uiMaxLinear = this->pbuildlayer->GetMaxLinear();
	Structure* pstruct;
	PathStructure * ppathstruct;

	PathFinderNode node;
	PathFinderNode nodeDone;
	std::vector<PathFinderNode> vProcessing;
	std::vector<PathFinderNode> vDone;
	bool boolFound;

// Dynamic compare functor
	bool (*pFunctor)(const PathFinderNode&, const PathFinderNode&);

// Variables used to rebuild the "destination vector" from start to stop
	int iFatherLinear;
	Destination destLast, destTemp;

// WARNING the Start point is a PathStructure
// do we need dynamic casting for automatic checking ?
	ppathstruct = dynamic_cast<PathStructure*>(pbuildlayer->GetStructure( rcuiW1, rcuiH1 ));
	if ( ppathstruct == NULL ) {
		OPENCITY_DEBUG("WARNING: Starting point NULL");
		return false;
	}

// Block all the other threads while we play with the structures
	SDL_LockMutex( this->pmutex );

// Initialize the functor
	switch (enumType) {
		case OC_DISTANCE:
			pFunctor = pathfinderCompareDistance;
			break;
		case OC_TRAFFIC:
			pFunctor = pathfinderCompareTraffic;
			break;
	}

// For each OC_ROAD structure
	for ( uiLinear = 0; uiLinear <= uiMaxLinear; uiLinear++ ) {
		pstruct = pbuildlayer->GetLinearStructure( uiLinear );
		if ((pstruct != NULL) and (pstruct->GetCode() == OC_STRUCTURE_ROAD )) {
		// Clear the "mark"
			pstruct->Unset( OC_STRUCTURE_MARK );
		// Set the maximum "length"
			((PathStructure*)pstruct)->SetLength(OC_PATHFINDER_MAX_LENGTH );
		}
	}

// The length value of the starting point is 0 (minimum)
	ppathstruct->SetLength( 0 );

// Insert the starting point to the processing vector
	node.iOwnLinear = this->uiWidth * rcuiH1 + rcuiW1;
	node.iFatherLinear = -1;		// there's no precedent node
	node.uiEvaluation = pathfinderEvaluate(rcuiW1, rcuiH1, rcuiW2, rcuiH2);
	node.ppath = ppathstruct;
	vProcessing.push_back( node );

// Loop until we finish or we found the arrival point
	boolFound = false;

	while (!boolFound and (!vProcessing.empty()) and (vDone.size() < uiMaxLength)) {
	// Sort the list so that the node with minimum "length" value
	// and not "marked" is placed at the bottom (back)
		std::sort( vProcessing.begin(), vProcessing.end(), pFunctor );

	// get the sorted node with minimum length, and not marked
	// NOTE: everything contained in the vProcessing is not marked !
		node = vProcessing.back();
		vProcessing.pop_back();

	// Conversion Linear <-> WH
		uiH = node.iOwnLinear / uiWidth;
		uiW = node.iOwnLinear % uiWidth;

	// IF the arrival point has been found THEN
		if ((uiW == rcuiW2) && (uiH == rcuiH2)) {
			boolFound = true;
			vDone.push_back( node );
			break;
		}

	   //---------------------- WARNING: repeated procedure begin ---------------------
	// Get the neighbour in the NORTH
		if ((this->pmap->GetNeighbourWH(uiW, uiH, uiWN, uiHN, OC_DIR_O_N ) == true)
		  &&((pstruct = pbuildlayer->GetStructure( uiWN, uiHN )) != NULL)
		  && (pstruct->GetCode() == OC_STRUCTURE_ROAD)
		  && (pstruct->IsSet(OC_STRUCTURE_MARK) == false)) {
//debug cout << "NORTH " << endl;
			ppathstruct = (PathStructure*)pstruct;
			uiLength = node.ppath->GetLength() + (uint)ppathstruct->GetTraffic();
		// Is the new path shorter ?
			if (ppathstruct->GetLength() > uiLength) {
				ppathstruct->SetLength( uiLength );
			// Now push the neighbour to the processing list
				nodeDone.iOwnLinear = uiHN * uiWidth + uiWN;
				nodeDone.iFatherLinear = node.iOwnLinear;
				nodeDone.uiEvaluation = pathfinderEvaluate(uiWN, uiHN, rcuiW2, rcuiH2);
				nodeDone.ppath = ppathstruct;
				vProcessing.push_back( nodeDone );
			}
		}

	// Get the neighbour in the EAST
		if ((this->pmap->GetNeighbourWH(uiW, uiH, uiWN, uiHN, OC_DIR_O_E ) == true)
		  &&((pstruct = pbuildlayer->GetStructure( uiWN, uiHN )) != NULL)
		  && (pstruct->GetCode() == OC_STRUCTURE_ROAD)
		  && (pstruct->IsSet(OC_STRUCTURE_MARK) == false)) {
//debug cout << "EAST " << endl;
			ppathstruct = (PathStructure*)pstruct;
			uiLength = node.ppath->GetLength() + (uint)ppathstruct->GetTraffic();
		// Is the new path shorter ?
			if (ppathstruct->GetLength() > uiLength) {
				ppathstruct->SetLength( uiLength );
			   // now push the neighbour to the processing list
				nodeDone.iOwnLinear = uiHN * uiWidth + uiWN;
				nodeDone.iFatherLinear = node.iOwnLinear;
				nodeDone.uiEvaluation = pathfinderEvaluate(uiWN, uiHN, rcuiW2, rcuiH2);
				nodeDone.ppath = ppathstruct;
				vProcessing.push_back( nodeDone );
			}
		}

	// Get the neighbour in the SOUTH
		if ((this->pmap->GetNeighbourWH(uiW, uiH, uiWN, uiHN, OC_DIR_O_S ) == true)
		  &&((pstruct = pbuildlayer->GetStructure( uiWN, uiHN )) != NULL)
		  && (pstruct->GetCode() == OC_STRUCTURE_ROAD)
		  && (pstruct->IsSet(OC_STRUCTURE_MARK) == false)) {
//debug cout << "SOUTH " << endl;
			ppathstruct = (PathStructure*)pstruct;
			uiLength = node.ppath->GetLength() + (uint)ppathstruct->GetTraffic();
		// Is the new path shorter ?
			if (ppathstruct->GetLength() > uiLength) {
				ppathstruct->SetLength( uiLength );
			   // now push the neighbour to the processing list
				nodeDone.iOwnLinear = uiHN * uiWidth + uiWN;
				nodeDone.iFatherLinear = node.iOwnLinear;
				nodeDone.uiEvaluation = pathfinderEvaluate(uiWN, uiHN, rcuiW2, rcuiH2);
				nodeDone.ppath = ppathstruct;
				vProcessing.push_back( nodeDone );
			}
		}

	// Get the neighbour in the WEST
		if ((this->pmap->GetNeighbourWH(uiW, uiH, uiWN, uiHN, OC_DIR_O_W ) == true)
		  &&((pstruct = pbuildlayer->GetStructure( uiWN, uiHN )) != NULL)
		  && (pstruct->GetCode() == OC_STRUCTURE_ROAD)
		  && (pstruct->IsSet(OC_STRUCTURE_MARK) == false)) {
//debug cout << "WEST " << endl;
			ppathstruct = (PathStructure*)pstruct;
			uiLength = node.ppath->GetLength() + (uint)ppathstruct->GetTraffic();
		// Is the new path shorter ?
			if (ppathstruct->GetLength() > uiLength) {
				ppathstruct->SetLength( uiLength );
			   // now push the neighbour to the processing list
				nodeDone.iOwnLinear = uiHN * uiWidth + uiWN;
				nodeDone.iFatherLinear = node.iOwnLinear;
				nodeDone.uiEvaluation = pathfinderEvaluate(uiWN, uiHN, rcuiW2, rcuiH2);
				nodeDone.ppath = ppathstruct;
				vProcessing.push_back( nodeDone );
			}
		}
	   //---------------------- repeated procedure end ---------------------

	// Mark this node as "done" then insert it into the "done" list
		node.ppath->Set( OC_STRUCTURE_MARK );
		vDone.push_back( node );
	}


// Build the destination vector from start to stop
	if (boolFound or (vDone.size() >= uiMaxLength)) {
	// Put the last node to the list
		int i = vDone.size()-1;
		node = vDone[ i ];
		iFatherLinear = node.iFatherLinear;
		destLast._uiW = node.iOwnLinear % uiWidth;
		destLast._uiL = node.iOwnLinear / uiWidth;
		destLast._iHMin = pmap->GetSquareMinHeight( destLast._uiW, destLast._uiL );
		destLast._iHMax = pmap->GetSquareMaxHeight( destLast._uiW, destLast._uiL );
		destLast._uiTime = 1;
		destLast._ubTraffic =
			((PathStructure*)pbuildlayer->
			GetLinearStructure( node.iOwnLinear ))->GetTraffic();

		rvdest.push_back( destLast );

		while (--i >= 0) {
			node = vDone[i];
		// IF this is NOT the father of the precedent node
			if (node.iOwnLinear != iFatherLinear)
				continue;

			iFatherLinear = node.iFatherLinear;
			destTemp._uiW = node.iOwnLinear % uiWidth;
			destTemp._uiL = node.iOwnLinear / uiWidth;
			destTemp._iHMin = pmap->GetSquareMinHeight( destTemp._uiW, destTemp._uiL );
			destTemp._iHMax = pmap->GetSquareMaxHeight( destTemp._uiW, destTemp._uiL );
			destTemp._uiTime = 1;
			destTemp._ubTraffic =
				((PathStructure*)pbuildlayer->
				GetLinearStructure( node.iOwnLinear ))->GetTraffic();

		// NOTE: we're going from the back to the front
			destTemp._eDir = Destination::GetDir( destTemp, destLast );
			rvdest.push_back( destTemp );
			destLast = destTemp;
		}

	// Reverse the destination vector so that the starting point is at the beginning
		reverse( rvdest.begin(), rvdest.end() );
	} // if boolFound

#ifndef PATHFINDER_NDEBUG
//debug, print out the found path
	if ((boolFound == true) or (vDone.size() >= uiMaxLength)) {
		PATHFINDER_DEBUG( "Shortest path found, number of nodes: " << rvdest.size() );
	   // initialize the "iFatherLinear" variable
	   // as the stop point's linear index
		iFatherLinear = vDone[ vDone.size()-1 ].iOwnLinear;
		for (int i = vDone.size()-1; i > -1; i--) {
			node = vDone[i];
		   // is this node is the father of the precedent node ?
			if (node.iOwnLinear == iFatherLinear) {
			   // convert Linear <-> WH
				uiH = node.iOwnLinear / uiWidth;
				uiW = node.iOwnLinear % uiWidth;
				cout << "W / H: " << uiW << " / " << uiH;
				cout << " Length: " << node.ppath->GetLength();
				cout << " Traffic: " << (int)node.ppath->GetTraffic();
				cout << endl;
				iFatherLinear = node.iFatherLinear;
			}
		}
//debug: print out all the processed WH
		PATHFINDER_DEBUG( "All processed nodes, number of nodes : " << vDone.size() );
		for (unsigned int i = 0; i < vDone.size(); i++) {
			node = vDone[i];
		   // convert Linear <-> WH
			uiH = node.iOwnLinear / uiWidth;
			uiW = node.iOwnLinear % uiWidth;
			cout << "W / H: " << uiW << " / " << uiH;
			cout << " Length: " << node.ppath->GetLength();
			cout << " Traffic: " << (int)node.ppath->GetTraffic();
			cout << " Evaluation: " << node.uiEvaluation;
			cout << endl;
		}
	}
#endif

// Let the other threads run now
	SDL_UnlockMutex( this->pmutex );

	return boolFound;
}


































