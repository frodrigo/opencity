/***************************************************************************
						flattern.cpp  -  description
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

#include "flattern.h"
#include "map.h"

#include <cmath>

namespace MapGen
{


   /*=====================================================================*/
Flattern::Flattern( const float power ):
_power(power)
{
	MAP_DEBUG( "ctor2" );
}


   /*=====================================================================*/
Flattern::~Flattern()
{
	MAP_DEBUG( "dtor" );
}


   /*=====================================================================*/
void Flattern::apply( Map* map )
{
	uint x = 0, y = 0;
	uint w = 0, l = 0;
	float min = 0, max = 0;

	_getMinMax( map, min, max );
	w = map->getW();
	l = map->getL();

	float coef = fabs( max - min );

	float h = 0;
	for( x = 0; x < w; ++x )
		for( y = 0; y < l; ++y ) {
			if( min >= 0 ) {
				map->setAt( x, y, powf((map->getAt(x,y)-min)/coef,_power)*coef+min );
			}
			else { /* min < 0 and max > 0 */
				h = map->getAt(x,y);
				if( h >= 0 ) {
					map->setAt( x, y, powf(h/max,_power)*max );
				}
				else {
					map->setAt( x, y, fabs(powf(fabs(h)/min,_power))*min );
				}
			}
		} // for
}


}
