/***************************************************************************
						triangulation.h  -  description
							-------------------
	begin                : july 30th, 2006
	copyright            : (C) 2006 by Fredreic RODRIGO
	email                : f.rodrigo free.fr

	$Id: map.h 20 2006-07-16 10:28:39Z neoneurone $
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

namespace triangulation
{

/** Triangulation abstract class
*/
class Triangulation {
public:

	Triangulation() {};
	virtual ~Triangulation() {};

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
