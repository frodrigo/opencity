/***************************************************************************
						mapmaker.h    -  description
							-------------------
	begin                : july 2nd, 2006
	copyright            : (C) 2006 by Frédéric RODRIGO
	email                :
	
	$Id: CodingStyle.h 5 2006-06-17 23:53:07Z neoneurone $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#ifndef _OPENCITY_MAPMAKER_H_
#define _OPENCITY_MAPMAKER_H_ 1

#include "map.h"

/**
	Map generators and filters interface
*/
class MapMaker {
public:
	enum MAP_TYPE {
		PLAIN = 0,
		HILL,
		MOUNTAIN
	};

	MapMaker(
		const uint w,
		const uint h,
		const MAP_TYPE type );

	~MapMaker();

	int **getMap();

private:
	uint		_w;
	uint		_h;
	MAP_TYPE	_type;
	Map			*_map;


   /*=====================================================================*/
   /*                         PRIVATE     METHODS                         */
   /*=====================================================================*/
	void _generate();
};

#endif
