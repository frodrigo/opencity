/***************************************************************************
                          modelloader.cpp  -  description
      $Id: modelloader.cpp,v 1.14 2006/06/05 09:58:06 neoneurone Exp $
                             -------------------
    begin                : sam mai 22 2004
    copyright            : (C) 2004 by Duong-Khang NGUYEN
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

#include "modelloader.h"

#include "ocm.h"
#include "model.h"

#include "ac3dmodel.h"		// For AC3D structure manipulation

#include "texture.h"		// for texture manipulation

#include <vector>
#include <fstream>
#include <cstring>

#define OC_OCM_MAX_LINE_LENGTH	80
#define OC_AC3D_MAX_LINE_LENGTH	1024

using std::vector;
using std::ifstream;

using namespace AC3D;


// Accumulate the locations command
	float locAccu[3];

// Debug variables
//	unsigned int nbPoly;
//	unsigned int nbVertex;


   /*=====================================================================*/
Model* const
ModelLoader::Load(
	const string & rcsFileName )
{
	OPENCITY_DEBUG( rcsFileName.c_str() );

// IF it's an OCM file THEN
	if (rcsFileName.rfind(".ocm") != rcsFileName.npos)
		return ModelLoader::LoadOCM(rcsFileName);
	else
// IF it's an AC3D file THEN
	if (rcsFileName.rfind(".ac") != rcsFileName.npos)
		return ModelLoader::LoadAC3D(rcsFileName);

// otherwise, return NULL
	assert(0);
	return NULL;
}


   /*=====================================================================*/
Model* const
ModelLoader::LoadOCM(
	const string & rcsFileName )
{
	vector<GLfloat> vf;
	vector<GLuint> vui;
	GLfloat fValue;
	OC_CHAR tempStr [OC_OCM_MAX_LINE_LENGTH];
// TOKILL	OC_CHAR tempPath [OC_OCM_MAX_LINE_LENGTH];
	OC_CHAR* endStr;

// model's parameter
	GLfloat* ftab;

// open the stream for reading
	ifstream ocmFile( rcsFileName.c_str() );
	if (ocmFile == NULL) {
		OPENCITY_DEBUG( "can not load file" );
		cerr << "Error loading model: " << rcsFileName << endl;
		assert(ocmFile != NULL);
		return NULL;
	}

// read the first line
	ocmFile.getline( tempStr, OC_OCM_MAX_LINE_LENGTH );

// while not EOF do processing
	while (!ocmFile.eof()) {
//debug printf("tempstr: '%s' \n", tempStr );
	// if not NULL nor commented out then
		if ((strlen(tempStr) != 0)
		&&  (tempStr[0] != '#')) {
			fValue = strtof( tempStr, &endStr );

		// is there any conversion error ?
			if ( tempStr == endStr ) {
				OPENCITY_DEBUG( "can not load file : " );
				ocmFile.close();
				return NULL;
			}

		// WARNING no error checking below here, tired of this :)
		// store the OCM value type code
			if ( fValue != OC_OCM_TEXFILE )
				vf.push_back( fValue );

		// read the primitive's values
			ocmFile.getline( tempStr, OC_OCM_MAX_LINE_LENGTH );

			if (fValue
			== OC_OCM_VERTEX) {
			// read 3 floats
				fValue = strtof( tempStr, &endStr );
				vf.push_back( fValue );
				fValue = strtof( endStr, &endStr );
				vf.push_back( fValue );
				fValue = strtof( endStr, &endStr );
				vf.push_back( fValue );
			} else
			if (fValue
			== OC_OCM_COLOR) {
			// read 4 floats
				fValue = strtof( tempStr, &endStr );
				vf.push_back( fValue );
				fValue = strtof( endStr, &endStr );
				vf.push_back( fValue );
				fValue = strtof( endStr, &endStr );
				vf.push_back( fValue );
				fValue = strtof( endStr, &endStr );
				vf.push_back( fValue );
			} else
			if (fValue
			== OC_OCM_TEXCOOR) {
			// read 3 floats
				fValue = strtof( tempStr, &endStr );
				vf.push_back( fValue );
				fValue = strtof( endStr, &endStr );
				vf.push_back( fValue );
				fValue = strtof( endStr, &endStr );
				vf.push_back( fValue );
			} else
			if (fValue
			== OC_OCM_TEXFILE) {
				OPENCITY_DEBUG("Loading texture file");
				vui.push_back( Texture::Load( ocHomeDirPrefix( tempStr )));
//debug cout << "Texture id got: " << vui.back() << endl;
			} else
			if (fValue
			== OC_OCM_TEXBIND) {
			// read 1 float
				fValue = strtof( tempStr, &endStr );
			// WARNING: no error checking
			// it's quite dangerous
			// 0 is a default texture in OpenGL
			// however in OCM it's -1
				if ( fValue != -1 )
					vf.push_back( vui[(GLuint)fValue] );
				else
					vf.push_back( .0 );
			}
			else {
				OPENCITY_DEBUG( "can not understand model file" );
				assert(0);
			}

		} // if not commented out then
	// read the next line
		ocmFile.getline( tempStr, OC_OCM_MAX_LINE_LENGTH );
	}

//debug cout << "vector size: " << vf.size() << endl;

// convert vector<GLfloat> to GLfloat bla []
	ftab = new GLfloat [vf.size()];
	for (uint i = 0; i < vf.size(); i++) {
		ftab[i] = vf[i];
//debug cout << ftab[i] << " / ";
	}

	ocmFile.close();
	return new Model( ftab, vf.size() );
}


   /*=====================================================================*/
Model* const
ModelLoader::LoadAC3D(
	const string & rcsFileName )
{
	AC3DModel ac3dmodel = AC3DModel( rcsFileName );
	vector<AC3DMaterial> vMaterial;
	map<string, GLuint> mapTexture;
	GLuint list = 0;
	string strPath = "";

	if (!ac3dmodel.IsGood())
		return NULL;

// Don't go further if there is no object to parse
	const AC3DObject* const pObject = ac3dmodel.GetPObject();
	if (pObject == NULL)
		return NULL;

// Get the path
	if (rcsFileName.rfind( '/' ) != rcsFileName.npos ) {
		strPath = rcsFileName.substr( 0, rcsFileName.rfind('/') );
	}
	else {
		strPath = ".";
	}
//debug	cout << "path: " << strPath << endl;

	vMaterial = ac3dmodel.GetVMaterial();
	list = glGenLists( 1 );

// Initialize the model view matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

// Initialize the "loc" command accumulation variable
	locAccu[0] = .0;
	locAccu[1] = .0;
	locAccu[2] = .0;

// Debug: count the number of polys and vertex
//	nbPoly = 0;
//	nbVertex = 0;

// Load all the texture used by the model
	modelloaderAC3DTextureToGL( strPath, pObject, mapTexture );

// Debug
	if (mapTexture.size() > 1) {
		OPENCITY_DEBUG("WARNING: more than 1 texture used by the model");
	}

// Recursively load all the objects into the display list
	glNewList( list, GL_COMPILE );
// Save the all enabled GL bits
	glPushAttrib( GL_ENABLE_BIT );
// Enable the texture target and bind the _first_ texture only
	if (mapTexture.size() > 0) {
		glEnable( GL_TEXTURE_2D );
//		glEnable( GL_BLEND );
	// Already activated in render.cpp
//		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );	
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
//		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );
		GLfloat env_color [] = { 0, 0, 0, 0 };
		GLfloat obj_color [] = { 01, 01, 01, 01 };
		glTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, env_color );
		glColor4fv( obj_color );
		glBindTexture( GL_TEXTURE_2D, (mapTexture.begin())->second);
	}
	else {
//		glDisable( GL_BLEND );
		glDisable( GL_TEXTURE_2D );
	}

// Load all the vertex
	glBegin( GL_TRIANGLES );
	modelloaderAC3DVertexToGL( strPath, vMaterial, pObject );
	glEnd();

// Restore all enabled bits
	glPopAttrib();
	glEndList();

// Debug: print out the number of polys
//	cout << "Number of polygons: " << nbPoly
//		 << " / vertex: " << nbVertex << endl;

	return new Model( list, mapTexture );
}


   /*=====================================================================*/
   /*                        PRIVATE      METHODS                         */
   /*=====================================================================*/
/* POLYGON version
void
ModelLoader::modelloaderAC3DToGL(
	const string& strPath,
	const vector<AC3DMaterial>& vMaterial,
	const AC3DObject* const pObject,
	map<string, GLuint>& mapTexture
	)
{
	const float* loc;
	AC3DMaterial mat;
	vector<Vertex> vVertex;

	vector<AC3DSurface*>::size_type pos, size;
	vector<AC3DSurface*> vpSurface;

	vector<AC3DObject*>::size_type posObj, sizeObj;
	vector<AC3DObject*> vpObj;

	vector<Ref>::size_type posRef, sizeRef;
	vector<Ref> vRef;
	Ref ref;

	GLuint tex = 0;
	bool texEnabled = false;


	assert( pObject != NULL );

// Set the texture
	if ( pObject->GetTextureFile() != "" ) {
		texEnabled = true;

	// IF the specified texture has not been loaded already THEN
		if (mapTexture.find( pObject->GetTextureFile() ) == mapTexture.end() ) {
		// Load the texture as specified by the texture command
			tex = Texture::Load( strPath + "/" + pObject->GetTextureFile() );
			mapTexture[ pObject->GetTextureFile() ] = tex;
		}
		else {
		// Assign the cached texture to the model
			OPENCITY_DEBUG("Texture loading cache hit");
			tex = mapTexture[ pObject->GetTextureFile() ];
		}

		glEnable( GL_TEXTURE_2D );
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
		glBindTexture( GL_TEXTURE_2D, tex);
	}

	loc = pObject->GetLoc();
	locAccu[0] += loc[0];
	locAccu[1] += loc[1];
	locAccu[2] += loc[2];

	vVertex = pObject->GetVVertex();
	vpSurface = pObject->GetVPSurface();

	size = vpSurface.size();
	for (pos = 0; pos < size; pos++) {
	// Debug
		++nbPoly;
	// Get the material of the current surface
		mat = vMaterial[ vpSurface[pos]->GetMat() ];
	// Set the color of the surface with COLOR_MATERIAL enabled
		glColor3f( mat.rgb.fR, mat.rgb.fG, mat.rgb.fB );

		vRef = vpSurface[pos]->GetVRef();
		sizeRef = vRef.size();
	// Debug
		cout << "Vertex per surface: " << sizeRef << endl;

		glBegin( GL_POLYGON );
		for (posRef = 0; posRef < sizeRef; posRef++) {
		// Debug
			++nbVertex;

			ref = vRef[posRef];
			if (texEnabled)
				glTexCoord2f( ref.fTexS, ref.fTexT );
			glVertex3f(
				vVertex[ref.uiVertIndex].x + locAccu[0],
				vVertex[ref.uiVertIndex].y + locAccu[1],
				vVertex[ref.uiVertIndex].z + locAccu[2]
			);
		}
		glEnd();
	} // For each surface

	if (texEnabled) {
		texEnabled = false;			// Safe
		glDisable( GL_TEXTURE_2D );
	}

// Parse all the child objects
	vpObj = pObject->GetVPObject();
	sizeObj = vpObj.size();
	for (posObj = 0; posObj < sizeObj; posObj++) {
		modelloaderAC3DToGL( strPath, vMaterial, vpObj[posObj], mapTexture );
	}

	locAccu[0] -= loc[0];
	locAccu[1] -= loc[1];
	locAccu[2] -= loc[2];
}
*/


   /*=====================================================================*/
void
ModelLoader::modelloaderAC3DTextureToGL
(
	const string& strPath,
	const AC3DObject* const pObject,
	map<string, GLuint>& mapTexture
)
{
	vector<AC3DObject*>::size_type posObj, sizeObj;
	vector<AC3DObject*> vpObj;

	GLuint tex = 0;


	assert( pObject != NULL );

// Get the texture
	if ( pObject->GetTextureFile() != "" ) {
	// IF the specified texture has not been loaded already THEN
		if (mapTexture.find( pObject->GetTextureFile() ) == mapTexture.end() ) {
		// Load the texture as specified by the texture command
			tex = Texture::Load( strPath + "/" + pObject->GetTextureFile() );
			mapTexture[ pObject->GetTextureFile() ] = tex;
		}
		else {
		// Assign the cached texture to the model
		//	OPENCITY_DEBUG("Texture loading cache hit");
			tex = mapTexture[ pObject->GetTextureFile() ];
		}
	}

// Parse all the child objects and retrieve the texture
	vpObj = pObject->GetVPObject();
	sizeObj = vpObj.size();
	for (posObj = 0; posObj < sizeObj; posObj++) {
		modelloaderAC3DTextureToGL( strPath, vpObj[posObj], mapTexture );
	}
}


   /*=====================================================================*/
// TRIANGLES version
void
ModelLoader::modelloaderAC3DVertexToGL
(
	const string& strPath,
	const vector<AC3DMaterial>& vMaterial,
	const AC3DObject* const pObject
)
{
	const float* loc;
	AC3DMaterial mat;
	vector<Vertex> vVertex;

	vector<AC3DSurface*>::size_type pos, size;
	vector<AC3DSurface*> vpSurface;

	vector<AC3DObject*>::size_type posObj, sizeObj;
	vector<AC3DObject*> vpObj;

	vector<Ref>::size_type posRef, sizeRef;
	vector<Ref> vRef;
	Ref ref;



	assert( pObject != NULL );

// debug, location calculation
//cout << "in  : " << locAccu[0] << "/" << locAccu[1] << "/" << locAccu[2] << endl;

	loc = pObject->GetLoc();
	locAccu[0] += loc[0];
	locAccu[1] += loc[1];
	locAccu[2] += loc[2];

	vVertex = pObject->GetVVertex();
	vpSurface = pObject->GetVPSurface();

	size = vpSurface.size();
	for (pos = 0; pos < size; pos++) {
// Debug ++nbPoly;
	// Get the material of the current surface
		mat = vMaterial[ vpSurface[pos]->GetMat() ];
	// Set the color of the surface with COLOR_MATERIAL enabled
		glColor3f( mat.rgb.fR, mat.rgb.fG, mat.rgb.fB );
//		glColor4f( 1, 1, 1, 1 );

		vRef = vpSurface[pos]->GetVRef();
		sizeRef = vRef.size();

// Debug cout << "Vertex per surface: " << sizeRef << endl;

		assert( sizeRef >= 3 );		// We need at least one triangle
// Debug nbVertex += sizeRef;
		for (posRef = 1; posRef < sizeRef-1; posRef++) {
			ref = vRef[0];
			glTexCoord2f( ref.fTexS, ref.fTexT );
			glVertex3f(
				vVertex[ref.uiVertIndex].x + locAccu[0],
				vVertex[ref.uiVertIndex].y + locAccu[1],
				vVertex[ref.uiVertIndex].z + locAccu[2]
			);

			ref = vRef[posRef];
			glTexCoord2f( ref.fTexS, ref.fTexT );
			glVertex3f(
				vVertex[ref.uiVertIndex].x + locAccu[0],
				vVertex[ref.uiVertIndex].y + locAccu[1],
				vVertex[ref.uiVertIndex].z + locAccu[2]
			);

			ref = vRef[posRef+1];
			glTexCoord2f( ref.fTexS, ref.fTexT );
			glVertex3f(
				vVertex[ref.uiVertIndex].x + locAccu[0],
				vVertex[ref.uiVertIndex].y + locAccu[1],
				vVertex[ref.uiVertIndex].z + locAccu[2]
			);
		}
	} // For each surface

// Parse all the child objects
	vpObj = pObject->GetVPObject();
	sizeObj = vpObj.size();
//debug
//cout << "kids: " << pObject->GetNumberKid() << " / objects: " << sizeObj << endl;
	for (posObj = 0; posObj < sizeObj; posObj++) {
		modelloaderAC3DVertexToGL( strPath, vMaterial, vpObj[posObj] );
	}

	locAccu[0] -= loc[0];
	locAccu[1] -= loc[1];
	locAccu[2] -= loc[2];

// debug, location calculation
//cout << "out : " << locAccu[0] << "/" << locAccu[1] << "/" << locAccu[2] << endl;
}


























