/***************************************************************************
						modelloader.h  -  description
							-------------------
	begin                : sam mai 22 2004
	copyright            : (C) 2004-2006 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_MODELLOADER_H_
#define _OPENCITY_MODELLOADER_H_ 1

#include "main.h"

#include "ac3dobject.h"			// Vertex struct
#include "ac3dmaterial.h"

#include <vector>

using std::vector;

using AC3D::Vertex;

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


//========================================================================
/** Calculate the result vector of OA ^ OB
*/
	static Vertex
	GetNormal(
		Vertex & vO,
		Vertex & vA,
		Vertex & vB );


private:
	static void
	_AC3DTextureToGL(
		const string& strPath,
		const AC3D::AC3DObject* const pObject,
		GLuint& tex
	);


	static void
	_AC3DVertexToGL(
		const string& strPath,
		const vector<AC3D::AC3DMaterial>& vMaterial,
		const AC3D::AC3DObject* const pObject,
		const bool bProcessTranslucent = false,
		const bool bProcessTwoSide = false
	);
};
#endif



































