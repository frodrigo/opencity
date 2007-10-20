/***************************************************************************
						AC3DSurface.cpp  -  description
							-------------------
	begin                : june 29th, 05
	copyright            : (C) 2005 by Duong-Khang NGUYEN
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

#include "ac3dsurface.h"

#include "macros.h"		// debug macros
#include "ac3dmacros.h"

#include <cstring>

namespace AC3D {


   /*=====================================================================*/
AC3DSurface::AC3DSurface():
uiAttribute(0)
{
	OPENCITY_DEBUG("ctor1");
}


   /*=====================================================================*/
AC3DSurface::AC3DSurface
(
	stringstream& data
):
uiAttribute(0)
{
//	OPENCITY_DEBUG("ctor2");

	this->Parse( data );
}


   /*=====================================================================*/
AC3DSurface::~AC3DSurface()
{
}


   /*=====================================================================*/
bool
AC3DSurface::IsTwoSide() const
{
	return this->bTwoSide;
}


   /*=====================================================================*/
const unsigned int
AC3DSurface::GetMat() const
{
	return this->uiMat;
}


   /*=====================================================================*/
const vector<Ref>&
AC3DSurface::GetVRef() const
{
	return this->vRef;
}


   /*=====================================================================*/
void
AC3DSurface::Parse
(
	stringstream& data
)
{
	char line[AC3D_MAX_LINE_LENGTH];
	unsigned int numRef;
	bool end;
	Ref newRef;

//debug cout << "Given ss: " << data.str() << endl;

	data.getline( line, AC3D_MAX_LINE_LENGTH );
	end = false;
	while (data.good() && !end) {
		if ( strncmp( line, AC3D_TOKEN_SURF, AC3D_TOKEN_SURF_L ) == 0 ) {
			sscanf( line, "SURF %x", &(this->uiAttribute));
			this->type = (SURFACE_TYPE)(this->uiAttribute & 0xF);
			this->bShaded = this->uiAttribute & 0x10;
			this->bTwoSide = this->uiAttribute & 0x20;
			data.getline( line, AC3D_MAX_LINE_LENGTH );
		} else
		if ( strncmp( line, AC3D_TOKEN_MAT, AC3D_TOKEN_MAT_L ) == 0 ) {
			sscanf( line,  "mat %u", &(this->uiMat) );
			data.getline( line, AC3D_MAX_LINE_LENGTH );
		} else
		if ( strncmp( line, AC3D_TOKEN_REFS, AC3D_TOKEN_REFS_L ) == 0 ) {
			sscanf( line, "refs %u", &(this->uiNumRef) );

		// Read all the following "refs line"
			numRef = this->uiNumRef;
			data.getline( line, AC3D_MAX_LINE_LENGTH );
			while (data.good() && (numRef-- > 0)) {
				sscanf(
					line,
					"%u %f %f",
					&newRef.uiVertIndex, &newRef.fTexS, &newRef.fTexT
				);
				this->vRef.push_back( newRef );
				data.getline( line, AC3D_MAX_LINE_LENGTH );
			}
			end = true;
		}
	}

	data.clear();
}


   /*=====================================================================*/
string
AC3DSurface::ToStr() const
{
	stringstream ss;
	vector<Ref>::size_type pos, size;

	ss.flags( stringstream::hex );
	ss << "SURF 0x" << this->uiAttribute << endl;
	ss.flags( stringstream::dec );

	ss << "mat " << this->uiMat << endl
	   << "refs " << this->uiNumRef << endl;

	size = this->vRef.size();
	for (pos = 0; pos < size; pos++ ) {
		ss << vRef[pos].uiVertIndex << " "
		   << vRef[pos].fTexS << " "
		   << vRef[pos].fTexT << endl;
	}

	return ss.str();
}



}




























