/***************************************************************************
						normalize.cpp  -  description
							-------------------
	begin                : july 2nd, 2006
	copyright            : (C) 2006 by Frédéric RODRIGO
	email                : f.rodrigo free.fr
	
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

#include "normalize.h"


namespace MapGen
{


   /*=====================================================================*/
Normalize::Normalize(
	const float min,
	const float max ):
_min(min),
_max(max)
{
	MAP_DEBUG( "ctor" );
}


   /*=====================================================================*/
Normalize::~Normalize()
{
	MAP_DEBUG( "dtor" );
}


   /*=====================================================================*/
void Normalize::apply( Map* map )
{
	float min, max;
	_getMinMax( map, min, max );

	uint w = map->getW();
	uint h = map->getL();

	float a = (_max-_min) / (max-min);
	float b = -min * a + _min;

	for( uint x = 0 ; x < w; ++x )
		for( uint y = 0 ; y < h; ++y )
			map->setAt( x, y, a*map->getAt(x,y)+b );
}


}
