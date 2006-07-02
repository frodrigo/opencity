/***************************************************************************
						map.cpp  -  description
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

#include "map.h"

#include <cmath>
#include <fstream>
using namespace std;


   /*=====================================================================*/
Map::Map(
	const uint w,
	const uint h ):
_w(w),
_h(h)
{
	MAP_DEBUG( "ctor" );

	_map = new float*[_w];
	for( uint x=0 ; x<_w ; x++ )
	{
		_map[x] = new float[_h];
		for( uint y=0 ; y<_h ; y++ )
			_map[x][y] = 0.0;
	}
}


   /*=====================================================================*/
Map::~Map()
{
	MAP_DEBUG( "dtor" );

	for( uint x=0 ; x<_w ; x++ )
		delete [] _map[x];
	delete [] _map;
}


   /*=====================================================================*/
void Map::setAt(
		int x,
		int y,
		float value )
{
	_map[x%_w][y%_h] = value;
}


   /*=====================================================================*/
float Map::getAt(
		int x,
		int y ) const
{
	return _map[x%_w][y%_h];
}


   /*=====================================================================*/
bool Map::save(	const string &file )
{
	ofstream f( file.c_str(), ios::out );
		
		if( ! f )
		return false;
	else
	{
			f << "P2" << endl;
			f << _w << " " << _h << endl;
			f << "256" << endl;
		for( uint x=0 ; x<_w ; x++ )
			for( uint y=0 ; y<_h ; y++ )
				f << int(_map[x][y]) << endl;
			f.close();
		return true;
	}
}


   /*=====================================================================*/
Map *Map::crop(
		const uint w,
		const uint h ) const
{
	Map* map = new Map( w, h );

	for( uint x=0 ; x<w ; x++ )
		for( uint y=0 ; y<h ; y++ )
			map->setAt( x, y, getAt( x, y ) );

	return map;
}


   /*=====================================================================*/
int **Map::toIntArray() const
{
	int **map = new int*[_w];
	for( uint x=0 ; x<_w ; x++ )
	{
		map[x] = new int[_h];
		for( uint y=0 ; y<_h ; y++ )
			map[x][y] = (int) round( getAt( x, y ) );
	}

	return map;
}
