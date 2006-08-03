/***************************************************************************
						lazy.h  -  description
							-------------------
	begin                : july 30th, 2006
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

#ifndef _OPENCITY_STAR_H_
#define _OPENCITY_STAR_H_ 1

#include "triangulation.h"

#include <iostream>

namespace triangulation
{

/** Bad algorithm. We draw all the triangle with the first vertex unvariable
*/
class Star : public Triangulation {

public:
	Star() { std::cout << "ctor" << std::endl; }
	virtual ~Star() { std::cout << "dtor" << std::endl; }

	virtual Triangle*
	process(
		const uint nVertex,
		const Vertex* vertex ) const;

};

}

#endif
