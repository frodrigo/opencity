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

namespace mapgen
{

   /*=====================================================================*/
Flattern::Flattern()
{
	MAP_DEBUG( "ctor1" );
}


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
	float min, max;
	_getMinMax( map, &min, &max );

	float coef = fabs( max - min );

	for( uint x=0 ; x<map->getW() ; ++x )
		for( uint y=0 ; y<map->getH() ; ++y )
		{
			if( min >= 0 )
			{
				map->setAt( x, y, powf((map->getAt(x,y)-min)/coef,_power)*coef+min );
			}
			else /* min < 0 and max > 0 */
			{
				float h = map->getAt(x,y);
				if( h >= 0 )
				{
					map->setAt( x, y, powf(h/max,_power)*max );
				}
				else
				{
					map->setAt( x, y, fabs(powf(fabs(h)/min,_power))*min );
				}
			}
		}
}

}
