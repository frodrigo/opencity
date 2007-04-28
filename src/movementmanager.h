/***************************************************************************
						movementmanager.h  -  description
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

#ifndef _OPENCITY_MOVEMENTMANAGER_H_
#define _OPENCITY_MOVEMENTMANAGER_H_ 1

#include "main.h"

#define OC_MOVEMENT_MAX		100


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

	const int Add( Movement* const pNew );

	const bool IsFull();


//========================================================================
/** Explicitly remove the specified or all the movement object from the
manager. The removed objects are destroyed by calling "delete" on them.
*/
	void Remove( const int ciIndex = -1 );


//========================================================================
/** Send the move order to the specified or all the movement object.
The object which can not move anymore is removed from the manager and
destroyed automatically by calling "delete" on it.
*/
	void Move( const int ciIndex = -1 );

	void Display( const int ciIndex = - 1 );


private:
	uint _uiCount;							///< The current number of elements
	Movement* tabmvt [OC_MOVEMENT_MAX];
	const GraphicManager* pcGraphicMgr;
	const Map* pcMap;
};
#endif




































