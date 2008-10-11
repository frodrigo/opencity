/***************************************************************************
						shaper.cpp  -  description
							-------------------
	begin                : feb 10th, 2007
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

#include "shaper.h"


namespace MapGen
{


   /*=====================================================================*/
Shaper::Shaper( const Shape *shape ):
_shape(shape)
{
	MAP_DEBUG( "ctor" );
}


   /*=====================================================================*/
Shaper::~Shaper()
{
	MAP_DEBUG( "dtor" );
	delete _shape;
}


   /*=====================================================================*/
void Shaper::apply( Map* map )
{
	uint w, h;

	w = map->getW();
	h = map->getL();

	for( uint x = 0 ; x < w; ++x )
		for( uint y = 0 ; y < h; ++y )
			map->setAt( x, y, map->getAt(x,y) + _shape->value(x,y) );
}


}
