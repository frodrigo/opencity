/***************************************************************************
						contextOnlyPositive.cpp  -  description
							-------------------
	begin                : jun 19th, 2007
	copyright            : (C) 2007 by Frédéric RODRIGO
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

#include "contextOnlyPositive.h"

namespace MapGen
{


   /*=====================================================================*/
ContextOnlyPositive::ContextOnlyPositive()
{
	MAP_DEBUG( "ctor" );
}


   /*=====================================================================*/
ContextOnlyPositive::~ContextOnlyPositive()
{
	MAP_DEBUG( "dtor" );
}


   /*=====================================================================*/
float ContextOnlyPositive::contextualize( const float height, const float context ) const
{
	// Cmp context on 0 must be on int(context), because all map are finaly in int
	// int(-0.99)=0
	return int(context) >= 0 ? height : 0;
	// Can also be written as
	// return context > -1 ? height : 0;
}

}
