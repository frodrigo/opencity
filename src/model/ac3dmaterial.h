/***************************************************************************
						AC3DMaterial.h  -  description
							-------------------
	begin                : june 29, 05
	copyright            : (C) 2005-2006 by Duong-Khang NGUYEN
	email                : neoneurone @ users sourceforge net
	
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

#ifndef _OPENCITY_AC3DAC3DMATERIAL_H_
#define _OPENCITY_AC3DAC3DMATERIAL_H_ 1

#include <string>

using std::string;

namespace AC3D {


/** RGBA float color structure
*/
struct Color {
	Color( float r, float g, float b ):fR(r),fG(g),fB(b){}
	float fR, fG, fB;
};


//========================================================================
/** This class represents the material used by the AC3D models
	\author Duong-Khang NGUYEN
*/
class AC3DMaterial{
public:
	AC3DMaterial();
	AC3DMaterial( const string data );
	
	~AC3DMaterial();


//========================================================================
	string ToStr() const;		// debug method

	string strName;
	Color rgb;				///< The RGB value of the material
	Color amb;				///< The ambiance light value
	Color emis;				///< The emission light value
	Color spec;				///< The specular light value
	float shi;				///< ?
	float trans;			///< The transparence value
};

}

#endif





















