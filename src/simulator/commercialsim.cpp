/***************************************************************************
                          commercialsim.cpp  -  description
      $Id: commercialsim.cpp,v 1.9 2006/03/12 23:14:18 neoneurone Exp $
                             -------------------
    begin                : 31th jan, 2004
    copyright            : (C) 2004-2006 by Duong-Khang NGUYEN
    email                : neoneurone @ users sourceforge net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

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


	if (this->enumSimState == SIMULATOR_RUNNING) {
	// get a random commercial structure
		pstruct = pbuildlayer->GetRandomStructure(
			w, l, OC_STRUCTURE_COM );

		if (pstruct != NULL) {
			boolLevelUp = false;

		// try to lock the mutex
		// prevent the others from deleting the structure
		// pointed by "pstruct" while we're playing with
			SDL_LockMutex( this->mutexMain );

			pstruct->Unset(
				OC_STRUCTURE_W |                  OC_STRUCTURE_G |
				OC_STRUCTURE_R | OC_STRUCTURE_C | OC_STRUCTURE_I |
				OC_STRUCTURE_P );
			pstruct->Set( OC_STRUCTURE_C );

		// is there a P in range ?
			if (CheckRange(
				w, l, OC_C_P_RANGE, OC_STRUCTURE_ROAD ) == true)
				pstruct->Set( OC_STRUCTURE_P );
		// is there a R in range ?
			if (CheckRange(
				w, l, OC_C_R_RANGE, OC_STRUCTURE_RES ) == true)
				pstruct->Set( OC_STRUCTURE_R );
		// is there a I in range ?
			if (CheckRange(
				w, l, OC_C_I_RANGE, OC_STRUCTURE_IND ) == true)
				pstruct->Set( OC_STRUCTURE_I );

			if (pstruct->IsSet(
				OC_STRUCTURE_E |
				OC_STRUCTURE_R | OC_STRUCTURE_C | OC_STRUCTURE_I |
				OC_STRUCTURE_P ) == true )
				boolLevelUp = true;

//debug
//cout << "CommercialSim speaking: " << iValue
//     << " / w: " << w << " / l: " << l << endl;

			iRandom = rand() % 100;
			oldGC = pstruct->GetGraphicCode();
			if (boolLevelUp == true) {
			// really levelup ?
				if (iRandom < OC_SIMULATOR_UP) {
					if ((this->CheckLevelUp(w, l, pstruct) == true)
					&&  (pstruct->LevelUp() == true)) {
						pbuildlayer->ResizeStructure( w, l, oldGC );
						this->iValue++;
					}
				}
			}  // end if levelup
			else {
			// really level down ?
				if (iRandom < OC_SIMULATOR_DOWN)
					if ((this->CheckLevelDown(w, l, pstruct) == true)
					&&  (pstruct->LevelDown() == true)) {
						pbuildlayer->ResizeStructure( w, l, oldGC );
						this->iValue--;
					}
			}

			SDL_UnlockMutex( this->mutexMain );
		} // if pstruct != NULL
	}  // if running

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
	if (pstruct != NULL)
	if (pstruct->GetCode() == OC_STRUCTURE_COM)
		this->iValue -= pstruct->GetLevel()-1;
}
































