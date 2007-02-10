/***************************************************************************
						choper.cpp  -  description
							-------------------
	begin                : fev 10th, 2007
	copyright            : (C) 2006 by Frédéric RODRIGO
	email                : f.rodrigo free.fr
	
	$Id: choper.cpp 124 2007-01-13 17:28:49Z neoneurone $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#include "choper.h"


namespace MapGen
{


   /*=====================================================================*/
Choper::Choper( const Shape *shape ):
_shape(shape)
{
	MAP_DEBUG( "ctor" );
}


   /*=====================================================================*/
Choper::~Choper()
{
	MAP_DEBUG( "dtor" );
	delete _shape;
}


   /*=====================================================================*/
void Choper::apply( Map* map )
{
	uint w, h;

	w = map->getW();
	h = map->getL();

	for( uint x = 0 ; x < w; ++x )
		for( uint y = 0 ; y < h; ++y )
		{
			float level = map->getAt(x,y);
			float shape = _shape->value(x,y);
			level = level > shape ? shape : level;
			map->setAt( x, y, level );
		}
}


}
