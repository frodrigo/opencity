/***************************************************************************
						residentialsim.cpp  -  description
							-------------------
	begin                : sep 21th, 2003
	copyright            : (C) 2003-2010 by Duong Khang NGUYEN
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
#include "opencity_structure_type.h"

// OpenCity headers
#include "residentialsim.h"
#include "buildinglayer.h"
#include "structure.h"


   /*=====================================================================*/
ResidentialSim::ResidentialSim(
	SDL_mutex* mutex,
	BuildingLayer* pblayer,
	Map* pmap ):
Simulator( mutex, pblayer, pmap )
{
	OPENCITY_DEBUG( "RSim param ctor" );
}


   /*=====================================================================*/
ResidentialSim::~ResidentialSim()
{
	OPENCITY_DEBUG( "RSim dtor" );
}


   //========================================================================
   // int Main() const
   //
   // description: simulate the growth of a Residential square
   // algorithm  :
   //              1. get a random Residential structure
   //              2. look around for necessary structures
   //              3. IF there's all needed structures around THEN
   //                    IF chance < OC_SIMULATOR_UP THEN
   //                       levelUp()
   //                    ELSE
   //                       levelDown()
   //                    FI
   //                 ELSE
   //                    IF chance < OC_SIMULATOR_DOWN THEN
   //                       levelDown()
   //                    FI
   //                 FI
   //========================================================================
int
ResidentialSim::Main()
{
	static uint w, l;
	static Structure* pstruct;
	static bool boolLevelUp;
	static int iRandom;
	static OPENCITY_GRAPHIC_CODE oldGC;


	if (_eSimState != SIMULATOR_RUNNING)
		return 0;

// Get a random residential structure
	pstruct = _pBuildLayer->GetRandomStructure(w, l, OC_STRUCTURE_RES );
	if (pstruct == NULL)
		return 0;

	boolLevelUp = false;
//	OPENCITY_DEBUG( "Begin - ResidentialSim - w/l: " << w << "/" << l );

// Try to lock the mutex to prevent the others from deleting the structure
// pointed by "pstruct" while we're playing with
	SDL_LockMutex( _pMutexMain );

	pstruct->Unset(
		OC_STRUCTURE_W                  | OC_STRUCTURE_G |
		OC_STRUCTURE_R | OC_STRUCTURE_C | OC_STRUCTURE_I |
		OC_STRUCTURE_P );
	pstruct->Set( OC_STRUCTURE_R );

	// IF there's a P in range THEN
	if (CheckRange(w, l, OC_R_P_RANGE, OC_STRUCTURE_ROAD) == true)
		pstruct->Set( OC_STRUCTURE_P );
	// ELSE we need more traffic
	else
		_tiVariation[Simulator::OC_TRAFFIC]++;

	// Is there a C in range ?
	if (CheckRange(w, l, OC_R_C_RANGE, OC_STRUCTURE_COM) == true)
		pstruct->Set( OC_STRUCTURE_C );
	else
		_tiVariation[Simulator::OC_COMMERCIAL]++;

	// Is there a I in range ?
	if (CheckRange(w, l, OC_R_I_RANGE, OC_STRUCTURE_IND) == true)
		pstruct->Set( OC_STRUCTURE_I );
	else
		_tiVariation[Simulator::OC_INDUSTRIAL]++;

	if (pstruct->IsSet(
		OC_STRUCTURE_E |
		OC_STRUCTURE_R | OC_STRUCTURE_C | OC_STRUCTURE_I |
		OC_STRUCTURE_P ) == true )
		boolLevelUp = true;

	iRandom = rand() % 100;
	oldGC = pstruct->GetGraphicCode();
	if (boolLevelUp == true) {
	// Really levelup ?
		if (iRandom < OC_SIMULATOR_UP) {
			if ((this->CheckLevelUp(w, l, pstruct) == true)
				and (pstruct->LevelUp() == true)) {
				_pBuildLayer->ResizeStructure( w, l, oldGC );
				_iValue++;
				_tiVariation[Simulator::OC_RESIDENTIAL]--;
			}
		}
	}  // end if levelup
	else {
	// Really level down ?
		if (iRandom < OC_SIMULATOR_DOWN)
			if ((this->CheckLevelDown(w, l, pstruct) == true)
			&&  (pstruct->LevelDown() == true)) {
				_pBuildLayer->ResizeStructure( w, l, oldGC );
				_iValue--;
				_tiVariation[Simulator::OC_RESIDENTIAL]++;
			}
	}

// Let the other simulators run
	SDL_UnlockMutex( _pMutexMain );

//	OPENCITY_DEBUG( "End - ResidentialSim - w/l: " << w << "/" << l );

	return 0;
}


   /*=====================================================================*/
void
ResidentialSim::RemoveStructure(
	const uint & w1,
	const uint & h1,
	const uint & w2,
	const uint & h2 )
{
	Structure* pstruct = _pBuildLayer->GetStructure( w1, h1 );

   // if this is a R zone
   // and it has a positive value according to its level
   // then we remove its value from the sim.
	if (pstruct != NULL)
	if (pstruct->GetCode() == OC_STRUCTURE_RES)
	if (pstruct->GetLevel() - 1 > 0)
		_iValue -= pstruct->GetLevel()-1;
}
