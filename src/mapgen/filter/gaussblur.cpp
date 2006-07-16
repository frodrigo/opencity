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

namespace mapgen
{

   /*=====================================================================*/
GaussBlur::GaussBlur( const uint length ):
_length(length)
{
	_convulsion = _blur( _length );
}


   /*=====================================================================*/
GaussBlur::~GaussBlur()
{
	delete [] _convulsion;
}


   /*=====================================================================*/
void GaussBlur::apply( Map* map )
{

	Map* tMap = new Map( map->getW(), map->getH() );

	for( uint x=0 ; x<map->getW() ; ++x )
		for( uint y=0 ; y<map->getH() ; ++y )
		{
			float sum = 0;
			for( uint i=0; i<_length*2+1 ; ++i )
				sum += _convulsion[i] * map->getAt( x+i-_length, y );
				tMap->setAt( x, y, sum );
		}

	for( uint x=0 ; x<map->getW() ; ++x )
		for( uint y=0 ; y<map->getH() ; ++y )
		{
			float sum = 0;
			for( uint i=0; i<_length*2+1 ; ++i )
				sum += _convulsion[i] * tMap->getAt( x, y+i-_length );
				map->setAt( x, y, sum );
		}

	delete tMap;
}


   /*=====================================================================*/
float GaussBlur::_gauss(
	const uint x,
	const uint width )
{
	return exp( -1.0 / width * x * x );
}


   /*=====================================================================*/
float *GaussBlur::_blur( const uint length )
{
	float *weights = new float[length*2+1];
	float sum = 0;

	for( uint x=0 ; x<length*2+1 ; ++x )
		sum += weights[x] = _gauss(x-length,length);

	for( uint x=0 ; x<length*2+1 ; ++x )
			weights[x] /= sum;

	return weights;
}

}
