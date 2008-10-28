/***************************************************************************
						model.h  -  description
							-------------------
	begin                : may 22th, 2004
	copyright            : (C) 2003-2005 by Duong Khang NGUYEN
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

#ifndef _OPENCITY_MODEL_H_
#define _OPENCITY_MODEL_H_ 1

#include "main.h"
#include "texture.h"

// The constants used to select the display list
#define OC_OPAQUE_ONESIDE_LIST		0x01
#define OC_OPAQUE_TWOSIDE_LIST		0x02
#define OC_ALPHA_LIST				0x04


//========================================================================
/** This is an internal 3d model representation used by OpenCity.
*/
class Model {
public:
	Model(
		GLuint dlOpaqueOneSide,
		GLuint dlOpaqueTwoSide,
		GLuint dlAlpha
	);

	Model(
		GLuint dlOpaqueOneSide,
		GLuint dlOpaqueTwoSide,
		GLuint dlAlpha,
		const string& rcsTextureFile
	);

	~Model();


//========================================================================
/** Simply call the internal OpenGL display list
*/
	void
	DisplayList() const;


//========================================================================
/** Call the internal OpenGL display list after modifying the MODELVIEW
	matrix according to the given parameters
\param rcfW = GL x
\param rcfL = GL z
\param tabY = GL y
*/
	void
	DisplayList(
		const OC_FLOAT & rcfW,
		const OC_FLOAT & rcfL,
		const OC_BYTE tabY []
	) const;

	void
	DisplayList(
		const OC_FLOAT & rcfW,
		const OC_FLOAT & rcfL,
		const OC_BYTE tabY [],
		const uint dlMask
	) const;


private:
// Display lists used with AC3D model
	GLuint	_uiOpaqueOneSide;		///< One side opaque polygon display list
	GLuint	_uiOpaqueTwoSide;		///< Two side opaque polygon display list
	GLuint	_uiAlpha;				///< Alpha bleding polygon display list

/// This model texture object is used to keep the right texture reference count
	Texture	moTexture;
};


#endif




































