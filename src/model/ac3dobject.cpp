/***************************************************************************
						AC3DModel.h  -  description
							-------------------
	begin                : june 29th, 05
	copyright            : (C) 2005-2007 by Duong Khang NGUYEN
	email                : neoneurone @ gmail com
	
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

#include "ac3dobject.h"

#include "macros.h"				// debug macros
#include "ac3dmacros.h"			// AC3D specific macros

#include <cstring>
#include <cstdio>
#include <cstdlib>


namespace AC3D {


   /*=====================================================================*/
AC3DObject::AC3DObject():
strName(""),
fCrease(45),
_bTranslucent(false),
uiNumVert(0),
uiNumSurf(0),
uiKids(0)
{
// WARNING: uninitialized attributes
	OPENCITY_DEBUG("ctor1");

	this->ac3dSetDefault();
}


   /*=====================================================================*/
AC3DObject::AC3DObject
(
	stringstream& data
):
strName(""),
fCrease(45),
_bTranslucent(false),
uiNumVert(0),
uiNumSurf(0),
uiKids(0)
{
//	OPENCITY_DEBUG("ctor2");

	this->ac3dSetDefault();
	this->Parse( data );
}


   /*=====================================================================*/
AC3DObject::~AC3DObject()
{
	//OPENCITY_DEBUG("dtor");
	vector<AC3DObject*>::size_type pos, size;


	size = this->vpObject.size();
	for (pos = 0; pos < size; pos++)
		delete vpObject[pos];

	size = this->vpSurface.size();
	for (pos = 0; pos < size; pos++)
		delete vpSurface[pos];
}


   /*=====================================================================*/
void
AC3DObject::AddKid
(
	AC3DObject* kid
)
{
	this->vpObject.push_back( kid );

// Automatically set the translucent attribute
	if (_bTranslucent && (this->type == OBJECT_GROUP))
		kid->SetTranslucent(true);
}


   /*=====================================================================*/
bool
AC3DObject::IsTranslucent() const
{
	return _bTranslucent;
}


   /*=====================================================================*/
void
AC3DObject::SetTranslucent(bool bValue)
{
	_bTranslucent = bValue;
}


   /*=====================================================================*/
string
AC3DObject::GetName() const
{
	return this->strName;
}


   /*=====================================================================*/
unsigned int
AC3DObject::GetNumberKid() const
{
	return this->uiKids;
}


   /*=====================================================================*/
const string&
AC3DObject::GetTextureFile() const
{
	return this->strTexture;
}


   /*=====================================================================*/
const float*
AC3DObject::GetLoc() const
{
	return this->fLoc;
}


   /*=====================================================================*/
const vector<Vertex>&
AC3DObject::GetVVertex() const
{
	return this->vVertex;
}


   /*=====================================================================*/
const vector<AC3DSurface*>&
AC3DObject::GetVPSurface() const
{
	return this->vpSurface;
}


   /*=====================================================================*/
const vector<AC3DObject*>&
AC3DObject::GetVPObject() const
{
	return this->vpObject;
}


   /*=====================================================================*/
void
AC3DObject::Parse(
	stringstream& data
)
{
	char line[AC3D_MAX_LINE_LENGTH];
	char cstr[AC3D_MAX_LINE_LENGTH];
	uint uiDataSize;		// The size of the optional data

//debug cout << "Given ss: " << data.str() << endl;
	
	data.getline( line, AC3D_MAX_LINE_LENGTH );
	while (data.good()) {
		if ( strncmp( line, AC3D_TOKEN_OBJECT, AC3D_TOKEN_OBJECT_L ) == 0 ) {
			sscanf( line, "OBJECT %s", cstr );
			if (strncmp( cstr, "world", 5 ) == 0)
				this->type = OBJECT_WORLD;
			else if (strncmp( cstr, "poly", 5 ) == 0)
				this->type = OBJECT_POLYGON;
			else if (strncmp( cstr, "group", 5 ) == 0)
				this->type = OBJECT_GROUP;
			data.getline( line, AC3D_MAX_LINE_LENGTH );
		} else
		if ( strncmp( line, AC3D_TOKEN_NAME, AC3D_TOKEN_NAME_L ) == 0 ) {
			sscanf( line,  "name %s", cstr );
			this->strName = cstr;
			data.getline( line, AC3D_MAX_LINE_LENGTH );
		} else
		if ( strncmp( line, AC3D_TOKEN_DATA, AC3D_TOKEN_DATA_L ) == 0 ) {
			sscanf( line,  "data %u", &uiDataSize );
		// Buffer overflow protection
			if ( uiDataSize >= AC3D_MAX_LINE_LENGTH ) {
				OPENCITY_FATAL( "The optional data is too big." );
				abort();
			}
		// Extract the optional data
			data.getline( cstr, AC3D_MAX_LINE_LENGTH );
			cstr[uiDataSize] = '\0';
			_sData = cstr;
		// OpenCity AC3D "alpha" command detection
			_bTranslucent = (_sData.find(OC_AC3D_TOKEN_ALPHA) != string::npos);
			data.getline( line, AC3D_MAX_LINE_LENGTH );
			OPENCITY_DEBUG( "The AC3D optional data is : |" << _sData << "|" );
		} else
		if ( strncmp( line, AC3D_TOKEN_TEXTURE, AC3D_TOKEN_TEXTURE_L ) == 0 ) {
			sscanf( line, "texture %s", cstr );
			this->strTexture = cstr;
		// Strip out the doublequotes
			if (this->strTexture.size() >= 2) {
				this->strTexture = this->strTexture.substr( 1, this->strTexture.size()-2 );
			} else {
				OPENCITY_DEBUG("Unable to strip doublequotes");
			}
			data.getline( line, AC3D_MAX_LINE_LENGTH );
		} else
		if ( strncmp( line, AC3D_TOKEN_TEXREP, AC3D_TOKEN_TEXREP_L ) == 0 ) {
			sscanf( line, "texrep %f %f", this->fTexRep, this->fTexRep+1 );
			data.getline( line, AC3D_MAX_LINE_LENGTH );
		} else
		if ( strncmp( line, AC3D_TOKEN_ROT, AC3D_TOKEN_ROT_L ) == 0 ) {
			sscanf(
				line,
				"rot %f %f %f %f %f %f %f %f %f",
				this->fRot  , this->fRot+1, this->fRot+2,
				this->fRot+3, this->fRot+4, this->fRot+5,
				this->fRot+6, this->fRot+7, this->fRot+8
			);
			data.getline( line, AC3D_MAX_LINE_LENGTH );
		} else
		if ( strncmp( line, AC3D_TOKEN_LOC, AC3D_TOKEN_LOC_L ) == 0 ) {
			sscanf( line, "loc %f %f %f", this->fLoc, this->fLoc+1, this->fLoc+2 );
			data.getline( line, AC3D_MAX_LINE_LENGTH );
		} else
		if ( strncmp( line, AC3D_TOKEN_CREASE, AC3D_TOKEN_CREASE_L ) == 0 ) {
			sscanf( line, "crease %f", &(this->fCrease) );
			data.getline( line, AC3D_MAX_LINE_LENGTH );
		} else
		if ( strncmp( line, AC3D_TOKEN_URL, AC3D_TOKEN_URL_L ) == 0 ) {
			sscanf( line, "url %s", cstr );
			this->strURL = cstr;
			data.getline( line, AC3D_MAX_LINE_LENGTH );
		} else
		if ( strncmp( line, AC3D_TOKEN_KIDS, AC3D_TOKEN_KIDS_L ) == 0 ) {
			sscanf( line, "kids %u", &(this->uiKids) );
			data.getline( line, AC3D_MAX_LINE_LENGTH );
		} else
		if ( strncmp( line, AC3D_TOKEN_NUMVERT, AC3D_TOKEN_NUMVERT_L ) == 0 ) {
			sscanf( line, "numvert %u", &(this->uiNumVert) );
			ac3dobjectParseVertex( this->uiNumVert, data, line );
		} else
		if ( strncmp( line, AC3D_TOKEN_NUMSURF, AC3D_TOKEN_NUMSURF_L ) == 0 ) {
			sscanf( line, "numsurf %u", &(this->uiNumSurf) );
			ac3dobjectParseSurface( this->uiNumSurf, data, line );
		} else {
			OPENCITY_DEBUG("WARNING: unknown command discarded. See below: ");
			OPENCITY_DEBUG( "|" << line << "|" );
			data.getline( line, AC3D_MAX_LINE_LENGTH );
		}
	}

	data.clear();
}


   /*=====================================================================*/
string
AC3DObject::ToStr() const
{
	stringstream ss;
	vector<AC3DObject*>::size_type pos, size;


	ss << "OBJECT ";
	switch (this->type) {
		case OBJECT_WORLD: ss << "world"; break;
		case OBJECT_POLYGON: ss << "poly"; break;
		case OBJECT_GROUP: ss << "group"; break;
	}
	ss << endl;

	ss << ((this->strName != "") ? "name " + this->strName + "\n" : "")
	   << "loc " << this->fLoc[0] << " "
				 << this->fLoc[1] << " "
				 << this->fLoc[2] << endl
	   << "texrep " << this->fTexRep[0] << " "
					<< this->fTexRep[1] << endl;

	ss << ((this->strTexture != "") ? "texture " + this->strTexture + "\n" : "")
	   << "crease " << this->fCrease << endl
	   << "numvert " << this->uiNumVert << endl;

	size = this->vVertex.size();
	for (pos = 0; pos < size; pos++) {
		ss << this->vVertex[pos].x << " "
		   << this->vVertex[pos].y << " "
		   << this->vVertex[pos].z << endl;
	}

	ss << "numsurf " << this->uiNumSurf << endl;

	size = this->vpSurface.size();
	for (pos = 0; pos < size; pos++)
		ss << this->vpSurface[pos]->ToStr();

	ss << "kids " << this->uiKids << endl;

	size = this->vpObject.size();
	for (pos = 0; pos < size; pos++)
		ss << this->vpObject[pos]->ToStr();

	return ss.str();
}


   /*=====================================================================*/
   /*                        PRIVATE      METHODS                         */
   /*=====================================================================*/
void
AC3DObject::ac3dSetDefault()
{
// Default "texrep" values
	this->fTexRep[0] = 1.;
	this->fTexRep[1] = 1.;

// Default "loc" values
	this->fLoc[0] = 0.; this->fLoc[1] = 0.; this->fLoc[2] = 0.;

// Default "rot" values
	this->fRot[0] = 1.; this->fRot[1] = 0.; this->fRot[2] = 0.;
	this->fRot[3] = 0.; this->fRot[4] = 1.; this->fRot[5] = 0.;
	this->fRot[3] = 0.; this->fRot[4] = 0.; this->fRot[5] = 1.;
}


   /*=====================================================================*/
void
AC3DObject::ac3dobjectParseVertex
(
	unsigned int nb,
	stringstream& data,
	char* buffer
)
{
	Vertex newVertex;

// WARNING no error checking
	data.getline( buffer, AC3D_MAX_LINE_LENGTH );
	while (data.good() && (nb-- > 0)) {
		sscanf( buffer, "%f %f %f", &newVertex.x, &newVertex.y, &newVertex.z );
		this->vVertex.push_back( newVertex );
		data.getline( buffer, AC3D_MAX_LINE_LENGTH );
	}
}


   /*=====================================================================*/
void
AC3DObject::ac3dobjectParseSurface
(
	unsigned int nb,
	stringstream& data,
	char* buffer
)
{
	stringstream ss;

	bool endBlock = false;
	bool endSurf = false;

	AC3DSurface* pSurf = NULL;

	data.getline( buffer, AC3D_MAX_LINE_LENGTH );
	while (data.good() && !endSurf) {
		if ( strncmp( buffer, AC3D_TOKEN_SURF, AC3D_TOKEN_SURF_L ) == 0 ) {
			ss << buffer << endl;
			data.getline( buffer, AC3D_MAX_LINE_LENGTH );
		} else
		if ( strncmp( buffer, AC3D_TOKEN_MAT, AC3D_TOKEN_MAT_L ) == 0 ) {
			ss << buffer << endl;
			data.getline( buffer, AC3D_MAX_LINE_LENGTH );
		} else
		if ( strncmp( buffer, AC3D_TOKEN_KIDS, AC3D_TOKEN_KIDS_L ) == 0 ) {
			endSurf = true;
		} else
		if ( strncmp( buffer, AC3D_TOKEN_REFS, AC3D_TOKEN_REFS_L ) == 0 ) {
			ss << buffer << endl;
			data.getline( buffer, AC3D_MAX_LINE_LENGTH );

			endBlock = false;
			while (data.good() && !endBlock) {
			// IF this is a new "SURF", "mat" or "refs" command
			// THEN  we reach the end of the block
				if ((strncmp( buffer, AC3D_TOKEN_SURF, AC3D_TOKEN_SURF_L ) == 0 )
				 || (strncmp( buffer, AC3D_TOKEN_MAT, AC3D_TOKEN_MAT_L ) == 0 )
				 || (strncmp( buffer, AC3D_TOKEN_REFS, AC3D_TOKEN_REFS_L ) == 0 )
				 || (strncmp( buffer, AC3D_TOKEN_KIDS, AC3D_TOKEN_KIDS_L ) == 0 )) {
					endBlock = true;
				}
				else {
					//debug cout << "New buffer: " << buffer << endl;
					ss << buffer << endl;
					data.getline( buffer, AC3D_MAX_LINE_LENGTH );
				}
			}

			// Create new surface from the data block
			pSurf = new AC3DSurface( ss );
			this->vpSurface.push_back( pSurf );
			ss.str("");
		}
	}
	
}


}

























