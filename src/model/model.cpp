/***************************************************************************
						model.cpp  -  description
							-------------------
	begin                : may 22th, 2004
	copyright            : (C) 2004-2007 by Duong-Khang NGUYEN
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

#include "model.h"


   /*=====================================================================*/
Model::Model
(
	GLuint dlOpaqueOneSide,
	GLuint dlOpaqueTwoSide,
	GLuint dlAlpha
):
_uiOpaqueOneSide( dlOpaqueOneSide ),
_uiOpaqueTwoSide( dlOpaqueTwoSide ),
_uiAlpha( dlAlpha )
{
	OPENCITY_DEBUG( "Pctor 1" );
}


   /*=====================================================================*/
Model::Model
(
	GLuint dlOpaqueOneSide,
	GLuint dlOpaqueTwoSide,
	GLuint dlAlpha,
	const string& rcsTextureFile
):
_uiOpaqueOneSide( dlOpaqueOneSide ),
_uiOpaqueTwoSide( dlOpaqueTwoSide ),
_uiAlpha( dlAlpha ),
moTexture( rcsTextureFile )
{
	OPENCITY_DEBUG( "Pctor 2" );
}


   /*=====================================================================*/
Model::~Model()
{
	OPENCITY_DEBUG( "dtor" );

// Delete display lists
	if (glIsList( _uiOpaqueOneSide ))
		glDeleteLists( _uiOpaqueOneSide, 1 );
	if (glIsList( _uiOpaqueTwoSide ))
		glDeleteLists( _uiOpaqueTwoSide, 1 );
	if (glIsList( _uiAlpha ))
		glDeleteLists( _uiAlpha, 1 );
}


   /*=====================================================================*/
void
Model::DisplayList() const
{
	assert( _uiOpaqueOneSide != 0 );
	assert( _uiOpaqueTwoSide != 0 );

	glCallList( _uiOpaqueOneSide );
	glCallList( _uiOpaqueTwoSide );
	if (glIsList( _uiAlpha ))
		glCallList( _uiAlpha );
}


   /*=====================================================================*/
void
Model::DisplayList
(
	const OC_FLOAT & rcfW,
	const OC_FLOAT & rcfL,
	const OC_BYTE tabY []
) const
{
	assert( tabY != NULL );
	assert( _uiOpaqueOneSide != 0 );
	assert( _uiOpaqueTwoSide != 0 );

// Call the opaque list first, then the alpha list
//	glMatrixMode( GL_MODELVIEW );		// default matrix mode
	glPushMatrix();
	glTranslatef( rcfW, tabY[0], rcfL );
	glCallList( _uiOpaqueOneSide );
	glCallList( _uiOpaqueTwoSide );
	if (glIsList( _uiAlpha ))
		glCallList( _uiAlpha );
	glPopMatrix();
}


   /*=====================================================================*/
void
Model::DisplayList
(
	const OC_FLOAT & rcfW,
	const OC_FLOAT & rcfL,
	const OC_BYTE tabY [],
	const uint dlMask
) const
{
	assert( tabY != NULL );
	assert( _uiOpaqueOneSide != 0 );
	assert( _uiOpaqueTwoSide != 0 );

// Call the selected lists
//	glMatrixMode( GL_MODELVIEW );		// default matrix mode
	glPushMatrix();

	// Correct for a 1x1 model
	GLdouble matrix[16] = {
		1,tabY[3]-tabY[0],0,0,
		0,1,0,0,
		0,tabY[1]-tabY[0],1,0,
		0,0,0,1
	};
	glMultMatrixd(matrix);

	glTranslatef( rcfW, tabY[0]-rcfW*(tabY[3]-tabY[0])-rcfL*(tabY[1]-tabY[0]), rcfL );
//	glTranslatef( rcfW, tabY[0], rcfL );
	if (dlMask & OC_OPAQUE_ONESIDE_LIST)
		glCallList( _uiOpaqueOneSide );
	if (dlMask & OC_OPAQUE_TWOSIDE_LIST)
		glCallList( _uiOpaqueTwoSide );
	if (dlMask & OC_ALPHA_LIST and glIsList( _uiAlpha ))
		glCallList( _uiAlpha );
	glPopMatrix();
}
