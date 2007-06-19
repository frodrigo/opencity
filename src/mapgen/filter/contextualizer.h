/***************************************************************************
						contextualizer.h  -  description
							-------------------
	begin                : jun 19th, 2007
	copyright            : (C) 2007 by Frédéric RODRIGO
	email                : f.rodrigo free.fr
	
	$Id: shaper.h 124 2007-01-13 17:28:49Z neoneurone $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#ifndef _OPENCITY_CONTEXTUALIZER_H_
#define _OPENCITY_CONTEXTUALIZER_H_ 1

#include "filter.h"

#include "context.h"
#include "../map.h"

namespace MapGen
{


//========================================================================
/** Generic contextualizer
*/
class Contextualizer: public Filter {
public:
	Contextualizer( const Context* context, const Map* contextMap );
	virtual ~Contextualizer();

	void apply( Map* map );

private:
	const Context* _context;
	const Map* _contextMap;

};


}

#endif
