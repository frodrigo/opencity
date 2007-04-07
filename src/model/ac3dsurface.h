/***************************************************************************
						AC3DSurface.h  -  description
							-------------------
	begin                : june 29th, 05
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

#ifndef _OPENCITY_AC3DAC3DSURFACE_H_
#define _OPENCITY_AC3DAC3DSURFACE_H_ 1

#include <vector>
#include <string>
#include <sstream>

#ifndef uint
	typedef unsigned int uint;
#endif

namespace AC3D {

using std::vector;
using std::string;
using std::stringstream;

/** The type of the AC3D surface. OpenCity only handles the polygon type
at the moment
*/
enum SURFACE_TYPE {
	SURFACE_POLYGON,			///< Polygon surface type
	SURFACE_CLOSEDLINE,			///< Closed line surface type
	SURFACE_LINE				///< Line type
};

/** A reference to a vertex
*/
struct Ref {
	unsigned int uiVertIndex;	///< The index of the referenced vertex
	float fTexS;				///< The vertex 2D S texture coordinate
	float fTexT;				///< The vertex 2D T texture coordinate
};


//========================================================================
/** A surface is composed of vertices
	\author Duong-Khang NGUYEN
*/
class AC3DSurface{
public:
    AC3DSurface();
    AC3DSurface( stringstream& data );

    ~AC3DSurface();


//========================================================================
/** Tell whether the surface is twosided
	\return True if the surface is twosided
*/
	bool IsTwoSide() const;

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
	uint	uiAttribute;
	SURFACE_TYPE type;
	bool	bShaded;
	bool	bTwoSide;
	uint	uiMat;				///< Index of the material used
	uint	uiNumRef;			///< Number of references
	vector<Ref> vRef;			///< Table of references
};

}

#endif

























