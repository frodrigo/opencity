/***************************************************************************
						contextualizer.cpp  -  description
							-------------------
	begin                : jun 19th, 2007
	copyright            : (C) 2007 by Frédéric RODRIGO
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

#include "contextualizer.h"


namespace MapGen
{


   /*=====================================================================*/
Contextualizer::Contextualizer( const Context* context, const Map* contextMap ):
_context(context),
_contextMap(contextMap)
{
	MAP_DEBUG( "ctor" );
}


   /*=====================================================================*/
Contextualizer::~Contextualizer()
{
	MAP_DEBUG( "dtor" );
	delete _context;
}


   /*=====================================================================*/
void Contextualizer::apply( Map* map )
{
	uint w = map->getW();
	uint h = map->getL();

	for( uint x=0 ; x<w ; ++x )
		for( uint y=0 ; y<h ; ++y )
			map->setAt( x, y, _context->contextualize( map->getAt(x,y), _contextMap->getAt(x,y) ) );
}


}
