/***************************************************************************
						map.h  -  description
							-------------------
	begin                : july 2nd, 2006
	copyright            : (C) 2006 by Frédéric RODRIGO
	email                : f.rodrigo free.fr
	
	$Id: CodingStyle.h 5 2006-06-17 23:53:07Z neoneurone $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#ifndef _OPENCITY_MAP_H_
#define _OPENCITY_MAP_H_ 1

#define MAP_NDEBUG 1			// Debugging off

#include "main.h"				///< OpenCity specifics

#ifndef MAP_NDEBUG
	#define MAP_DEBUG( msg ) OPENCITY_DEBUG( msg )
#else
	#define MAP_DEBUG( msg )
#endif


/**
	Float height map
*/
class Map {
public:
	Map(
		const uint w,
		const uint h );

	~Map();

	/* module setter */
	void setAt(
		int x,
		int y,
		float value );

	/* modulo getter */
	float getAt(
		int x,
		int y ) const;

	bool save( const string &file );

	inline uint getW() const { return _w; }

	inline uint getH() const { return _h; }

	/* Return a sub-map */
	Map* crop(
		const uint w,
		const uint h ) const;

	/* Convert the private float array to an allocated int array */
	int **toIntArray() const;

private:
	uint	_w;
	uint	_h;
	float	**_map;

};

#endif
