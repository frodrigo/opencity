/***************************************************************************
						star.cpp  -  description
							-------------------
	begin                : july 2nd, 2006
	copyright            : (C) 2006 by Frederic RODRIGO
	email                : f.rodrigo free.fr

	$Id: filter.h 20 2006-07-16 10:28:39Z neoneurone $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#include "star.h"

namespace Triangulation
{

Triangle*
Star::process( const uint nVertex, const Vertex* vertex ) const
{
	Triangle *triangle = new Triangle[nVertex-2];
	for ( uint i=1; i<nVertex-1; ++i )
	{
		triangle[i-1].a = vertex[0];
		triangle[i-1].b = vertex[i];
		triangle[i-1].c = vertex[i+1];
	}
	return triangle;
}

}
