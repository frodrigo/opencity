/***************************************************************************
						shapeTopRound.cpp  -  description
							-------------------
	begin                : feb 10th, 2007
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

#include "shapeTopRound.h"

#include <math.h>

namespace MapGen
{


   /*=====================================================================*/
ShapeTopRound::ShapeTopRound( const uint w, const uint h, const float base, const float rude, const uint l ):
Shape(w,h),
_base(base),
_rude(rude),
_l(l)
{
	MAP_DEBUG( "ctor" );
	_w2 = w/2;
	_h2 = h/2;
	_c = _w2 > _h2 ? _w2 : _h2;
}


   /*=====================================================================*/
ShapeTopRound::~ShapeTopRound()
{
	MAP_DEBUG( "dtor" );
}


   /*=====================================================================*/
/*
 * gnuplot> splot [-100:100] [-100:100] -exp(abs(y)/100*10)/exp(10)
 */
float ShapeTopRound::value( const uint x, const uint y ) const
{
	int _x = x - _w2;
	int _y = y - _h2;

	#define max(a,b) ( (a)>(b) ? (a) : (b) )
	return ( -exp(max(fabsf(_x),fabsf(_y))/_c*_rude)/exp(_rude) + _base ) * _l;
}


}
