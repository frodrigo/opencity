/***************************************************************************
						mapmaker.cpp    -  description
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

#include "mapmaker.h"

#include "diamon.h"
#include "gaussblur.h"
#include "flattern.h"
#include "normalize.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
using std::vector;

namespace MapGen
{

   /*=====================================================================*/
MapMaker::MapMaker(
	const uint w,
	const uint h,
	const MAP_TYPE mapType,
	const WATER_TYPE waterType,
	const TREE_DENSITY_TYPE treeDensityType,
	const uint seed ):
_w(w),
_h(h),
_mapType(mapType),
_waterType(waterType),
_map(NULL),
_treeDensityType(treeDensityType),
_treeDensity(NULL),
_seed(seed)
{
	MAP_DEBUG( "ctor" );

	srand( seed );
	uint mapSeed = rand();
	uint treeDensitySeed = rand();

	_generateMap( mapSeed );
	_generateTreeDensity( treeDensitySeed );

	srand( time(NULL) );
}


   /*=====================================================================*/
MapMaker::~MapMaker()
{
	MAP_DEBUG( "dtor" );

	if( _map != NULL )
		delete _map;

	if( _treeDensity != NULL )
		delete _treeDensity;
}


   /*=====================================================================*/
Map* MapMaker::_generate(
	const Generator* generator,
	vector<Filter*> filters ) const
{
	// Render the map
	Map* map = generator->render();
	delete generator;

	// Apply filters
	for( vector<Filter*>::iterator i=filters.begin() ; i!=filters.end() ; i++ )
	{
		(*i)->apply( map );
		delete *i;
	}

	Map* cropedMap = map->crop( _w, _h );
	delete map;
	return cropedMap;
}


   /*=====================================================================*/
void MapMaker::_generateMap( const uint seed )
{
	Generator* generator;
	vector<Filter*> filters;

	// Select generator
	switch( _mapType )
	{
		default:
			{
				uint largerSide = _w > _h ? _w : _h;
				uint side = (uint) ceil( log2( (float)largerSide ) );
				generator = new Diamon( seed, side );
			} break;
	}

	// Add filter
	switch( _mapType )
	{
		case PLAIN :
			filters.push_back( new Normalize(-3*_waterType,5) );
			filters.push_back( new Flattern(3) );
			filters.push_back( new GaussBlur(2) );
			break;
		case MOUNTAIN :
			filters.push_back( new Normalize(-10*_waterType,20) );
			filters.push_back( new Flattern(1) );
			filters.push_back( new GaussBlur(2) );
			break;
		default:
		case HILL :
			filters.push_back( new Normalize(-5*_waterType,10) );
			filters.push_back( new Flattern(2) );
			filters.push_back( new GaussBlur(2) );
			break;
	}

	_map = _generate( generator, filters );
}


   /*=====================================================================*/
void MapMaker::_generateTreeDensity( const uint seed )
{
	Generator* generator;
	vector<Filter*> filters;

	// Select generator
	switch( _treeDensityType )
	{
		default:
			{
				uint largerSide = _w > _h ? _w : _h;
				uint side = (uint) ceil( log2( (float)largerSide ) );
				generator = new Diamon( seed, side );
			} break;
	}

	// Add filter
	switch( _treeDensityType )
	{
		case SPARSE :
			filters.push_back( new Flattern(8) );
			filters.push_back( new GaussBlur(2) );
			filters.push_back( new Normalize(0,8) );
			break;
		case DENSE :
			filters.push_back( new Flattern(1) );
			filters.push_back( new GaussBlur(2) );
			filters.push_back( new Normalize(3,20) );
			break;
		default:
		case NORMAL :
			filters.push_back( new Flattern(4) );
			filters.push_back( new GaussBlur(2) );
			filters.push_back( new Normalize(0,10) );
			break;
	}

	_treeDensity = _generate( generator, filters );
}


   /*=====================================================================*/
int* MapMaker::getMap()
{
	return _map->toIntArray();
}


   /*=====================================================================*/
int* MapMaker::getTreeDensity()
{
	return _treeDensity->toIntArray();
}


}
