/***************************************************************************
						mapmaker.cpp    -  description
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

#include "mapmaker.h"

#include "diamon.h"
#include "gaussblur.h"
#include "flattern.h"
#include "normalize.h"

#include <cmath>
#include <vector>
using std::vector;


   /*=====================================================================*/
MapMaker::MapMaker(
	const uint w,
	const uint h,
	const MAP_TYPE type ):
_w(w),
_h(h),
_type(type),
_map(NULL)
{
	MAP_DEBUG( "ctor" );

	_generate();
}


   /*=====================================================================*/
MapMaker::~MapMaker()
{
	MAP_DEBUG( "dtor" );

	if( _map != NULL )
		delete _map;
}


   /*=====================================================================*/
void MapMaker::_generate()
{
	Generator *generator;
	vector<Filter*> filters;
	
	// Select generator
	switch( _type )
	{
		default:
			{
				uint largerSide = _w > _h ? _w : _h;
				uint side = (uint) ceil( log2( (float)largerSide ) );
				generator = new Diamon( side );
			} break;
	}

	// Add filter
	switch( _type )
	{
		case PLAIN :
			filters.push_back( new Flattern(4) );
			filters.push_back( new GaussBlur(2) );
			filters.push_back( new Normalize(0,5) );
			break;
		case MOUNTAIN :
			filters.push_back( new Flattern(1) );
			filters.push_back( new GaussBlur(2) );
			filters.push_back( new Normalize(0,20) );
			break;
		default:
		case HILL :
			filters.push_back( new Flattern(2) );
			filters.push_back( new GaussBlur(2) );
			filters.push_back( new Normalize(0,10) );
			break;
	}

	// Render the map
	Map *map = generator->render();
	delete generator;

	// Apply filters
	for( vector<Filter*>::iterator i=filters.begin() ; i!=filters.end() ; i++ )
	{
		(*i)->apply( map );
		delete *i;
	}

	_map = map->crop( _w, _h );
	delete map;

	int **intMap = _map->toIntArray();
}


int **MapMaker::getMap()
{
	return _map->toIntArray();
}
