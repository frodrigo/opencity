/***************************************************************************
						contextOnlyPositive.cpp  -  description
							-------------------
	begin                : jun 19th, 2007
	copyright            : (C) 2007 by Frédéric RODRIGO
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
	return context > 0 ? height : 0;
}

}
