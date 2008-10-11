/***************************************************************************
						triangulation.h  -  description
							-------------------
	begin                : july 30th, 2006
	copyright            : (C) 2007 by Fredreic RODRIGO
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

#ifndef _OPENCITY_TRIANGULATION_H_
#define _OPENCITY_TRIANGULATION_H_ 1

#include "geometry.h"

#ifndef uint
typedef unsigned int uint;
#endif

namespace Triangulation
{

/** Triangulation abstract class
*/
class Triangulation {
public:

	Triangulation() {}
	virtual ~Triangulation() {}

//========================================================================
/** Compute triangulation on vertex, return a Triangle[nVertex-2]
*/
	virtual Triangle*
	process(
		const uint nVertex,
		const Vertex* vertex ) const = 0;

};

}

#endif
