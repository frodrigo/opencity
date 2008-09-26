/***************************************************************************
						modelloader.cpp  -  description
							-------------------
	begin                : may 22th, 2004
	copyright            : (C) 2004-2008 by Duong-Khang NGUYEN
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

#include "modelloader.h"
#include "model.h"
#include "ac3dmodel.h"			// For AC3D structure manipulation
#include "ac3dobject.h"			// For normal calculation
#include "texture.h"			// for texture manipulation
#include "star.h"				// Triangulation algorithms

#include <vector>
#include <fstream>
#include <cstring>

#define OC_AC3D_MAX_LINE_LENGTH	1024

using std::vector;
using std::ifstream;

using namespace AC3D;


// Local module static variables
static float locAccu[3];		///< Accumulate the locations command
static bool bNeedAlpha;			///< Alpha processing state

// Debug variables
//	unsigned int nbPoly;
//	unsigned int nbVertex;


   /*=====================================================================*/
Model* const
ModelLoader::Load(
	const string & rcsFileName )
{
	OPENCITY_DEBUG( rcsFileName.c_str() );

// File extension checking
	if (rcsFileName.rfind(".ac") != rcsFileName.npos) {
		return ModelLoader::LoadAC3D(rcsFileName);
	}
	else {
		OPENCITY_ERROR( "Unable to load model file: " << rcsFileName );
		return NULL;
	}

// Otherwise, return NULL
	assert(0);
	return NULL;
}


   /*=====================================================================*/
Model* const
ModelLoader::LoadAC3D( const string & rcsFileName )
{
	AC3DModel ac3dmodel( rcsFileName );
	vector<AC3DMaterial> vMaterial;
	GLuint list = 0, listTwoSide = 0, listAlpha = 0;
	string strPath = "", strFile = "";

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

// Initialize the model view matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

// Initialize the "loc" command accumulation variable
	locAccu[0] = .0;
	locAccu[1] = .0;
	locAccu[2] = .0;

// Initialize the alpha state
	bNeedAlpha = false;

// Debug: count the number of polys and vertex
//	nbPoly = 0;
//	nbVertex = 0;

// Load all the texture used by the model
	Texture oModelTexture;
	_AC3DTextureToGL( pObject, strFile );
	if (strFile != "") {
		oModelTexture = Texture( strPath + "/" + strFile );
	}
	GLuint uiName = oModelTexture.GetName();


   /*=====================================================================*/
// Recursively load all the objects into the _opaque_ one side display list
	list = glGenLists( 1 );
	glNewList( list, GL_COMPILE );
	glPushAttrib( GL_ENABLE_BIT );
	glEnable( GL_CULL_FACE );

// Enable the texture target and bind the _first_ texture only
	if ( glIsTexture(uiName) == GL_TRUE ) {
		glEnable( GL_TEXTURE_2D );
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		glBindTexture( GL_TEXTURE_2D, uiName );
	}
	else {
		glDisable( GL_TEXTURE_2D );
	}

// Load all the vertex
	glBegin( GL_TRIANGLES );
	_AC3DVertexToGL( strPath, vMaterial, pObject, false, false );
	glEnd();

// Restore OpenGL attributes
	glPopAttrib();
	glEndList();


   /*=====================================================================*/
// Recursively load all the objects into the _opaque_ two side display list
	listTwoSide = glGenLists( 1 );
	glNewList( listTwoSide, GL_COMPILE );
	glPushAttrib( GL_ENABLE_BIT );

// Enable the texture target and bind the _first_ texture only
	if ( glIsTexture(uiName) == GL_TRUE ) {
		glEnable( GL_TEXTURE_2D );
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		glBindTexture( GL_TEXTURE_2D, uiName );
	}
	else {
		glDisable( GL_TEXTURE_2D );
	}

// Load all the vertex
	glBegin( GL_TRIANGLES );
	_AC3DVertexToGL( strPath, vMaterial, pObject, false, true );
	glEnd();

// Restore OpenGL attributes
	glPopAttrib();
	glEndList();


   /*=====================================================================*/
	if (bNeedAlpha) {
// Recursively load all the objects into the _alpha_ display list
	listAlpha = glGenLists( 1 );
	glNewList( listAlpha, GL_COMPILE );
	glPushAttrib( GL_ENABLE_BIT );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.2);
// Enable the texture target and bind the _first_ texture only
	if ( glIsTexture(uiName) == GL_TRUE ) {
		glEnable( GL_TEXTURE_2D );
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		glBindTexture( GL_TEXTURE_2D, uiName );
	}
	else {
		glDisable( GL_TEXTURE_2D );
	}

// Load all the vertex
	glBegin( GL_TRIANGLES );
	_AC3DVertexToGL( strPath, vMaterial, pObject, true );
	glEnd();

// Restore all enabled bits
	glPopAttrib();
	glEndList();
	}		// if (bNeedAlpha)


// Debug: print out the number of polys
//	cout << "Number of polygons: " << nbPoly
//		 << " / vertex: " << nbVertex << endl;

	Model* pModel = NULL;
	if (glIsTexture(uiName)) {
		pModel = new Model( list, listTwoSide, listAlpha, strPath + "/" + strFile );
	}
	else {
		pModel = new Model( list, listTwoSide, listAlpha );
	}
	assert( pModel != NULL );

	return pModel;
}


   /*=====================================================================*/
Vertex
ModelLoader::GetNormal(
	Vertex & vO,
	Vertex & vA,
	Vertex & vB )
{
	static Vertex a, b, c;

/// This is the secret formula: c = a^b ;)
//	cx = ay * bz - by * az;
//	cy = bx * az - ax * bz;
//	cz = ax * by - bx * ay;

// Calculate the relative coordinates of A and B to O
	a.x = vA.x - vO.x;
	a.y = vA.y - vO.y;
	a.z = vA.z - vO.z;

	b.x = vB.x - vO.x;
	b.y = vB.y - vO.y;
	b.z = vB.z - vO.z;

// Now, calculate the normal
	c.x = a.y * b.z - b.y * a.z;
	c.y = b.x * a.z - a.x * b.z;
	c.z = a.x * b.y - b.x * a.y;

	return c;
}


   /*=====================================================================*/
   /*                        PRIVATE      METHODS                         */
   /*=====================================================================*/
void
ModelLoader::_AC3DTextureToGL
(
	const AC3DObject* const pObject,
	string& strTextureFile
)
{
	vector<AC3DObject*>::size_type posObj, sizeObj;
	vector<AC3DObject*> vpObj;


	assert( pObject != NULL );

// Get the texture
	if ( pObject->GetTextureFile() != "" ) {
		if (strTextureFile == "") {
			strTextureFile = pObject->GetTextureFile();
		}
		else {
			if (strTextureFile != pObject->GetTextureFile()) {
				OPENCITY_FATAL( "The model tries to use multiple textures" );
				assert( 0 );
			}
		}
	}

// Parse all the child objects and retrieve the texture
	vpObj = pObject->GetVPObject();
	sizeObj = vpObj.size();
	for (posObj = 0; posObj < sizeObj; posObj++) {
		_AC3DTextureToGL( vpObj[posObj], strTextureFile );
	}
}


   /*=====================================================================*/
// TRIANGLES version, inline triangulation
void
ModelLoader::_AC3DVertexToGL
(
	const string& strPath,
	const vector<AC3DMaterial>& vMaterial,
	const AC3DObject* const pObject,
	const bool bProcessTranslucent,
	const bool bProcessTwoSide
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
	Ref r1, r2, r3;

// Used for normal calculation
	Vertex v1, v2, v3, normal;

	assert( pObject != NULL );
	loc = pObject->GetLoc();
	locAccu[0] += loc[0];
	locAccu[1] += loc[1];
	locAccu[2] += loc[2];

// Does this object need alpha processing ?
	if (pObject->IsTranslucent()) {
		bNeedAlpha = true;
	}

// Process only objects that we are asked to do
	if (bProcessTranslucent xor pObject->IsTranslucent())
		goto process_child_objects;

	vVertex = pObject->GetVVertex();
	vpSurface = pObject->GetVPSurface();

	size = vpSurface.size();
	for (pos = 0; pos < size; pos++) {
	// Process only the polygons with the requested number of sides
		if (!bProcessTranslucent and (bProcessTwoSide xor vpSurface[pos]->IsTwoSide())) {
			continue;
		}

// Debug ++nbPoly;
	// Get the material of the current surface
		mat = vMaterial[ vpSurface[pos]->GetMat() ];
	// Set the color of the surface with COLOR_MATERIAL enabled
		glColor3f( mat.rgb.fR, mat.rgb.fG, mat.rgb.fB );
//		glColor4f( 1, 1, 1, 1 );

		vRef = vpSurface[pos]->GetVRef();
		sizeRef = vRef.size();

		assert( sizeRef >= 3 );		// We need at least one triangle
		for (posRef = 1; posRef < sizeRef-1; posRef++) {
		// Fist vertex
			r1 = vRef[0];
			v1.x = vVertex[r1.uiVertIndex].x + locAccu[0];
			v1.y = vVertex[r1.uiVertIndex].y + locAccu[1];
			v1.z = vVertex[r1.uiVertIndex].z + locAccu[2];

		// Second vertex
			r2 = vRef[posRef];
			v2.x = vVertex[r2.uiVertIndex].x + locAccu[0];
			v2.y = vVertex[r2.uiVertIndex].y + locAccu[1];
			v2.z = vVertex[r2.uiVertIndex].z + locAccu[2];

		// Third vertex
			r3 = vRef[posRef+1];
			v3.x = vVertex[r3.uiVertIndex].x + locAccu[0];
			v3.y = vVertex[r3.uiVertIndex].y + locAccu[1];
			v3.z = vVertex[r3.uiVertIndex].z + locAccu[2];

		// Now issue the OpenGL commands
			normal = GetNormal( v1, v2, v3 );
			glNormal3f( normal.x, normal.y, normal.z );

			glTexCoord2f( r1.fTexS, r1.fTexT );
			glVertex3f( v1.x, v1.y, v1.z );

			glTexCoord2f( r2.fTexS, r2.fTexT );
			glVertex3f( v2.x, v2.y, v2.z );

			glTexCoord2f( r3.fTexS, r3.fTexT );
			glVertex3f( v3.x, v3.y, v3.z );
		}
	} // For each surface

process_child_objects:

// Parse all the child objects
	vpObj = pObject->GetVPObject();
	sizeObj = vpObj.size();
	for (posObj = 0; posObj < sizeObj; posObj++) {
		_AC3DVertexToGL( strPath, vMaterial, vpObj[posObj], bProcessTranslucent, bProcessTwoSide );
	}

	locAccu[0] -= loc[0];
	locAccu[1] -= loc[1];
	locAccu[2] -= loc[2];
}




















