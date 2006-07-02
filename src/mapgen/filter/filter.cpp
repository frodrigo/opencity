/***************************************************************************
						filter.cpp  -  description
							-------------------
	begin                : july 2nd, 2006
	copyright            : (C) 2006 by Fr�d�ric RODRIGO
	email                :
	
	$Id: CodingStyle.h 5 2006-06-17 23:53:07Z neoneurone $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#include "filter.h"


   /*=====================================================================*/
Filter::Filter()
{
	MAP_DEBUG( "ctor" );
}


   /*=====================================================================*/
Filter::~Filter()
{
	MAP_DEBUG( "dtor" );
}


   /*=====================================================================*/
void Filter::apply( Map *map )
{
}


   /*=====================================================================*/
void Filter::_getMinMax(
	const Map* map,
	float* min,
	float* max )
{
	*min = *max = map->getAt( 0, 0 );
	
	for( int x=0 ; x<map->getW() ; ++x )
		for( int y=0 ; y<map->getH() ; ++y )
		{
			float val = map->getAt( x, y );
			if( val < *min )
				*min = val;
			if( val > *max )
				*max = val;
		}
}
