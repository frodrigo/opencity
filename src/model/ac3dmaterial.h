/***************************************************************************
                          AC3DMaterial.h  -  description
          $Id$
                             -------------------
    begin                : mer juin 29 05
    copyright            : (C) 2005 by Duong-Khang NGUYEN
    email                : neoneurone @ users sourceforge net
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

struct Color {
	Color( float r, float g, float b ):fR(r),fG(g),fB(b){}
	float fR, fG, fB;
};

/**
@author Duong-Khang NGUYEN
*/
class AC3DMaterial{
public:
	AC3DMaterial();
	AC3DMaterial( const string data );
	
	~AC3DMaterial();


//========================================================================
	string ToStr() const;		// debug method

	string strName;
	Color rgb;
	Color amb;
	Color emis;
	Color spec;
	float shi;
	float trans;
};

}

#endif





















