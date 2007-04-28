/***************************************************************************
					movementmanager.cpp  -  description
							-------------------
	begin                : may 16th, 2004
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

// OpenCity headers
#include "movement.h"
#include "movementmanager.h"
#include "graphicmanager.h"


   /*=====================================================================*/
MovementManager::MovementManager(
	const GraphicManager* gm,
	const Map* map):
_uiCount(0),
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
		tabmvt[ i ] = NULL;		// Safe
	}
	_uiCount = 0;
}


   /*=====================================================================*/
const int
MovementManager::Add(
	Movement* const pNew )
{
	for (uint i = 0; i < OC_MOVEMENT_MAX; i++) {
		if (tabmvt[ i ] == NULL) {
			tabmvt[ i ] = pNew;
			++_uiCount;
			return i;
		}
	}

	return -1;
}


   /*=====================================================================*/
const bool
MovementManager::IsFull()
{
	return _uiCount == OC_MOVEMENT_MAX;
}


   /*=====================================================================*/
void
MovementManager::Remove(
	const int ciIndex )
{
// IF the user has specified the index of the element
// THEN only remove that element
	if ((ciIndex > -1) && (ciIndex < OC_MOVEMENT_MAX )) {
		if (tabmvt[ ciIndex ] != NULL) {
			delete tabmvt[ ciIndex ];
			tabmvt[ ciIndex ] = NULL;
			--_uiCount;
		}
		return;
	}

// ELSE remove all the contained movement
	for (uint i = 0; i < OC_MOVEMENT_MAX; i++) {
		if (tabmvt[ i ] != NULL) {
			delete tabmvt[ i ];
			tabmvt[ i ] = NULL;
		}
	}
	_uiCount = 0;
}


   /*=====================================================================*/
void
MovementManager::Move(
	const int ciIndex )
{
// IF the user has specified the index of the element
// THEN only call that element
	if ((ciIndex > -1) && (ciIndex < OC_MOVEMENT_MAX )) {
		if (tabmvt[ ciIndex ] != NULL and tabmvt[ ciIndex ]->Move() == false) {
			delete tabmvt[ ciIndex ];
			tabmvt[ ciIndex ] = NULL;
			--_uiCount;
		}
		return;
	}

// ELSE move all the contained movement
	for (uint i = 0; i < OC_MOVEMENT_MAX; i++) {
		if (tabmvt[ i ] != NULL and tabmvt[ i ]->Move() == false) {
			delete tabmvt[ i ];
			tabmvt[ i ] = NULL;
			--_uiCount;
		}
	}
}


   /*=====================================================================*/
void
MovementManager::Display(
	const int ciIndex )
{
// IF the user has specified the index of the element
// THEN only call that element
	if ((ciIndex > -1) && (ciIndex < OC_MOVEMENT_MAX )) {
		if (tabmvt[ ciIndex ] != NULL)
			this->pcGraphicMgr->Display(
				tabmvt[ ciIndex ]->_fCurrentW,
				tabmvt[ ciIndex ]->_fCurrentL,
				tabmvt[ ciIndex ]->_fCurrentH,
				tabmvt[ ciIndex ] );
		return;
	}

// ELSE display all the contained movement
	for (uint i = 0; i < OC_MOVEMENT_MAX; i++) {
		if (tabmvt[ i ] != NULL)
			this->pcGraphicMgr->Display(
				tabmvt[ i ]->_fCurrentW,
				tabmvt[ i ]->_fCurrentL,
				tabmvt[ i ]->_fCurrentH,
				tabmvt[ i ] );
	}
}

































