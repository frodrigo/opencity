/***************************************************************************
						electricitysim.cpp  -  description
							-------------------
	begin                : mar 2nd, 2004
	copyright            : (C) 2004-2006 by Duong-Khang NGUYEN
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

#include "electricitysim.h"

#include "buildinglayer.h"
#include "structure.h"

#include "../map.h"						// DevCpp sucks

#include "propertymanager.h"


extern PropertyManager* gpPropertyMgr;	// Global property manager


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
	vectorpairuiEPlant.clear();
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


	if (this->enumSimState == SIMULATOR_RUNNING) {
		SDL_LockMutex( this->mutexMain );
		// See MainSim::RefreshSimValue(), 16 sep 2006
		//_iValue = _iValueMax;

		// clear the mark and E bit of ALL structure
		pbuildlayer->StructureUnset( OC_STRUCTURE_MARK | OC_STRUCTURE_E );

		// for each EPLANT do
		iter = vectorpairuiEPlant.begin();
		while ( iter != vectorpairuiEPlant.end() ) {
			pairstructWH = *iter;
			pstruct = pbuildlayer->GetStructure( pairstructWH.first, pairstructWH.second );

			// only process the EPLANT if it is not marked yet
			if (pstruct != NULL)
			if (pstruct->IsSet( OC_STRUCTURE_MARK ) == false )
				dequepairui.push_back( pairstructWH );

			// now process the deque
			while ( !dequepairui.empty() ) {
				pairstructWH = dequepairui.front();
				dequepairui.pop_front();
				// process the front structure
				pstruct = pbuildlayer->GetStructure(
					pairstructWH.first, pairstructWH.second );
				// WARNING: pstruct is never NULL here !
				switch (pstruct->GetCode()) {
					// turn the E bit of the following structures on
					case OC_STRUCTURE_EPLANT_COAL:
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
							pairstructWH.first,
							pairstructWH.second,
							OC_E_RCI_RANGE,
							OC_STRUCTURE_ELECTRIC )
							== true) {
							pstruct->Set(OC_STRUCTURE_E);
							// See MainSim::RefreshSimValue(), 16 sep 2006
							//_iValue--;
						}
						break;

					// the ELINE structure have a shorter range
					case OC_STRUCTURE_ELINE:
						if (CheckRange(
							pairstructWH.first,
							pairstructWH.second,
							OC_E_E_RANGE,
							OC_STRUCTURE_ELECTRIC )
							== true) {
							pstruct->Set(OC_STRUCTURE_E);
							// See MainSim::RefreshSimValue(), 16 sep 2006
							//_iValue--;
						}
						break;

				// Ignored structures
					case OC_STRUCTURE_PARK:
					case OC_STRUCTURE_FLORA:
					case OC_STRUCTURE_ROAD:
						break;

				// What's the heck ?
					case OC_STRUCTURE_UNDEFINED:
					case OC_STRUCTURE_TEST:
					case OC_STRUCTURE_ANY:
					case OC_STRUCTURE_ELECTRIC:

				// Future asserts
					case OC_STRUCTURE_MILITARYDEPT:

				// What's the heck ?
					default:
						OPENCITY_DEBUG( "What's the heck ?" );
						assert( 0 );
						break;
				} // switch

				// mark the structure we have just processed
				pstruct->Set( OC_STRUCTURE_MARK );

				// put all the neighbour into the deque
				// if they are not yet marked (processed)
				// look for the neighour in the NORTH
				if ((pmapOfCity->GetNeighbourWH(
					pairstructWH.first, pairstructWH.second,
					nW, nH, OC_DIR_N ) == true )
					&&(pstruct = pbuildlayer->GetStructure( nW, nH ))
					&&(pstruct->IsSet(OC_STRUCTURE_MARK) == false )
					&&(dequeContain(make_pair(nW, nH)) == false))
					dequepairui.push_back(
						pair<uint,uint>( nW, nH) );

				// look for the neighour in the EAST
				if ((pmapOfCity->GetNeighbourWH(
					pairstructWH.first, pairstructWH.second,
					nW, nH, OC_DIR_E ) == true )
					&&(pstruct = pbuildlayer->GetStructure( nW, nH ))
					&&(pstruct->IsSet(OC_STRUCTURE_MARK) == false )
					&&(dequeContain(make_pair(nW, nH)) == false))
					dequepairui.push_back(
						pair<uint,uint>( nW, nH) );

				// look for the neighour in the SOUTH
				if ((pmapOfCity->GetNeighbourWH(
					pairstructWH.first, pairstructWH.second,
					nW, nH, OC_DIR_S ) == true )
					&&(pstruct = pbuildlayer->GetStructure( nW, nH ))
					&&(pstruct->IsSet(OC_STRUCTURE_MARK) == false )
					&&(dequeContain(make_pair(nW, nH)) == false))
					dequepairui.push_back(
						pair<uint,uint>( nW, nH) );

				// look for the neighour in the WEST
				if ((pmapOfCity->GetNeighbourWH(
					pairstructWH.first, pairstructWH.second,
					nW, nH, OC_DIR_W ) == true )
					&&(pstruct = pbuildlayer->GetStructure( nW, nH ))
					&&(pstruct->IsSet(OC_STRUCTURE_MARK) == false )
					&&(dequeContain(make_pair(nW, nH)) == false))
					dequepairui.push_back(
						pair<uint,uint>( nW, nH) );
			} // while

			// next EPLANT
			iter++;
		} // while vectorpairuiEPlant

		SDL_UnlockMutex( this->mutexMain );
	} // if run


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

	pstruct = pbuildlayer->GetStructure( w1, h1 );
	if ( pstruct != NULL )
		switch( pstruct->GetCode() ) {
			case OC_STRUCTURE_EPLANT_COAL:
				_iValueMax += OC_EPLANT_COAL_POWER;
				_uiNumberEPlant++;
				vectorpairuiEPlant.push_back( pair<uint, uint>( w1, h1 ) );
				break;
			default: // keep gcc happy
				break;
		}
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

   // remove the w, h of the main structure
	vector< pair<uint, uint> >::iterator iter;

	pstruct = pbuildlayer->GetStructure( mainW, mainH );
	if ( pstruct != NULL ) {
		enumStructCode = pstruct->GetCode();
		switch ( enumStructCode ) {
			case OC_STRUCTURE_PART:
			   // get the main structure pointer, and code
				pstruct = pstruct->GetMain();
				enumStructCode = pstruct->GetCode();

			   // Calculate the structure's range
				gpPropertyMgr->GetWLH( pstruct->GetGraphicCode(), sw, 4, sl, 4, sh, 1 );
				sw--; sl--;
			   // now look for the main w, h
				this->pmapOfCity->GetPossibleWH( mainW1, mainH1, -sw, -sl );
				this->pmapOfCity->GetPossibleWH( mainW2, mainH2,  sw,  sl );
				mainH = mainH1;
				while ( (boolFound == false)
				     && (mainH <= mainH2) ) {
					mainW = mainW1;
					while ( (boolFound == false)
					     && (mainW <= mainW2) ) {
					   // if the current structure at mainW, mainH
					   // is the main structure then we found it
						if ( pbuildlayer->GetStructure( mainW, mainH )
						  == pstruct )
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
				// See MainSim::RefreshSimValue(), 16 sep 2006
				//if (pstruct->IsSet( OC_STRUCTURE_E ) == true)
				//	_iValue++;
				break;

			default: // keep gcc happy
				break;
		}

		switch ( enumStructCode ) {
			case OC_STRUCTURE_EPLANT_COAL:
				_iValueMax -= OC_EPLANT_COAL_POWER;
				_uiNumberEPlant--;

			   // search for the pair of mainW, mainH in
			   // the "vectorpairuiEPlant"
			   // and delete it if found

				iter = std::find( vectorpairuiEPlant.begin(),
						  vectorpairuiEPlant.end(),
						  make_pair( mainW, mainH ) );
				if ( iter != vectorpairuiEPlant.end() )
					vectorpairuiEPlant.erase( iter );
				break;

			default: // keep gcc happy
				break;
		}
	}
}


   /*=====================================================================*/
bool
ElectricitySim::dequeContain( const pair<uint, uint> & pairui )
{
	if (find( this->dequepairui.begin(),  this->dequepairui.end(), pairui )
		!= this->dequepairui.end())
		return true;
	else
		return false;
}

































