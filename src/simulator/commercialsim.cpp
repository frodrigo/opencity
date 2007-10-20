/***************************************************************************
						commercialsim.cpp  -  description
							-------------------
	begin                : jan 31th, 2004
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

// Useful enumeration
#include "opencity_structure_type.h"

// OpenCity headers
#include "commercialsim.h"
#include "buildinglayer.h"
#include "structure.h"


   /*======================================================================*/
CommercialSim::CommercialSim(
	SDL_mutex* mutex,
	BuildingLayer* pblayer,
	Map* pmap ):
Simulator( mutex, pblayer, pmap )
{
	OPENCITY_DEBUG( "CSim param ctor" );
}


   /*======================================================================*/
CommercialSim::~CommercialSim()
{
	OPENCITY_DEBUG( "CSim dtor" );
}


   /*======================================================================*/
int
CommercialSim::Main()
{
	static uint w, l;
	static Structure* pstruct;
	static bool boolLevelUp;
	static int iRandom;
	static OPENCITY_GRAPHIC_CODE oldGC;


	if (this->enumSimState != SIMULATOR_RUNNING)
		return 0;

// Get a random commercial structure
	pstruct = pbuildlayer->GetRandomStructure(w, l, OC_STRUCTURE_COM);
	if (pstruct == NULL)
		return 0;

	boolLevelUp = false;
// Try to lock the mutex to prevent the others from deleting the structure
// pointed by "pstruct" while we're playing with
	SDL_LockMutex( this->mutexMain );

	pstruct->Unset(
		OC_STRUCTURE_W |                  OC_STRUCTURE_G |
		OC_STRUCTURE_R | OC_STRUCTURE_C | OC_STRUCTURE_I |
		OC_STRUCTURE_P );
	pstruct->Set( OC_STRUCTURE_C );

// is there a P in range ?
	if (CheckRange(w, l, OC_C_P_RANGE, OC_STRUCTURE_ROAD) == true)
		pstruct->Set( OC_STRUCTURE_P );
	else
		_tiVariation[Simulator::OC_TRAFFIC]++;

// is there a R in range ?
	if (CheckRange(w, l, OC_C_R_RANGE, OC_STRUCTURE_RES) == true)
		pstruct->Set( OC_STRUCTURE_R );
	else
		_tiVariation[Simulator::OC_RESIDENTIAL]++;

// is there a I in range ?
	if (CheckRange(w, l, OC_C_I_RANGE, OC_STRUCTURE_IND) == true)
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
	// really levelup ?
		if (iRandom < OC_SIMULATOR_UP) {
			if ((this->CheckLevelUp(w, l, pstruct) == true)
			&&  (pstruct->LevelUp() == true)) {
				pbuildlayer->ResizeStructure( w, l, oldGC );
				_iValue++;
				_tiVariation[Simulator::OC_COMMERCIAL]--;
			}
		}
	}  // end if levelup
	else {
	// really level down ?
		if (iRandom < OC_SIMULATOR_DOWN)
			if ((this->CheckLevelDown(w, l, pstruct) == true)
			&&  (pstruct->LevelDown() == true)) {
				pbuildlayer->ResizeStructure( w, l, oldGC );
				_iValue--;
				_tiVariation[Simulator::OC_COMMERCIAL]++;
			}
	}

	SDL_UnlockMutex( this->mutexMain );


	return 0;
}


   /*=====================================================================*/
void
CommercialSim::RemoveStructure(
	const uint & w1,
	const uint & h1,
	const uint & w2,
	const uint & h2 )
{
	Structure* pstruct = pbuildlayer->GetStructure( w1, h1 );

// if this is a C zone
// and it has a positive value according to its level
// then we remove its value from the sim.
// and if level is 0 ?
	if (pstruct != NULL)
	if (pstruct->GetCode() == OC_STRUCTURE_COM)
	if (pstruct->GetLevel() - 1 > 0)
		_iValue -= pstruct->GetLevel()-1;
}
































