/***************************************************************************
                          AC3DSurface.h  -  description
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

#ifndef _OPENCITY_AC3DAC3DSURFACE_H_
#define _OPENCITY_AC3DAC3DSURFACE_H_ 1

#include <vector>
#include <string>
#include <sstream>

using std::vector;
using std::string;
using std::stringstream;


namespace AC3D {

enum SURFACE_TYPE {
	SURFACE_POLYGON,
	SURFACE_CLOSEDLINE,
	SURFACE_LINE
};

struct Ref {
	unsigned int uiVertIndex;
	float fTexS;
	float fTexT;
};

/**
@author Duong-Khang NGUYEN
*/
class AC3DSurface{
public:
    AC3DSurface();
    AC3DSurface( stringstream& data );

    ~AC3DSurface();


//========================================================================
/**
	\return The index of the used material
*/
	const unsigned int
	GetMat() const;


//========================================================================
/**
	\return The constant reference to the vector which contains all
	the references of the surface
*/
	const vector<Ref>&
	GetVRef() const;


	void Parse( stringstream& data );

	string ToStr() const;


private:
	unsigned int uiAttribute;
	SURFACE_TYPE type;
	bool bShaded;
	bool bTwoSide;
	unsigned int uiMat;			///< Index of the material used
	unsigned int uiNumRef;		///< Number of references
	vector<Ref> vRef;			///< Table of references
};

}

#endif

























