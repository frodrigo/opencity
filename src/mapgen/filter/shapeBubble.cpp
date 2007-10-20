/***************************************************************************
						shapeBubble.cpp  -  description
							-------------------
	begin                : feb 10th, 2007
	copyright            : (C) 2006 by Frédéric RODRIGO
	email                : f.rodrigo free.fr
	
	$Id: shapeBubble.cpp 124 2007-01-13 17:28:49Z neoneurone $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#include "shapeBubble.h"

#include <math.h>

namespace MapGen
{


   /*=====================================================================*/
ShapeBubble::ShapeBubble( const uint w, const uint h, const float base, const float rude, const uint l ):
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
ShapeBubble::~ShapeBubble()
{
	MAP_DEBUG( "dtor" );
}


   /*=====================================================================*/
/*
 * gnuplot> splot [-100:100] [-100:100] -exp(sqrt(x*x+y*y)/100*5)/exp(sqrt(2)*5)
 */
float ShapeBubble::value( const uint x, const uint y ) const
{
	int _x = x - _w2;
	int _y = y - _h2;

	return ( -exp(sqrt(_x*_x+_y*_y)/_c*_rude) / exp(sqrt(2)*_rude) + _base ) * _l;
}


}
