/***************************************************************************
						normalize.h  -  description
							-------------------
	begin                : july 2nd, 2006
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

#ifndef _OPENCITY_NORMALIZE_H_
#define _OPENCITY_NORMALIZE_H_ 1

#include "filter.h"

namespace MapGen
{


//========================================================================
/** Linear re-range of the map
*/
class Normalize: public Filter {
public:
	Normalize(
		const float min,
		const float max );
	~Normalize();

	void apply( Map* map );

private:
	float	_min;
	float	_max;

};


}

#endif
