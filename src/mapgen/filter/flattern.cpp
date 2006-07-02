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
	
	float coef = max - min;
	
	for( uint x=0 ; x<map->getW() ; ++x )
		for( uint y=0 ; y<map->getH() ; ++y )
			map->setAt( x, y, powf((map->getAt(x,y)-min)/coef,_power)*coef+min );
}
