/***************************************************************************
						shapeVolcano.cpp  -  description
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

#include "shapeVolcano.h"

#include <math.h>

namespace MapGen
{


   /*=====================================================================*/
ShapeVolcano::ShapeVolcano( const uint w, const uint h, const float base, const float rude, const uint l ):
Shape(w,h),
_base(base),
_rude(rude),
_l(l)
{
	MAP_DEBUG( "ctor" );
	_w2 = w/2;
	_h2 = h/2;
	uint _c = _w2 > _h2 ? _w2 : _h2;
	_cc = _c*_c * 4./100;
}


   /*=====================================================================*/
ShapeVolcano::~ShapeVolcano()
{
	MAP_DEBUG( "dtor" );
}


   /*=====================================================================*/
/*
 * gnuplot> splot [-4:4] [-4:4] (x**2 + y**2)*exp(-x**2 - y**2)
 */
float ShapeVolcano::value( const uint x, const uint y ) const
{
	int _x = x - _w2;
	int _y = y - _h2;

	float t = ( _x*_x + _y*_y ) / _cc * _rude;
	return ( t*exp(-t) + _base ) * 2.6 * _l;
}


}
