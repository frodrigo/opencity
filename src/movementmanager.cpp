/***************************************************************************
                          movementmanager.cpp  -  description
                             -------------------
    begin                : dim mai 16 2004
    modified             : sam jun 5  2004
    copyright            : (C) 2004 by Duong-Khang NGUYEN
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

#include "movement.h"
#include "movementmanager.h"

#include "graphicmanager.h"

   /*=====================================================================*/
MovementManager::MovementManager(
	const GraphicManager* gm,
	const Map* map):
pcGraphicMgr( gm ),
pcMap( map )
{
	OPENCITY_DEBUG("ctor");

   // tab movement initialization
	for (uint i = 0; i < OC_MOVEMENT_MAX; i++) {
		tabmvt[ i ] = NULL;
	}
}


   /*=====================================================================*/
MovementManager::~MovementManager()
{
	OPENCITY_DEBUG("dtor");

	for (uint i = 0; i < OC_MOVEMENT_MAX; i++) {
		delete tabmvt[ i ];
	}
}


   /*=====================================================================*/
const int
MovementManager::Add(
	Movement* const pNew )
{
	for (uint i = 0; i < OC_MOVEMENT_MAX; i++) {
		if (tabmvt[ i ] == NULL) {
			tabmvt[ i ] = pNew;
			return i;
		}
	}

	return -1;
}


   /*=====================================================================*/
Movement* const
MovementManager::Remove(
	const uint & rcuiIndex )
{
	Movement* pRemoved = tabmvt[ rcuiIndex ];
	tabmvt[ rcuiIndex ] = NULL;

   // WARNING: pRemoved is not deleted !
	return pRemoved;
}


   /*=====================================================================*/
void
MovementManager::Move(
	const int & rciIndex )
{
   // if the user has specified the index of the element
   // then only call that element
	if ((rciIndex > -1) && (rciIndex < OC_MOVEMENT_MAX )) {
		if (tabmvt[ rciIndex ] != NULL)
		if (tabmvt[ rciIndex ]->Move() == false) {
			delete tabmvt[ rciIndex ];
			tabmvt[ rciIndex ] = NULL;
		}
		return;
	}

   // move all the contained movement
	for (uint i = 0; i < OC_MOVEMENT_MAX; i++) {
		if (tabmvt[ i ] != NULL)
		if (tabmvt[ i ]->Move() == false) {
			delete tabmvt[ i ];
			tabmvt[ i ] = NULL;
		}
	}
}


   /*=====================================================================*/
void
MovementManager::Display(
	const int & rciIndex )
{
   // if the user has specified the index of the element
   // then only call that element
	if ((rciIndex > -1) && (rciIndex < OC_MOVEMENT_MAX )) {
		if (tabmvt[ rciIndex ] != NULL)
			this->pcGraphicMgr->Display(
				tabmvt[ rciIndex ]->fCurrentW,
				tabmvt[ rciIndex ]->fCurrentH,
				tabmvt[ rciIndex ] );
		return;
	}

   // move all the contained movement
	for (uint i = 0; i < OC_MOVEMENT_MAX; i++) {
		if (tabmvt[ i ] != NULL)
			this->pcGraphicMgr->Display(
				tabmvt[ i ]->fCurrentW,
				tabmvt[ i ]->fCurrentH,
				tabmvt[ i ] );
	}
}

































