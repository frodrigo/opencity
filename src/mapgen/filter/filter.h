/***************************************************************************
						filter.h  -  description
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

#ifndef _OPENCITY_FILTER_H_
#define _OPENCITY_FILTER_H_ 1

#include "../map.h"			// ../ because of silly DevCpp

namespace MapGen
{


//========================================================================
/** Filter abstract class/interface
*/
class Filter {
public:
	Filter();
	virtual ~Filter();


//========================================================================
/** Apply filter on map in place
*/
	virtual void apply( Map* map );


protected:

//========================================================================
/** Extract the min and the max values out of the map
*/
	void _getMinMax(
		const Map* map,
		float& min,
		float& max );
};


}

#endif
