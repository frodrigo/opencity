/***************************************************************************
						AC3DObject.h  -  description
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

/** The different AC3D object type
*/
enum OBJECT_TYPE {
	OBJECT_WORLD,			///< World object
	OBJECT_POLYGON,			///< Polygon object
	OBJECT_GROUP			///< Group object
};

/** A vertex structure which is composed of 3 space coordinates
*/
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
/** Add a new kid to this object. It automatically set the translucent
attribute of the children to true if the parent is a translucent group.
*/
	void
	AddKid( AC3DObject* kid );


//========================================================================
/** Decode the "name" attribute to determine whether the object is
translucent or not. This is specific to OpenCity
	\return True if the object is translucent
*/
	bool
	IsTranslucent() const;


//========================================================================
/** Set the translucent attribute of the object to true regarless its
encoded value in the "name" attribute.
*/
	void
	SetTranslucent(bool bValue);


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
	string	_sData;					///< The optional data is stocked as std::string
	string	strTexture;				///< Texture file name
	float	fTexRep[2];				///< Default (1, 1). The texture repeat values for the tiling of a texture on an object's surfaces.
	float	fRot[9];				///< Rotation matrix
	float	fLoc[3];				///< Translation vector
	float	fCrease;				///< Angle in degrees for calculating normals for smoothing
	string	strURL;					///< The URL of the file

	bool	_bTranslucent;			///< True if the object is translucent, false otherwise

	uint	uiNumVert;				///< Number of vertices in this object
	vector<Vertex> vVertex;			///< Table of vertices

	uint	uiNumSurf;				///< Number of surfaces of this object
	vector<AC3DSurface*> vpSurface;	///< Table of surfaces

	uint	uiKids;					///< Number of kids
	vector<AC3DObject*> vpObject;	///< Table of child objects


//========================================================================
// Private methods
//========================================================================
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























