/***************************************************************************
                          modelloader.h  -  description
    $Id$
                             -------------------
    begin                : sam mai 22 2004
    copyright            : (C) by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_MODELLOADER_H_
#define _OPENCITY_MODELLOADER_H_ 1

#include "main.h"

#include "ac3dmaterial.h"
#include "ac3dobject.h"

#include <map>				// Used for texture loading cache

using std::map;

class Model;

//========================================================================
/** The purpose of this class is simple: read the given file, then return
	an object of type Model.
*/

class ModelLoader {
public:

//========================================================================
/** Detect the file type according to its extension. Currently,
	it checks for ".ocm" and ".ac"
*/
	static Model* const
	Load(
		const string & rcsFileName );


//========================================================================
/** OCM file loader
*/
	static Model* const
	LoadOCM(
		const string & rcsFileName );


//========================================================================
/** AC3D file loader
*/
	static Model* const
	LoadAC3D(
		const string & rcsFileName );


private:
/* TOKILL, no used any more
This is a helper class for AC3D file support
*/
/*
	class ac3dObject {
	public:
		ac3dObject():type(""),name(""),t1(.0),t2(.0),t3(.0),kids(0){};

		string type;                 // Object's type (world / group / poly ?)
		string name;                 // Object's name (name command)
		OC_FLOAT t1, t2, t3;    // Object's translation vector (loc command)
		uint kids;                   // Object's number of kids (kids command)
	};
*/


	static void
	modelloaderAC3DTextureToGL(
		const string& strPath,
		const AC3D::AC3DObject* const pObject,
		map<string, GLuint>& mapTexture
	);


	static void
	modelloaderAC3DVertexToGL(
		const string& strPath,
		const vector<AC3D::AC3DMaterial>& vMaterial,
		const AC3D::AC3DObject* const pObject
	);
};
#endif



































