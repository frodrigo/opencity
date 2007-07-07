/***************************************************************************
						flattern.cpp  -  description
							-------------------
	begin                : july 16th, 2007
	copyright            : (C) 2007 by Frédéric RODRIGO
	email                : f.rodrigo free.fr
	
	$Id: flattern.cpp 167 2007-02-10 15:58:22Z frodrigo $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#include "cutter.h"
#include "map.h"

#include <cmath>

namespace MapGen
{


bool CutterFunctor::operator()( const float value )
{
	// default behaviours, keep content
	return false;
}


   /*=====================================================================*/
Cutter::Cutter( CutterFunctor* cutterfunctor, const float defaultValue ):
_cutterfunctor(cutterfunctor),
_defaultValue(defaultValue)
{
	MAP_DEBUG( "ctor2" );
}


   /*=====================================================================*/
Cutter::~Cutter()
{
	MAP_DEBUG( "dtor" );
	delete _cutterfunctor;
}


   /*=====================================================================*/
void Cutter::apply( Map* map )
{
	uint w = map->getW();
	uint l = map->getL();

	for( uint x = 0; x < w; ++x )
		for( uint y = 0; y < l; ++y ) {
			if( (*_cutterfunctor)( map->getAt( x, y ) ) ) {
				map->setAt( x, y, _defaultValue );
			}
		}
}


   /*=====================================================================*/
CutterFunctorConstant::CutterFunctorConstant( const float value, const bool higher ):
_value(value),
_higher(higher)
{
}


   /*=====================================================================*/
bool CutterFunctorConstant::operator()( const float value )
{
	return _higher? value > _value : value < _value;
}


   /*=====================================================================*/
CutterFunctorOnlyPositive::CutterFunctorOnlyPositive():
CutterFunctorConstant( 0, false )
{
}


}
