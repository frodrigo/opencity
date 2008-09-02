/***************************************************************************
						map.cpp  -  description
							-------------------
	begin                : july 2nd, 2006
	copyright            : (C) 2006-2007 by Frédéric RODRIGO
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
#include <assert.h>

// Conditional libraries headers
#ifdef OPENCITY_PNG_SAVE
	#include "SDL.h"
	#include "pngfuncs.h"
#endif

using namespace std;


namespace MapGen
{


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
void
Map::setAt(
	int x,
	int y,
	float value )
{
    assert( x>=0 && y>=0 && x<(int)_w && y<(int)_h );
    _map[x][y] = value;
}


   /*=====================================================================*/
float
Map::getAt(
	int x,
	int y ) const
{
	// bordered getter
	x = x<0 ? 0 : x;
	x = x>=(long int)_w ? _w-1 : x;
	y = y<0 ? 0 : y;
	y = y>=(long int)_h ? _h-1 : y;
	return _map[x][y];
}


   /*=====================================================================*/
#ifdef OPENCITY_PNG_SAVE
bool Map::save(	const string &file )
{
	Uint8* buffer = new Uint8[_w*_h*3];
	assert( buffer != NULL );

	for( uint x=0 ; x<_w ; x++ )
		for( uint y=0 ; y<_h ; y++ )
		{
			uint offset = ( y*_w + x ) * 3;
			buffer[offset+0] = buffer[offset+1] = buffer[offset+2] = int(_map[x][y]) + 128;
		}

	bool retValue = false;

	SDL_Surface* heightMap = SDL_CreateRGBSurfaceFrom( buffer, _w, _h, 3*8, _w*3, 0, 0, 0, 0);

	if( heightMap == NULL )
	{
		MAP_INFO( "Cant create surface for heightmap : \"" << SDL_GetError() << "\"" );
		retValue = false;
	}
	else
	{
		if( png_save_surface( (file+".png").c_str(), heightMap ) == 0 )
		{
			MAP_INFO( "Map saved under \"" << file << "\"" );
			retValue = true;
		}
		else
		{
			MAP_ERROR( "C'ant save map under \"" << file << "\"" );
			retValue = false;
		}

		SDL_FreeSurface( heightMap );
	}

	delete[] buffer;

	return retValue;
}
#endif			// #ifdef OPENCITY_PNG_SAVE

   /*=====================================================================*/
Map* Map::crop(
		const uint w,
		const uint h ) const
{
	Map* map = new Map( w, h );

	// Crop centre of source to center of destination
	uint dcx = this->_w/2 - w/2;
	uint dcy = this->_h/2 - h/2;

	for( uint x=0 ; x<w ; ++x )
	    for( uint y=0 ; y<h ; ++y )
			map->setAt( x, y, getAt( x+dcx, y+dcy ) );

	return map;
}


   /*=====================================================================*/
int* Map::toIntArray() const
{
	int* map = new int[_w*_h];

	for( uint x=0 ; x<_w ; x++ )
		for( uint y=0 ; y<_h ; y++ )
			map[x+y*_w] = (int) getAt( x, y );

	return map;
}


   /*=====================================================================*/
Map* Map::halfShift() const
{
	Map* halfShiftMap = new Map( _w-1, _h-1 );

	// FIXME produce positive value even if one is negative => tree under water on coast (not displayed because not plane)
	for( uint x=0 ; x<_w-1 ; ++x )
		for( uint y=0 ; y<_h-1 ; ++y )
			halfShiftMap->setAt( x, y, ( getAt(x,y)+getAt(x+1,y)+getAt(x,y+1)+getAt(x+1,y+1) ) / 4 );

	return halfShiftMap;
}


}
