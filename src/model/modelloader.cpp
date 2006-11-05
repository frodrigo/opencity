/***************************************************************************
						modelloader.cpp  -  description
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

#include "modelloader.h"

#include "ocm.h"
#include "model.h"

#include "ac3dmodel.h"			// For AC3D structure manipulation
#include "ac3dobject.h"			// For normal calculation

#include "texture.h"			// for texture manipulation

#include "star.h"				// Triangulation algorithms

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
bool bNeedAlpha;

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
	GLuint list = 0, listAlpha = 0;
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
	_AC3DTextureToGL( strPath, pObject, mapTexture );

// Debug
	if (mapTexture.size() > 1) {
		OPENCITY_DEBUG("WARNING: more than 1 texture used by the model");
		assert( 0 );
	}

   /*=====================================================================*/
// Recursively load all the objects into the _opaque_ display list
	list = glGenLists( 1 );
	glNewList( list, GL_COMPILE );
// Save the all enabled GL bits
	glPushAttrib( GL_ENABLE_BIT );
	glDisable( GL_BLEND );
// Enable the texture target and bind the _first_ texture only
	if (mapTexture.size() > 0) {
		glEnable( GL_TEXTURE_2D );
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
		glBindTexture( GL_TEXTURE_2D, (mapTexture.begin())->second);
	}
	else {
		glDisable( GL_TEXTURE_2D );
	}

// Load all the vertex
	glBegin( GL_TRIANGLES );
	_AC3DVertexToGL( strPath, vMaterial, pObject, false );
	glEnd();

// Restore all enabled bits
	glPopAttrib();
	glEndList();

   /*=====================================================================*/
	if (bNeedAlpha) {
// Recursively load all the objects into the _alpha_ display list
	listAlpha = glGenLists( 1 );
	glNewList( listAlpha, GL_COMPILE );
// Save the all enabled GL bits
	glPushAttrib( GL_ENABLE_BIT );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.2);
//	glBlendFunc( GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA );
//	glBlendFunc( GL_ONE, GL_ZERO );
// Enable the texture target and bind the _first_ texture only
	if (mapTexture.size() > 0) {
		glEnable( GL_TEXTURE_2D );
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
//		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );
//		GLfloat env_color [] = { 0, 0, 0, 0 };
//		GLfloat env_color [] = { 1, 1, 1, 0 };
//		glTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, env_color );
//		GLfloat obj_color [] = { 01, 01, 01, 01 };
//		glColor4fv( obj_color );
		glBindTexture( GL_TEXTURE_2D, (mapTexture.begin())->second);
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

	return new Model( list, listAlpha, mapTexture );
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
		_AC3DTextureToGL( strPath, vpObj[posObj], mapTexture );
	}
}


   /*=====================================================================*/
/* POLYGON version
void
ModelLoader::_AC3DToGL(
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
		_AC3DToGL( strPath, vMaterial, vpObj[posObj], mapTexture );
	}

	locAccu[0] -= loc[0];
	locAccu[1] -= loc[1];
	locAccu[2] -= loc[2];
}
*/


   /*=====================================================================*/
// TRIANGLE_FAN version
/*
void
ModelLoader::_AC3DVertexToGL
(
	const string& strPath,
	const vector<AC3DMaterial>& vMaterial,
	const AC3DObject* const pObject,
	const bool bProcessTranslucent
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


// debug, location calculation
//cout << "in  : " << locAccu[0] << "/" << locAccu[1] << "/" << locAccu[2] << endl;

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


	// With 3 first vertices, we can calculate the normal for the current surface
	// Fist vertex
		r1 = vRef[0];
		v1.x = vVertex[r1.uiVertIndex].x + locAccu[0];
		v1.y = vVertex[r1.uiVertIndex].y + locAccu[1];
		v1.z = vVertex[r1.uiVertIndex].z + locAccu[2];

	// Second vertex
		r2 = vRef[1];
		v2.x = vVertex[r2.uiVertIndex].x + locAccu[0];
		v2.y = vVertex[r2.uiVertIndex].y + locAccu[1];
		v2.z = vVertex[r2.uiVertIndex].z + locAccu[2];

	// Third vertex
		r3 = vRef[2];
		v3.x = vVertex[r3.uiVertIndex].x + locAccu[0];
		v3.y = vVertex[r3.uiVertIndex].y + locAccu[1];
		v3.z = vVertex[r3.uiVertIndex].z + locAccu[2];

	// Set the normal
		glBegin( GL_TRIANGLE_FAN );
		normal = GetNormal( v1, v2, v3 );
		glNormal3f( normal.x, normal.y, normal.z );

	// Draw the first 3 vertices
		glTexCoord2f( r1.fTexS, r1.fTexT );
		glVertex3f( v1.x, v1.y, v1.z );

		glTexCoord2f( r2.fTexS, r2.fTexT );
		glVertex3f( v2.x, v2.y, v2.z );

		glTexCoord2f( r3.fTexS, r3.fTexT );
		glVertex3f( v3.x, v3.y, v3.z );

	// Draw the other vertices
		for (posRef = 3; posRef < sizeRef; posRef++) {
			r1 = vRef[posRef];
			v1.x = vVertex[r1.uiVertIndex].x + locAccu[0];
			v1.y = vVertex[r1.uiVertIndex].y + locAccu[1];
			v1.z = vVertex[r1.uiVertIndex].z + locAccu[2];

			glTexCoord2f( r1.fTexS, r1.fTexT );
			glVertex3f( v1.x, v1.y, v1.z );
		} // For each vertex
		glEnd();
	} // For each surface

process_child_objects:

// Parse all the child objects
	vpObj = pObject->GetVPObject();
	sizeObj = vpObj.size();
//debug
//cout << "kids: " << pObject->GetNumberKid() << " / objects: " << sizeObj << endl;
	for (posObj = 0; posObj < sizeObj; posObj++) {
		_AC3DVertexToGL( strPath, vMaterial, vpObj[posObj], bProcessTranslucent );
	}

	locAccu[0] -= loc[0];
	locAccu[1] -= loc[1];
	locAccu[2] -= loc[2];

// debug, location calculation
//cout << "out : " << locAccu[0] << "/" << locAccu[1] << "/" << locAccu[2] << endl;
}
*/


   /*=====================================================================*/
// TRIANGLES version, inline triangulation
//#define TRIANGULATION_TEST 1
#undef TRIANGULATION_TEST
#ifndef TRIANGULATION_TEST
void
ModelLoader::_AC3DVertexToGL
(
	const string& strPath,
	const vector<AC3DMaterial>& vMaterial,
	const AC3DObject* const pObject,
	const bool bProcessTranslucent
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


// debug, location calculation
//cout << "in  : " << locAccu[0] << "/" << locAccu[1] << "/" << locAccu[2] << endl;

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
//debug
//cout << "kids: " << pObject->GetNumberKid() << " / objects: " << sizeObj << endl;
	for (posObj = 0; posObj < sizeObj; posObj++) {
		_AC3DVertexToGL( strPath, vMaterial, vpObj[posObj], bProcessTranslucent );
	}

	locAccu[0] -= loc[0];
	locAccu[1] -= loc[1];
	locAccu[2] -= loc[2];

// debug, location calculation
//cout << "out : " << locAccu[0] << "/" << locAccu[1] << "/" << locAccu[2] << endl;
}


   /*=====================================================================*/
// TRIANGLES version, external triangulation
#else
void
ModelLoader::_AC3DVertexToGL
(
	const string& strPath,
	const vector<AC3DMaterial>& vMaterial,
	const AC3DObject* const pObject,
	const bool bProcessTranslucent
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

// Triangulation
	triangulation::Triangulation* triangulator = new triangulation::Star();
	triangulation::Triangle* triangles = NULL;
	triangulation::Vertex* triVertices = NULL;


	assert( pObject != NULL );


// debug, location calculation
//cout << "in  : " << locAccu[0] << "/" << locAccu[1] << "/" << locAccu[2] << endl;

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

		triVertices = new triangulation::Vertex[sizeRef];

		for (posRef = 0; posRef < sizeRef; posRef++) {
			r1 = vRef[posRef];
			triVertices[posRef].x = vVertex[r1.uiVertIndex].x + locAccu[0];
			triVertices[posRef].y = vVertex[r1.uiVertIndex].y + locAccu[1];
			triVertices[posRef].z = vVertex[r1.uiVertIndex].z + locAccu[2];
			triVertices[posRef].texCoord.fS = r1.fTexS;
			triVertices[posRef].texCoord.fT = r1.fTexT;
		}

		triangles = triangulator->process( sizeRef, triVertices );
		delete [] triVertices;

		for (posRef = 0; posRef < sizeRef-2; posRef++) {
		// Now issue the OpenGL commands
		//	normal = GetNormal(
		//		triangles[posRef].a, triangles[posRef].b, triangles[posRef].c );
		//	glNormal3f( normal.x, normal.y, normal.z );

			glTexCoord2f(
				triangles[posRef].a.texCoord.fS, triangles[posRef].a.texCoord.fT );
			glVertex3f(
				triangles[posRef].a.x, triangles[posRef].a.y, triangles[posRef].a.z );
			glTexCoord2f(
				triangles[posRef].b.texCoord.fS, triangles[posRef].b.texCoord.fT );
			glVertex3f(
				triangles[posRef].b.x, triangles[posRef].b.y, triangles[posRef].b.z );
			glTexCoord2f(
				triangles[posRef].c.texCoord.fS, triangles[posRef].c.texCoord.fT );
			glVertex3f(
				triangles[posRef].c.x, triangles[posRef].c.y, triangles[posRef].c.z );
		}

		delete [] triangles;

	} // For each surface

process_child_objects:

// Parse all the child objects
	vpObj = pObject->GetVPObject();
	sizeObj = vpObj.size();
//debug
//cout << "kids: " << pObject->GetNumberKid() << " / objects: " << sizeObj << endl;
	for (posObj = 0; posObj < sizeObj; posObj++) {
		_AC3DVertexToGL( strPath, vMaterial, vpObj[posObj], bProcessTranslucent );
	}

	locAccu[0] -= loc[0];
	locAccu[1] -= loc[1];
	locAccu[2] -= loc[2];

	delete triangulator;

// debug, location calculation
//cout << "out : " << locAccu[0] << "/" << locAccu[1] << "/" << locAccu[2] << endl;
}
#endif




















