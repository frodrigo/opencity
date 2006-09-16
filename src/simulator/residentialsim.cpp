/***************************************************************************
						residentialsim.cpp  -  description
							-------------------
	begin                : dim sep 21 2003
	copyright            : (C) 2003-2006 by Duong-Khang NGUYEN
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


	if (this->enumSimState == SIMULATOR_RUNNING) {
		// get a random residential structure
		pstruct = pbuildlayer->GetRandomStructure(
			w, l, OC_STRUCTURE_RES );

		if (pstruct != NULL) {
			boolLevelUp = false;

			// try to lock the mutex
			// prevent the others from deleting the structure
			// pointed by "pstruct" while we're playing with
			SDL_LockMutex( this->mutexMain );

			pstruct->Unset(
				OC_STRUCTURE_W                  | OC_STRUCTURE_G |
				OC_STRUCTURE_R | OC_STRUCTURE_C | OC_STRUCTURE_I |
				OC_STRUCTURE_P );
			pstruct->Set( OC_STRUCTURE_R );

			// is there a P in range ?
			if (CheckRange(
				w, l, OC_R_P_RANGE, OC_STRUCTURE_ROAD ) == true)
				pstruct->Set( OC_STRUCTURE_P );
			// is there a C in range ?
			if (CheckRange(
				w, l, OC_R_C_RANGE, OC_STRUCTURE_COM ) == true)
				pstruct->Set( OC_STRUCTURE_C );
			// is there a I in range ?
			if (CheckRange(
				w, l, OC_R_I_RANGE, OC_STRUCTURE_IND ) == true)
				pstruct->Set( OC_STRUCTURE_I );

			if (pstruct->IsSet(
				OC_STRUCTURE_E |
				OC_STRUCTURE_R | OC_STRUCTURE_C | OC_STRUCTURE_I |
				OC_STRUCTURE_P ) == true )
				boolLevelUp = true;

//debug
//cout << "ResidentialSim speaking: " << iValue
//     << " / w: " << w << " / l: " << l << endl;

			iRandom = rand() % 100;
			oldGC = pstruct->GetGraphicCode();
			if (boolLevelUp == true) {
			// really levelup ?
				if (iRandom < OC_SIMULATOR_UP) {
					if ((this->CheckLevelUp(w, l, pstruct) == true)
					&&  (pstruct->LevelUp() == true)) {
						pbuildlayer->ResizeStructure( w, l, oldGC );
						_iValue++;
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
					}
			}

			// let the others run
			SDL_UnlockMutex( this->mutexMain );
		} // if pstruct != NULL
	}  // if running

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
	Structure* pstruct = pbuildlayer->GetStructure( w1, h1 );

   // if this is a R zone
   // and it has a positive value according to its level
   // then we remove its value from the sim.
	if (pstruct != NULL)
	if (pstruct->GetCode() == OC_STRUCTURE_RES)
	if (pstruct->GetLevel() - 1 > 0)
		_iValue -= pstruct->GetLevel()-1;
}























