/***************************************************************************
						gaussblur.cpp  -  description
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

#include "gaussblur.h"

#include <cmath>

namespace MapGen
{


   /*=====================================================================*/
GaussBlur::GaussBlur( const uint length ):
_uiLength(length)
{
	_afConvulsion = _blur( _uiLength );
}


   /*=====================================================================*/
GaussBlur::~GaussBlur()
{
	delete [] _afConvulsion;
}


   /*=====================================================================*/
void
GaussBlur::apply( Map* map )
{
	float sum = 0;

	Map* tMap = new Map( map->getW(), map->getL() );

	uint w = map->getW();
	uint h = map->getL();

	uint size = _uiLength*2+1;
	for( uint x = 0; x < w; ++x )
		for( uint y = 0; y < h; ++y ) {
			sum = 0;

			for( uint i = 0; i < size; ++i )
				sum += _afConvulsion[i] * map->getAt( x+i-_uiLength, y );
			tMap->setAt( x, y, sum );
		}

	for( uint x = 0; x < w; ++x )
		for( uint y = 0; y < h; ++y ) {
			sum = 0;

			for( uint i = 0; i < size; ++i )
				sum += _afConvulsion[i] * tMap->getAt( x, y+i-_uiLength );
			map->setAt( x, y, sum );
		}

	delete tMap;
}


   /*=====================================================================*/
float
GaussBlur::_gauss(
	const uint x,
	const uint width )
{
	return exp( -1.0 / width * x * x );
}


   /*=====================================================================*/
float
*GaussBlur::_blur( const uint length )
{
	uint size = length*2+1;
	float* weights = new float[size];
	float sum = 0;

	uint x = 0;
	for( x = 0; x < size; ++x ) {
		weights[x] = _gauss(x-length, length);
		sum += weights[x];
	}

	for( x = 0; x < size; ++x )
		weights[x] /= sum;

	return weights;
}


}
