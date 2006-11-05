/***************************************************************************
						geometry.h  -  description
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

#ifndef _OPENCITY_GEOMETRY_H_
#define _OPENCITY_GEOMETRY_H_ 1

namespace Triangulation
{

struct TexCoord
{
	float fS, fT;
};

struct Vertex
{
	float x, y, z;
	TexCoord texCoord;
};

struct Triangle
{
	Vertex a, b, c;
};

}

#endif
