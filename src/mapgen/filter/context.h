/***************************************************************************
						context.h  -  description
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

#ifndef _OPENCITY_CONTEXT_H_
#define _OPENCITY_CONTEXT_H_ 1

#include "../map.h"


namespace MapGen
{


//========================================================================
/** Define generic context
*/
class Context {
public:
	Context();
	virtual ~Context();

	virtual float contextualize( const float height, const float context ) const = 0;

private:

};


}

#endif
