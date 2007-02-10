/***************************************************************************
						choper.h  -  description
							-------------------
	begin                : fev 10th, 2007
	copyright            : (C) 2006 by Frédéric RODRIGO
	email                : f.rodrigo free.fr
	
	$Id: choper.h 124 2007-01-13 17:28:49Z neoneurone $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#ifndef _OPENCITY_CHOPER_H_
#define _OPENCITY_CHOPER_H_ 1

#include "filter.h"

#include "shape.h"

namespace MapGen
{


//========================================================================
/** Generic choper apply
*/
class Choper: public Filter {
public:
	Choper( const Shape *shape );
	virtual ~Choper();

	void apply( Map* map );

private:
	const Shape *_shape;

};


}

#endif
