/***************************************************************************
						gaussblur.h  -  description
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

#ifndef _OPENCITY_GAUSSBLUR_H_
#define _OPENCITY_GAUSSBLUR_H_ 1

#include "filter.h"

namespace mapgen
{

/** Apply a Guass blur on the map with arbitrary precision window size
*/
class GaussBlur: public Filter {
public:
	GaussBlur( const uint length );

	~GaussBlur();

	void apply( Map* map );

private:
	uint	_length;
	float	*_convulsion;

	static float _gauss(
			const uint x,
			const uint width );

	static float *_blur( const uint length );

};

}

#endif
