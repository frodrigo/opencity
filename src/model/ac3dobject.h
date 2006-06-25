/***************************************************************************
							AC3DObject.h  -  description
								-------------------
	begin                : mer juin 29 05
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

#ifndef _OPENCITY_AC3DOBJECT_H_
#define _OPENCITY_AC3DOBJECT_H_ 1

#include "ac3dsurface.h"

#include <vector>
#include <string>
#include <sstream>

namespace AC3D {

using std::vector;
using std::string;
using std::stringstream;

enum OBJECT_TYPE {
	OBJECT_WORLD,
	OBJECT_POLYGON,
	OBJECT_GROUP
};


struct Vertex {
	float x, y, z;
};


//========================================================================
/**
	\author Duong-Khang NGUYEN
*/
class AC3DObject{
public:
	AC3DObject();
	AC3DObject( stringstream& data );

	~AC3DObject();


//========================================================================
/** Add a new kid to this object
*/
	void
	AddKid( AC3DObject* kid );


//========================================================================
/** Decode the "name" to determine whether the object is translucent or not
	\return True if the object is translucent
*/
	bool
	IsTranslucent() const;


//========================================================================
/**
	\return The content of the name command
*/
	string
	GetName() const;


//========================================================================
/**
	\return The number of kids which may differ from the size of the vector
	returned by GetVPObject() during loading process.
*/
	unsigned int
	GetNumberKid() const;


//========================================================================
/**
	\return The file name of the texture
*/
	const string&
	GetTextureFile() const;


//========================================================================
/**
	\return The "location" translation vector
*/
	const float*
	GetLoc() const;


//========================================================================
/**
	\return The constant reference to the vector which contains all
	the vertex of the object
*/
    const vector<Vertex>&
	GetVVertex() const;


//========================================================================
/**
	\return The constant reference to the vector which contains all
	the pointers to the surface of the object
*/
    const vector<AC3DSurface*>&
	GetVPSurface() const;


//========================================================================
/**
	\return The constant reference to the vector which contains all
	the child objects
*/
    const vector<AC3DObject*>&
	GetVPObject() const;


//========================================================================
/** Parse and extract the data from the given stream
*/
	void Parse( stringstream& data );


	string ToStr() const;

private:
	OBJECT_TYPE type;				///< Object's type
	string	strName;				///< Content of the "name" command
	int		iData;					///< Not used
	string	strTexture;				///< Texture file name
	float	fTexRep[2];				///< Default (1, 1). The texture repeat values for the tiling of a texture on an object's surfaces.
	float	fRot[9];				///< Rotation matrix
	float	fLoc[3];				///< Translation vector
	float	fCrease;				///< Angle in degrees for calculating normals for smoothing
	string	strURL;					///< The URL of the file
	
	uint	uiNumVert;				///< Number of vertices in this object
	vector<Vertex> vVertex;			///< Table of vertices

	uint	uiNumSurf;				///< Number of surfaces of this object
	vector<AC3DSurface*> vpSurface;	///< Table of surfaces

	uint	uiKids;					///< Number of kids
	vector<AC3DObject*> vpObject;	///< Table of child objects


   /*=====================================================================*/
   /*                         PRIVATE     METHODS                         */
   /*=====================================================================*/
	void ac3dSetDefault();

	void
	ac3dobjectParseVertex(
		unsigned int nb,
		stringstream& data,
		char* buffer );

	void
	ac3dobjectParseSurface(
		unsigned int nb,
		stringstream& data,
		char* buffer );
};

}

#endif























