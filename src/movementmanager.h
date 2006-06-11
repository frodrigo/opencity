/***************************************************************************
                          movementmanager.h  -  description
    $Id$
                             -------------------
    begin                : dim mai 16 2004
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

#ifndef _OPENCITY_MOVEMENTMANAGER_H_
#define _OPENCITY_MOVEMENTMANAGER_H_ 1

#include "main.h"

	#define OC_MOVEMENT_MAX		100

using std::vector;

class Movement;
class GraphicManager;
class Map;

   //========================================================================
   /** Offer a simple way to handle all the moving stuff
   */
class MovementManager {
public:
	MovementManager(
		const GraphicManager* gm,
		const Map* map
	);
	~MovementManager();

	const int
	Add(
		Movement* const pNew );

	Movement* const
	Remove(
		const uint & rcuiIndex );

	void
	Move(
		const int & rciIndex = -1 );

	void
	Display(
		const int & rciIndex = - 1 );


private:
	Movement* tabmvt [OC_MOVEMENT_MAX];
	const GraphicManager* pcGraphicMgr;
	const Map* pcMap;
};
#endif




































