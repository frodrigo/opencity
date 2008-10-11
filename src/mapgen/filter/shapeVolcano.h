/***************************************************************************
						shapeVolcano.h  -  description
							-------------------
	begin                : feb 10th, 2007
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

#ifndef _OPENCITY_SHAPEVOLCANO_H_
#define _OPENCITY_SHAPEVOLCANO_H_ 1

#include "shape.h"

namespace MapGen
{


//========================================================================
/** Apply bubble shape
*/
class ShapeVolcano: public Shape {
public:
	ShapeVolcano( const uint w, const uint h, const float base, const float rude, const uint l );
	virtual ~ShapeVolcano();

	float value( const uint x, const uint y ) const;

private:
	uint		_w2;
	uint		_h2;
	float		_cc;
	float		_base;
	float		_rude;
	uint		_l;

};


}

#endif
