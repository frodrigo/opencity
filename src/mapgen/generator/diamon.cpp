/***************************************************************************
						diamon.cpp  -  description
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

#include "diamon.h"

#include <cmath>
#include <cstdlib>

namespace MapGen
{


   /*=====================================================================*/
Diamon::Diamon( const uint seed, const uint order ):
Generator( seed ),
_side((uint)pow(2.0,(int)order))
{
	MAP_DEBUG("ctor");
}


   /*=====================================================================*/
Diamon::~Diamon()
{
	MAP_DEBUG("dtor");
}


   /*=====================================================================*/
Map* Diamon::render() const
{
	Map* map = new Map( _side, _side );
	map->setAt( 0,0, 128 );
	_sub( map, 0, 0, _side-1, _side-1 );
	return map;
}


   /*=====================================================================*/
float Diamon::_all( const uint p )
{
	return ((float)rand())/RAND_MAX * p * 2 - p;
}


   /*=====================================================================*/
void Diamon::_sub(
	Map* map,
	const uint x1,
	const uint y1,
	const uint x2,
	const uint y2 ) const
{
	uint mapW = 0, mapL = 0;
	uint p = map->getW() / 2;

	mapW = map->getW();
	mapL = map->getL();

	/* loop en scale */
	uint p2 = 0;
	uint i = 0, j = 0;
	while ( p != 0 ) {
		p2 = 2*p;

		#define getAt(x,y) getAt( (x)%mapW, (y)%mapL )

		// Diamond
		for ( i = p; i < mapW; i+=p2 )
			for ( j = p; j < mapL; j+=p2 )
				map->setAt(
					i, j,
					( map->getAt(i-p,j-p) + map->getAt(i+p,j-p) + map->getAt(i+p,j+p) + map->getAt(i-p,j+p) )/4 + _all(p)
				);

		// Square
		for ( i = p; i < mapW; i+=p2 )
			for ( j = 0; j < mapL; j+=p2 )
				map->setAt(
					i, j,
					( map->getAt(i-p,j) + map->getAt(i+p,j) + map->getAt(i,j-p) + map->getAt(i,j+p) )/4 + _all(p)
				);

		for ( i = 0; i < mapW; i+=p2 )
			for ( j = p; j < mapL; j+=p2 )
				map->setAt(
					i, j,
					( map->getAt(i-p,j) + map->getAt(i+p,j) + map->getAt(i,j-p) + map->getAt(i,j+p) )/4 + _all(p)
				);

		#undef getAt

		p /= 2;
	}
}

}
