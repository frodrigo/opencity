/***************************************************************************
						mapmaker.h    -  description
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

#ifndef _OPENCITY_MAPMAKER_H_
#define _OPENCITY_MAPMAKER_H_ 1

#include "map.h"

#include <vector>
using std::vector;


namespace MapGen
{


// Forwar declarations
class Generator;
class Filter;


//========================================================================
/** Map generators and filters interface
*/
class MapMaker {
public:
	enum MAP_TYPE {
		PLAIN = 0,
		HILL,
		MOUNTAIN
	};

	enum WATER_TYPE {
		DRY = 0,
		LAKE,
		COAST
	};

	enum MAP_SHAPE_TYPE {
		NONE = 0,
		ISLAND,
		VOLCANO,
		CRATER
	};

	enum TREE_DENSITY_TYPE {
		SPARSE = 0,
		NORMAL,
		DENSE
	};

	MapMaker(
		const uint w,
		const uint h,
		const MAP_TYPE mapType,
		const WATER_TYPE waterType,
		const MAP_SHAPE_TYPE mapShapeType,
		const TREE_DENSITY_TYPE treeDensityType,
		const uint seed );

	~MapMaker();

	int* getMap();
	int* getTreeDensity();

private:
	uint		_w;
	uint		_h;

	MAP_TYPE	_mapType;
	WATER_TYPE	_waterType;
	MAP_SHAPE_TYPE	_mapShapeType;
	Map*		_map;

	TREE_DENSITY_TYPE	_treeDensityType;
	Map*		_treeDensity;

	uint		_seed;


//========================================================================
// Private methods
//========================================================================
	Map* _generate(
    	const Generator* generator,
    	vector<Filter*> filters ) const;

	void _generateMap( const uint seed );

	void _generateTreeDensity( const uint seed );

};


}


#endif
