/***************************************************************************
                          AC3DMaterial.cpp  -  description
          $Id: ac3dmaterial.cpp,v 1.4 2006/03/12 23:14:18 neoneurone Exp $
                             -------------------
    begin                : mer juin 29 05
    copyright            : (C) 2005 by Duong-Khang NGUYEN
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
 
#include "ac3dmaterial.h"

#include "macros.h"				// for debugging macros
#include "ac3dmacros.h"

#include <sstream>

using std::stringstream;

namespace AC3D {


   /*=====================================================================*/
AC3DMaterial::AC3DMaterial():
rgb( .0, .0, .0 ),
amb( .0, .0, .0 ),
emis( .0, .0, .0 ),
spec( .0, .0, .0 ),
shi( .0 ),
trans( .0 )
{
}


   /*=====================================================================*/
AC3DMaterial::AC3DMaterial(
	const string data
):
rgb( .0, .0, .0 ),
amb( .0, .0, .0 ),
emis( .0, .0, .0 ),
spec( .0, .0, .0 ),
shi( .0 ),
trans( .0 )
{
	char cstr[AC3D_MAX_LINE_LENGTH];

//	OPENCITY_DEBUG("ctor");
	assert( data != "" );

// WARNING: no error checking
// MATERIAL "ac3dmat1" rgb 1 1 1  amb 0.2 0.2 0.2  emis 0 0 0  spec 0.5 0.5 0.5  shi 10  trans 0
	sscanf(
		data.c_str()
		, "MATERIAL %s rgb %f %f %f amb %f %f %f emis %f %f %f spec %f %f %f shi %f trans %f "
		, cstr
		, &rgb.fR, &rgb.fG, &rgb.fB
		, &amb.fR, &amb.fG, &amb.fB
		, &emis.fR, &emis.fG, &emis.fB
		, &spec.fR, &spec.fG, &spec.fB
		, &shi , &trans
	);
		
	this->strName = cstr;
}


   /*=====================================================================*/
AC3DMaterial::~AC3DMaterial()
{
}


   /*=====================================================================*/
string
AC3DMaterial::ToStr() const
{
	stringstream ss;

	ss << "MATERIAL " << this->strName << " "
	   << "rgb " << rgb.fR << " " << rgb.fG << " " << rgb.fB << " "
	   << "amb " << amb.fR << " " << amb.fG << " " << amb.fB << " "
	   << "emis " << emis.fR << " " << emis.fG << " " << emis.fB << " "
	   << "spec " << spec.fR << " " << spec.fG << " " << spec.fB << " "
	   << "shi " << shi << " "
	   << "trans " << trans << endl;

	return ss.str();
}


}
















