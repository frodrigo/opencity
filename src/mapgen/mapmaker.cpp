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
#include "shaper.h"
#include "choper.h"
#include "shapeBubble.h"
#include "shapeTopRound.h"
#include "shapeVolcano.h"

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
	const MAP_SHAPE_TYPE mapShapeType,
	const TREE_DENSITY_TYPE treeDensityType,
	const uint seed ):
_w(w),
_h(h),
_mapType(mapType),
_waterType(waterType),
_mapShapeType(mapShapeType),
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

	// Set waterLevel
	int waterLevel;
	switch( _waterType )
	{
		case DRY :
			waterLevel = 0;
			break;
		case COAST :
			waterLevel = 2;
			break;
		default:
		case LAKE :
			waterLevel = 1;
			break;
	}

	// Add map type filter
	int minLevel, maxLevel;
	switch( _mapType )
	{
		case PLAIN :
			minLevel = -3*waterLevel;
			maxLevel = 5;
			filters.push_back( new Normalize(minLevel,maxLevel) );
			filters.push_back( new Flattern(3) );
			break;
		case MOUNTAIN :
			minLevel = -10*waterLevel;
			maxLevel = 20;
			filters.push_back( new Normalize(minLevel,maxLevel) );
			filters.push_back( new Flattern(1) );
			break;
		default:
		case HILL :
			minLevel = -5*waterLevel;
			maxLevel = 10;
			filters.push_back( new Normalize(minLevel,maxLevel) );
			filters.push_back( new Flattern(2) );
			break;
	}

	// Add water filter
	switch( _mapShapeType )
	{
		case ISLAND :
			filters.push_back( new Shaper( new ShapeBubble(_w,_h,0,3,-minLevel/2) ) );
			filters.push_back( new Choper( new ShapeTopRound(_w,_h,1.-4./maxLevel/maxLevel,5,maxLevel) ) );
			filters.push_back( new Choper( new ShapeBubble(_w,_h,.5,1,maxLevel) ) );
			break;
		case VOLCANO :
			filters.push_back( new Shaper( new ShapeVolcano(_w,_h,0,0.3,maxLevel) ) );
			break;
		case CRATER :
			break;
		default:
		case NONE :
			break;
	}

	// Smooth the result
	filters.push_back( new GaussBlur(2) );

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
