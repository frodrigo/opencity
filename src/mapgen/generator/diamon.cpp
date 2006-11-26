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

#include <stdlib.h>
#include <math.h>

namespace MapGen
{

   /*=====================================================================*/
Diamon::Diamon( const uint seed, const uint order ):
Generator( seed ),
_side((uint)pow(2,order))
{
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
float Diamon::_all( const uint p ) const
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
	uint p = map->getW() / 2;

	/* loop en scale */
	while( p != 0 )
	{
		uint p2 = 2 * p;

		//Diamond
		for( uint i=p ; i<map->getW() ; i+=p2 )
			for( uint j=p ; j<map->getH() ; j+=p2 )
				map->setAt( i, j, ( map->getAt(i-p,j-p) + map->getAt(i+p,j-p) + map->getAt(i+p,j+p) + map->getAt(i-p,j+p) )/4 + _all(p) );

		// Square
		for( uint i=p ; i<map->getW() ; i+=p2 )
			for( uint j=0 ; j<map->getH() ; j+=p2 )
			map->setAt( i, j, ( map->getAt(i-p,j) + map->getAt(i+p,j) + map->getAt(i,j-p) + map->getAt(i,j+p) )/4 + _all(p) );
		for( uint i=0 ; i<map->getW() ; i+=p2 )
			for( uint j=p ; j<map->getH() ; j+=p2 )
				map->setAt( i, j, ( map->getAt(i-p,j) + map->getAt(i+p,j) + map->getAt(i,j-p) + map->getAt(i,j+p) )/4 + _all(p) );

		p /= 2;
	}
}

}
