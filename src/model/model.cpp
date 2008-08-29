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
Model::Model(
	const GLfloat data [],
	const GLuint & size ):
ftabData( data ),
ftabRGB( NULL ),
ftabTexCoord( NULL ),
uitabTexName( NULL ),
_uiOpaqueOneSide( 0 ),
_uiOpaqueTwoSide( 0 ),
_uiAlpha( 0 ),
_uiTexture( 0 )
{
	OPENCITY_DEBUG( "ctor1" );

	uiTabSize = size;
}


   /*=====================================================================*/
Model::Model(
	const GLfloat data [],
	const GLuint & size,
	const GLfloat rgb [],
	const GLfloat tcoord [],
	const GLuint  tname [] ):
ftabData( data ),
ftabRGB( rgb ),
ftabTexCoord( tcoord ),
uitabTexName( tname ),
_uiOpaqueOneSide( 0 ),
_uiOpaqueTwoSide( 0 ),
_uiAlpha( 0 ),
_uiTexture( 0 )
{
	OPENCITY_DEBUG( "ctor2" );

	uiTabSize = size;
}


   /*=====================================================================*/
Model::Model
(
	GLuint dlOpaqueOneSide,
	GLuint dlOpaqueTwoSide,
	GLuint dlAlpha,
	GLuint tex
):
ftabData( NULL ),
ftabRGB( NULL ),
ftabTexCoord( NULL ),
uitabTexName( NULL ),
uiTabSize( 0 ),
_uiOpaqueOneSide( dlOpaqueOneSide ),
_uiOpaqueTwoSide( dlOpaqueTwoSide ),
_uiAlpha( dlAlpha ),
_uiTexture( tex )
{
	OPENCITY_DEBUG( "ctor3" );
}


   /*=====================================================================*/
Model::~Model()
{
	OPENCITY_DEBUG( "dtor" );

	delete [] ftabData;
	delete [] ftabRGB;
	delete [] ftabTexCoord;
	delete [] uitabTexName;

// Delete display lists
	if (glIsList( _uiOpaqueOneSide ))
		glDeleteLists( _uiOpaqueOneSide, 1 );
	if (glIsList( _uiOpaqueTwoSide ))
		glDeleteLists( _uiOpaqueTwoSide, 1 );
	if (glIsList( _uiAlpha ))
		glDeleteLists( _uiAlpha, 1 );

// Delete texture
	if (glIsTexture(_uiTexture))
		glDeleteTextures( 1, &_uiTexture );
}


   /*=====================================================================*/
void
Model::DisplayPoly(
	const OC_FLOAT & rcfW,
	const OC_FLOAT & rcfH,
	const OC_BYTE tabY []
) const
{
	uint	uiVertI = 0;
	uint	uiTCoordI = 0;
	int	iMatI = -1;
	uint	uiMatINew = 0;
	int	iOldTex = -1;

// WARNING: GL_COLOR_MATERIAL is enabled with
	glEnable( GL_TEXTURE_2D );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glBegin(GL_QUADS);

	while (uiVertI < uiTabSize) {
		uiMatINew = uiVertI / 12;		// _Quad_

	   // IF it's a new material THEN
		if (uiMatINew != (uint)iMatI) {
			iMatI = uiMatINew;

		// IF this quad needs a new tex THEN
			if (iOldTex != (GLint)uitabTexName[iMatI]) {
				glEnd();
				iOldTex = uitabTexName[iMatI];
				glBindTexture(GL_TEXTURE_2D, uitabTexName[iMatI]);
				glBegin( GL_QUADS );
			}

			glColor4f(
				ftabRGB[iMatI*3],
				ftabRGB[iMatI*3+1],
				ftabRGB[iMatI*3+2], 1. );
		}

		glTexCoord2f(
			ftabTexCoord[uiTCoordI],
			ftabTexCoord[uiTCoordI+1]);
		glVertex3f(
			ftabData[uiVertI] + rcfW,
			ftabData[uiVertI+1],
			ftabData[uiVertI+2] + rcfH );

		uiTCoordI += 2;
		uiVertI += 3;
	}

	glEnd();

	glDisable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );
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
Model::DisplayList(
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
Model::DisplayList(
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






















