/***************************************************************************
						electricitysim.cpp  -  description
							-------------------
	begin                : march 2nd, 2004
	copyright            : (C) 2004-2010 by Duong Khang NGUYEN
	email                : neoneurone @ gmail com

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
#include "electricitysim.h"
#include "buildinglayer.h"
#include "structure.h"

// Global settings
#include "globalvar.h"
extern GlobalVar gVars;

// Standard headers
#include <algorithm>

using namespace std;


   /*=====================================================================*/
ElectricitySim::ElectricitySim(
	SDL_mutex* mutex,
	BuildingLayer* pblayer,
	Map* pmap ):
Simulator( mutex, pblayer, pmap ),
_uiNumberEPlant( 0 ),
_iValueMax( 0 )
{
	OPENCITY_DEBUG( "ESim param ctor" );
}


   /*=====================================================================*/
ElectricitySim::~ElectricitySim()
{
	OPENCITY_DEBUG( "ESim dtor" );
}


   /*=====================================================================*/
void
ElectricitySim::SaveTo( std::fstream& rfs )
{
// Call the base class method
	Simulator::SaveTo( rfs );
}


   /*=====================================================================*/
void
ElectricitySim::LoadFrom( std::fstream& rfs )
{
// Call the base class method
	Simulator::LoadFrom( rfs );

// Member variable reinitialization
	_uiNumberEPlant = 0;
	_iValueMax = 0;
	_vectorpairuiEPlant.clear();
}


   //========================================================================
   // int simulatorMain()
   //
   // description: simulate the transmission of the electricity
   //
   // algorithm  : it is inpired by my GRAPH knownledge
   //              (which is quite limited). In few words, it is
   //              breadth-first search algorithm
   //
   // note       : (28 jun 04) Currently, RCISims unset the Electricity bit
   //              then set it again if there's an electric structure around
   //              the under processing structure. I don't think that is a
   //              good idea. We should let the electricity sim do its job.
   //========================================================================
int
ElectricitySim::Main()
{
	static Structure* pstruct;
	static vector< pair<uint, uint> >::iterator iter;
	static pair<uint, uint> pairstructWH;
	static uint nW, nH;

// IF the simulators are not running THEN return
	if (_eSimState != SIMULATOR_RUNNING)
		return 0;

	SDL_LockMutex( _pMutexMain );

// clear the mark and E bit of ALL structure
	_pBuildLayer->StructureUnset( (OC_BYTE)( OC_STRUCTURE_MARK | OC_STRUCTURE_E ) );

// for each EPLANT do
	iter = _vectorpairuiEPlant.begin();
	while ( iter != _vectorpairuiEPlant.end() ) {
		pairstructWH = *iter;
		pstruct = _pBuildLayer->GetStructure( pairstructWH.first, pairstructWH.second );

	// only process the EPLANT if it is not marked yet
		if (pstruct != NULL)
		if (pstruct->IsSet( OC_STRUCTURE_MARK ) == false )
			_dequepairui.push_back( pairstructWH );

	// now process the deque
		while ( !_dequepairui.empty() ) {
			pairstructWH = _dequepairui.front();
			_dequepairui.pop_front();
		// process the front structure
			pstruct = _pBuildLayer->GetStructure( pairstructWH.first, pairstructWH.second );
		// WARNING: pstruct is never NULL here !
			switch (pstruct->GetCode()) {
		// turn the E bit of the following structures on
			case OC_STRUCTURE_EPLANT_COAL:
			case OC_STRUCTURE_EPLANT_NUCLEAR:
				pstruct->Set(OC_STRUCTURE_E);
				break;

		// the RCI zones need nearby electric zone
		// in order to be connected to the E system
			case OC_STRUCTURE_RES:
			case OC_STRUCTURE_COM:
			case OC_STRUCTURE_IND:
			case OC_STRUCTURE_PART:
			case OC_STRUCTURE_FIREDEPT:
			case OC_STRUCTURE_POLICEDEPT:
			case OC_STRUCTURE_EDUCATIONDEPT:
			case OC_STRUCTURE_HOSPITALDEPT:
				if (CheckRange(
					pairstructWH.first, pairstructWH.second,
					OC_E_RCI_RANGE, OC_STRUCTURE_ELECTRIC ) == true) {
					pstruct->Set(OC_STRUCTURE_E);
				}
				break;

		// the ELINE structure have a shorter range
			case OC_STRUCTURE_ELINE:
				if (CheckRange(
					pairstructWH.first, pairstructWH.second,
					OC_E_E_RANGE, OC_STRUCTURE_ELECTRIC ) == true) {
					pstruct->Set(OC_STRUCTURE_E);
				}
				break;

		// Ignored structures
			case OC_STRUCTURE_PARK:
			case OC_STRUCTURE_FLORA:
			case OC_STRUCTURE_ROAD:
			case OC_STRUCTURE_TEST:			// Development test feature
				break;

		// What's the heck ?
			case OC_STRUCTURE_UNDEFINED:
			case OC_STRUCTURE_ANY:
			case OC_STRUCTURE_ELECTRIC:

		// Future asserts
			case OC_STRUCTURE_MILITARYDEPT:

		// What's the heck ?
			default:
				OPENCITY_DEBUG( "What is this structure code: " << pstruct->GetCode() );
				assert( 0 );
				break;
			} // switch

		// Mark the structure we have just processed
			pstruct->Set( OC_STRUCTURE_MARK );

		// Put all the neighbour into the deque if they are not yet marked (processed)
		// look for the neighour in the NORTH
			if ((_pMapCity->GetNeighbourWH(
				pairstructWH.first, pairstructWH.second,
				nW, nH, OC_DIR_O_N ) == true )
				and (pstruct = _pBuildLayer->GetStructure( nW, nH ))
				and (pstruct->IsSet(OC_STRUCTURE_MARK) == false )
				and (this->DequeContain(make_pair(nW, nH)) == false))
				_dequepairui.push_back( pair<uint,uint>( nW, nH) );

		// look for the neighour in the EAST
			if ((_pMapCity->GetNeighbourWH(
				pairstructWH.first, pairstructWH.second,
				nW, nH, OC_DIR_O_E ) == true )
				and (pstruct = _pBuildLayer->GetStructure( nW, nH ))
				and (pstruct->IsSet(OC_STRUCTURE_MARK) == false )
				and (this->DequeContain(make_pair(nW, nH)) == false))
				_dequepairui.push_back( pair<uint,uint>( nW, nH) );

		// look for the neighour in the SOUTH
			if ((_pMapCity->GetNeighbourWH(
				pairstructWH.first, pairstructWH.second,
				nW, nH, OC_DIR_O_S ) == true )
				and (pstruct = _pBuildLayer->GetStructure( nW, nH ))
				and (pstruct->IsSet(OC_STRUCTURE_MARK) == false )
				and (this->DequeContain(make_pair(nW, nH)) == false))
				_dequepairui.push_back( pair<uint,uint>( nW, nH) );

		// look for the neighour in the WEST
			if ((_pMapCity->GetNeighbourWH(
				pairstructWH.first, pairstructWH.second,
				nW, nH, OC_DIR_O_W ) == true )
				and (pstruct = _pBuildLayer->GetStructure( nW, nH ))
				and (pstruct->IsSet(OC_STRUCTURE_MARK) == false )
				and (this->DequeContain(make_pair(nW, nH)) == false))
				_dequepairui.push_back( pair<uint,uint>( nW, nH) );
		} // while

		// next EPLANT
		iter++;
	} // while _vectorpairuiEPlant

	SDL_UnlockMutex( _pMutexMain );

	return 0;
}



   /*=====================================================================*/
void
ElectricitySim::AddStructure(
	const uint & w1,
	const uint & h1,
	const uint & w2,
	const uint & h2 )
{
	Structure* pstruct;

	pstruct = _pBuildLayer->GetStructure( w1, h1 );
	if ( pstruct == NULL )
		return;

	switch( pstruct->GetCode() ) {
		case OC_STRUCTURE_EPLANT_COAL:
			_iValueMax += OC_EPLANT_COAL_POWER;
			_uiNumberEPlant++;
			_vectorpairuiEPlant.push_back( pair<uint, uint>( w1, h1 ) );
			break;

		case OC_STRUCTURE_EPLANT_NUCLEAR:
			_iValueMax += OC_EPLANT_NUCLEAR_POWER;
			_uiNumberEPlant++;
			_vectorpairuiEPlant.push_back( pair<uint, uint>( w1, h1 ) );
			break;

		default: // keep gcc happy
			break;
	} // switch
}


   /*=====================================================================*/
void
ElectricitySim::RemoveStructure(
	const uint & w1,
	const uint & h1,
	const uint & w2,
	const uint & h2 )
{
	Structure* pstruct;
	OPENCITY_STRUCTURE_CODE enumStructCode;
	uint mainW1 = w1, mainH1 = h1, mainW2 = w1, mainH2 = h1;
	uint mainW = w1, mainH = h1;
	bool boolFound = false;
	uint sw, sl, sh;				// Structure's width, length and height

// Remove the w, h of the main structure
	vector< pair<uint, uint> >::iterator iter;

	pstruct = _pBuildLayer->GetStructure( mainW, mainH );
	if ( pstruct == NULL )
		return;

// Normal structure codes
	enumStructCode = pstruct->GetCode();
	switch ( enumStructCode ) {
		case OC_STRUCTURE_PART:
			// get the main structure pointer, and code
			pstruct = pstruct->GetMain();
			enumStructCode = pstruct->GetCode();

			// Calculate the structure's range
			gVars.gpPropertyMgr->GetWLH( pstruct->GetGraphicCode(), sw, 4, sl, 4, sh, 1 );
			sw--; sl--;
			// now look for the main w, h
			_pMapCity->GetPossibleWH( mainW1, mainH1, -sw, -sl );
			_pMapCity->GetPossibleWH( mainW2, mainH2,  sw,  sl );
			mainH = mainH1;
			while ( (boolFound == false) and (mainH <= mainH2) ) {
				mainW = mainW1;
				while ( (boolFound == false) and (mainW <= mainW2) ) {
				// IF the current structure at mainW, mainH
				// is the main structure THEN we found it
					if ( _pBuildLayer->GetStructure( mainW, mainH ) == pstruct )
						boolFound = true;
					else
						mainW++;
				}
				mainH++;
			}
			break;

		case OC_STRUCTURE_RES:
		case OC_STRUCTURE_COM:
		case OC_STRUCTURE_IND:
		case OC_STRUCTURE_FIREDEPT:
		case OC_STRUCTURE_POLICEDEPT:
		case OC_STRUCTURE_EDUCATIONDEPT:
			break;

		default: // keep gcc happy
			break;
	}

// Power plant structure codes
	switch ( enumStructCode ) {
		case OC_STRUCTURE_EPLANT_COAL:
			_iValueMax -= OC_EPLANT_COAL_POWER;
			_uiNumberEPlant--;

		// Search for the pair of mainW, mainH in the "_vectorpairuiEPlant"
		// and delete it if found
			iter = find(
				_vectorpairuiEPlant.begin(),
				_vectorpairuiEPlant.end(),
				make_pair( mainW, mainH )
			);
			if ( iter != _vectorpairuiEPlant.end() )
				_vectorpairuiEPlant.erase( iter );
			break;

		case OC_STRUCTURE_EPLANT_NUCLEAR:
			_iValueMax -= OC_EPLANT_NUCLEAR_POWER;
			_uiNumberEPlant--;

		// search for the pair of mainW, mainH in the "_vectorpairuiEPlant"
		// and delete it if found
			iter = find(
				_vectorpairuiEPlant.begin(),
				_vectorpairuiEPlant.end(),
				make_pair( mainW, mainH )
			);
			if ( iter != _vectorpairuiEPlant.end() )
				_vectorpairuiEPlant.erase( iter );
			break;

		default: // keep gcc happy
			break;
	}
}


   /*=====================================================================*/
const int
ElectricitySim::GetMaxValue() const
{
	return _iValueMax;
}


   /*=====================================================================*/
bool
ElectricitySim::DequeContain( const pair<uint, uint> & pairui )
{
	if (find( _dequepairui.begin(), _dequepairui.end(), pairui )
		!= _dequepairui.end())
		return true;
	else
		return false;
}
