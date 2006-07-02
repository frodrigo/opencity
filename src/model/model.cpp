/***************************************************************************
                          model.cpp  -  description
          $Id$
                             -------------------
    begin                : sam mai 22 2004
    copyright            : (C) by Duong-Khang NGUYEN
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

#include "model.h"

#include "ocm.h"

static GLuint uiTabIndex;
static GLuint uiYIndex;
static const GLfloat* ftabTemp;


   /*=====================================================================*/
Model::Model(
	const GLfloat data [],
	const GLuint & size ):
ftabData( data ),
ftabRGB( NULL ),
ftabTexCoord( NULL ),
uitabTexName( NULL ),
uiOpaqueList( 0 ),
uiAlphaList( 0 )
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
uiOpaqueList( 0 ),
uiAlphaList( 0 )
{
	OPENCITY_DEBUG( "ctor2" );

	uiTabSize = size;
}


   /*=====================================================================*/
Model::Model
(
	GLuint dlOpaque,
	GLuint dlAlpha,
	std::map<string, GLuint> mapTex
):
ftabData( NULL ),
ftabRGB( NULL ),
ftabTexCoord( NULL ),
uitabTexName( NULL ),
uiTabSize( 0 ),
uiOpaqueList( dlOpaque ),
uiAlphaList( dlAlpha ),
mapTexture( mapTex )
{
	OPENCITY_DEBUG( "ctor3" );
}


   /*=====================================================================*/
Model::~Model()
{
	OPENCITY_DEBUG( "dtor" );

	std::map<string, GLuint>::iterator iter;
	std::map<string, GLuint>::const_iterator end;
	
	delete [] ftabData;
	delete [] ftabRGB;
	delete [] ftabTexCoord;
	delete [] uitabTexName;

// Delete display lists
	if (glIsList( this->uiOpaqueList ))
		glDeleteLists( this->uiOpaqueList, 1 );
	if (glIsList( this->uiAlphaList ))
		glDeleteLists( this->uiAlphaList, 1 );

// Delete texture
	end = this->mapTexture.end();
	for (iter = this->mapTexture.begin(); iter != end; iter++)
		glDeleteTextures( 1, &(iter->second) );
}


   /*=====================================================================*/
void
Model::Display(
	const OC_FLOAT & rcfW,
	const OC_FLOAT & rcfH,
	const OC_FLOAT & rcfY ) const
{
	uiTabIndex = 0;
	ftabTemp = ftabData;

   // FIXME: this is ugly
	glBegin( GL_QUADS );

   // go through the data table
   // and display the correct primitive
	while (uiTabIndex < uiTabSize) {
		if (ftabTemp[uiTabIndex]
		==  OC_OCM_VERTEX) {
			glVertex3f(
				ftabTemp[uiTabIndex+1] + rcfW,
				ftabTemp[uiTabIndex+2] + rcfY,
				ftabTemp[uiTabIndex+3] + rcfH );
			uiTabIndex += 4;
//debug cout << "vertex" << endl;
		} else
		if (ftabTemp[uiTabIndex]
		== OC_OCM_COLOR) {
			glColor4fv( ftabTemp + uiTabIndex + 1 );
			uiTabIndex += 5;
//debug cout << "color" << endl;
		} else
		if (ftabTemp[uiTabIndex]
		==  OC_OCM_TEXCOOR) {
			glTexCoord3fv( ftabTemp + uiTabIndex + 1 );
			uiTabIndex += 4;
		} else
		if (ftabTemp[uiTabIndex]
		== OC_OCM_TEXBIND) {
		   //TODO: this is REALLY dirty
			glEnd();
			if ( (GLuint)ftabTemp[uiTabIndex+1] == 0 ) {
			   // restore the current rendering context
				glTexCoord4i( 0, 0, 0, 1 );
				glDisable( GL_TEXTURE_2D );
			   // NOTE: works around the ATI 9M bug
				glColor4ub( 255, 255, 255, 255 );
				glEnable( GL_COLOR_MATERIAL );
				glEnable( GL_LIGHTING );
				glDisable( GL_BLEND );
			}
			else {
			   // NOTE: works around the ATI 9M bug
				glDisable( GL_LIGHTING );
				glDisable( GL_COLOR_MATERIAL );

			   // enable alpha blending
				glEnable( GL_BLEND );
				glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

			   // enable texturing
				glEnable( GL_TEXTURE_2D );
				glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
				glBindTexture( GL_TEXTURE_2D, (GLuint)ftabTemp[uiTabIndex+1] );
//debug cout << "Binding texture id: " << (GLuint)ftabTemp[uiTabIndex+1] << endl;
			}
			glBegin( GL_QUADS );
			uiTabIndex += 2;
		}
		else {
			OPENCITY_DEBUG("WARNING: unknown OCM code");
			assert(0);			// Abort in under debuging mode
			uiTabIndex++;		// Avoid infinite loop in released code
		}
	}

	glEnd();
}


   /*=====================================================================*/
void
Model::Display2(
	const OC_FLOAT & rcfW,
	const OC_FLOAT & rcfH,
	const OC_BYTE tabY [] ) const
{
	uiTabIndex = 0;
	uiYIndex = 0;
	ftabTemp = ftabData;

   // FIXME: this is ugly
	glBegin( GL_QUADS );

   // go through the data table
   // and display the correct primitive
	while (uiTabIndex < uiTabSize) {
		if (ftabTemp[uiTabIndex]
		==  OC_OCM_VERTEX) {
			glVertex3f(
				ftabTemp[uiTabIndex+1] + rcfW,
				ftabTemp[uiTabIndex+2] + tabY[uiYIndex],
				ftabTemp[uiTabIndex+3] + rcfH );
			uiTabIndex += 4;
			uiYIndex++;
//debug cout << "vertex" << endl;
//debug cout << "tabYH : " << (int)tabY[uiYIndex] << endl;
		} else
		if (ftabTemp[uiTabIndex]
		== OC_OCM_COLOR) {
			glColor4fv( ftabTemp + uiTabIndex + 1 );
			uiTabIndex += 5;
//debug cout << "color" << endl;
		} else
		if (ftabTemp[uiTabIndex]
		==  OC_OCM_TEXCOOR) {
			glTexCoord3fv( ftabTemp + uiTabIndex + 1 );
			uiTabIndex += 4;
		} else
		if (ftabTemp[uiTabIndex]
		== OC_OCM_TEXBIND) {
		   //TODO: this is REALLY dirty
			glEnd();
			if ( (GLuint)ftabTemp[uiTabIndex+1] == 0 ) {
			   // restore the current rendering context
				glTexCoord4i( 0, 0, 0, 1 );
				glDisable( GL_TEXTURE_2D );
			   // NOTE: works around the ATI 9M bug
				glColor4ub( 255, 255, 255, 255 );
				glEnable( GL_COLOR_MATERIAL );
				glEnable( GL_LIGHTING );
				glDisable( GL_BLEND );
			}
			else {
			   // NOTE: works around the ATI 9M bug
				glDisable( GL_LIGHTING );
				glDisable( GL_COLOR_MATERIAL );

			   // enable alpha blending
				glEnable( GL_BLEND );
				glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

			   // enable texturing
				glEnable( GL_TEXTURE_2D );
				glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
				glBindTexture( GL_TEXTURE_2D, (GLuint)ftabTemp[uiTabIndex+1] );
//debug cout << "Binding texture id: " << (GLuint)ftabTemp[uiTabIndex+1] << endl;
			}
			glBegin( GL_QUADS );
			uiTabIndex += 2;
		}
		else {
			OPENCITY_DEBUG("WARNING: unknown OCM code");
		}
	}

	glEnd();
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
	assert( this->uiOpaqueList != 0 );
	glCallList( this->uiOpaqueList );
	if (glIsList( this->uiAlphaList ))
		glCallList( this->uiAlphaList );
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
	assert( this->uiOpaqueList != 0 );

// Call the opaque list first, then the alpha list
//	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glTranslatef( rcfW, tabY[0], rcfL );
	glCallList( this->uiOpaqueList );
	if (glIsList( this->uiAlphaList ))
		glCallList( this->uiAlphaList );
	glPopMatrix();
}

























