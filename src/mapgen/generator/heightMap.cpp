/***************************************************************************
						heightMap.cpp  -  description
							-------------------
	begin                : April 1first, 2007
	copyright            : (C) 2007 by Frédéric RODRIGO
	email                : f.rodrigo free.fr
	
	$Id: diamon.cpp 124 2007-01-13 17:28:49Z neoneurone $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#include "heightMap.h"

#include "SDL_image.h"

namespace MapGen
{


   /*=====================================================================*/
HeightMap::HeightMap( const string heightMapFile ):
Generator( 0 ),
file(heightMapFile)
{
	MAP_DEBUG("ctor");
}


   /*=====================================================================*/
HeightMap::~HeightMap()
{
	MAP_DEBUG("dtor");
}


   /*=====================================================================*/
Map* HeightMap::render() const
{
	MAP_DEBUG("Try to load \"" + file + "\"");

	SDL_Surface *heighMapSurface = IMG_Load( file.c_str() );

	Map* map;
	if( heighMapSurface == NULL ) {
		MAP_INFO("Fail load \"" + file + "\" : " + IMG_GetError() );
		MAP_INFO("Use a 50x50 flat map inside." );
		map = new Map( 50, 50 );
	}
	else
	{
		map = new Map( heighMapSurface->w, heighMapSurface->h );

		for( int x=0 ; x < heighMapSurface->w ; ++x ) {
			for( int y=0 ; y < heighMapSurface->h ; ++y ) {
				Uint32 pixel = getPixel( heighMapSurface, x, y );

				Uint8 r;
				Uint8 g;
				Uint8 b;
				SDL_GetRGB( pixel, heighMapSurface->format, &r, &g, &b );

				Uint8 gray = Uint8( ( r + g + b ) / 3);

				map->setAt( x, y, gray-128 );
			}
		}

		SDL_FreeSurface( heighMapSurface );
	}

	return map;
}


Uint32 HeightMap::getPixel( SDL_Surface *surf, int x, int y ) const
{
	int bpp = surf->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
	case 2:
		return *(Uint16 *)p;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
	case 4:
		return *(Uint32 *)p;
	default:
		return 0;
	}
}

}
