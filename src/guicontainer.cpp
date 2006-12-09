/***************************************************************************
						guicontainer.cpp    -  description
							-------------------
	begin                : mar 22th, 2004
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

#include "guicontainer.h"

#include "texture.h"

GLuint GUIContainer::_uiTexture = 0;
GLuint GUIContainer::_uiNumberContainer = 0;

   /*=====================================================================*/
GUIContainer::GUIContainer(
	const int & rciX,
	const int & rciY,
	const uint & rcuiW,
	const uint & rcuiH )
{
	OPENCITY_DEBUG( "New GUI container" );

	SDL_Surface* pScreen = SDL_GetVideoSurface();
	this->uiWinWidth = pScreen->w;
	this->uiWinHeight = pScreen->h;

	this->iX = rciX;
	this->iY = rciY;
	this->uiWidth = rcuiW;
	this->uiHeight = rcuiH;

	++GUIContainer::_uiNumberContainer;
// Not used yet
// Load the background once
//	if (GUIContainer::_uiNumberContainer++ == 0)
//		GUIContainer::_uiTexture = Texture::Load( ocHomeDirPrefix(OC_GUICONTAINER_BG) );
}


   /*=====================================================================*/
GUIContainer::~GUIContainer()
{
	OPENCITY_DEBUG( "GUI container deleted" );

	std::vector<GUIMain*>::size_type stvector;
	GUIMain* pguimain;

//--- now remove the container pointer from all the contained controls
	for ( stvector = 0; stvector < this->vectorpguimain.size(); stvector++ ) {
		if ( (pguimain = vectorpguimain[ stvector ]) != NULL )
			pguimain->SetContainer( NULL );
	}

// Delete all the pointers in the "vector"
// Note: the pointed memory is not freed
	this->vectorpguimain.clear();

// Delete the background if this is the last container
	if (GUIContainer::_uiNumberContainer-- == 1) {
	// Free the associated texture if there is one
		if (glIsTexture( GUIContainer::_uiTexture ) == GL_TRUE) {
			glDeleteTextures( 1, &GUIContainer::_uiTexture );
			GUIContainer::_uiTexture = 0;
		}
	}
}


   /*=====================================================================*/
const uint
GUIContainer::Add(
	GUIMain* const pguimain )
{
// add the new pointer to the end
	pguimain->SetContainer( this );
	this->vectorpguimain.push_back( pguimain );

// get its index for future use
	return this->vectorpguimain.size();
}


   /*=====================================================================*/
const uint
GUIContainer::GetNumber() const
{
	return this->vectorpguimain.size();
}


   /*=====================================================================*/
void
GUIContainer::GetWinWH(
	int & riWinW,
	int & riWinH ) const
{
	riWinW = this->uiWinWidth;
	riWinH = this->uiWinHeight;
}


   /*=====================================================================*/
const uint
GUIContainer::GetClick() const
{
	static std::vector<GUIMain*>::size_type stvector;
	static GUIMain* pguimain;

// We return immediatly if this is NOT visible
	if ( IsSet( OC_GUIMAIN_VISIBLE ) == false )
		return 0;

// FOR each object DO
// IF we find a object which is clicked and has mouseover THEN
	for ( stvector = 0; stvector < this->vectorpguimain.size(); stvector++ ) {
		if ( (pguimain = vectorpguimain[ stvector ]) != NULL )
		if ( pguimain->IsSet( OC_GUIMAIN_CLICKED | OC_GUIMAIN_MOUSEOVER ) == true )
			return ++stvector;
	}

// There's no clicked object
	return 0;
}


   /*=====================================================================*/
void
GUIContainer::Set(
	const uint & rcuiIndex,
	const OC_UBYTE & rcubAttribute ) const
{
	assert( rcuiIndex <= vectorpguimain.size() );

	vectorpguimain[ rcuiIndex-1 ]->Set( rcubAttribute );
}


   /*=====================================================================*/
void
GUIContainer::ResetAttribute(
	const OC_UBYTE & rcubAttr ) const
{
	std::vector<GUIMain*>::size_type stvector;
	GUIMain* pguimain;

// Reset the attributes of all contained objects
	for ( stvector = 0; stvector < this->vectorpguimain.size(); stvector++ ) {
		if ( (pguimain = vectorpguimain[ stvector ]) != NULL )
			pguimain->Unset( rcubAttr );
	}
}


   /*=====================================================================*/
void
GUIContainer::Display() const
{
	static std::vector<GUIMain*>::size_type stvector;
	static GUIMain* pguimain;

// We return immediatly if this is NOT visible
	if ( IsSet( OC_GUIMAIN_VISIBLE ) == false )
		return;

// Save the old projection matrix before processing
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
// Use the orthonormal projection
	gluOrtho2D( 0, uiWinWidth-1, 0, uiWinHeight-1 );

// Store the old matrix and move the container to the correct position
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	glTranslatef( iX, iY, 0.0 );

// Display the background
/* not used yet
	glPushAttrib( GL_ENABLE_BIT );
	glEnable( GL_BLEND );
	glEnable( GL_TEXTURE_2D );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glBindTexture( GL_TEXTURE_2D, GUIContainer::_uiTexture );

	glBegin( GL_QUADS );
	glTexCoord2i( 0, 0 );	glVertex2i( 1, 1 );
	glTexCoord2i( 1, 0 );	glVertex2i( this->uiWidth, 1 );
	glTexCoord2i( 1, 1 );	glVertex2i( this->uiWidth, this->uiHeight );
	glTexCoord2i( 0, 1 );	glVertex2i( 1, this->uiHeight );
	glEnd();

	glPopAttrib();
*/

// Now display all the contained GUI controls
	for ( stvector = 0; stvector < this->vectorpguimain.size(); stvector++ ) {
		if ( (pguimain = vectorpguimain[ stvector ]) != NULL )
			pguimain->Display();
	}

	glPopMatrix();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
}


   /*=====================================================================*/
void
GUIContainer::uiKeyboard( const SDL_KeyboardEvent & rcsSDLKeyboardEvent )
{}


   /*=====================================================================*/
void
GUIContainer::uiMouseMotion( const SDL_MouseMotionEvent & rcsSDLMouseMotionEvent )
{
	static std::vector<GUIMain*>::size_type stvector;
	static GUIMain* pguimain;

// We return immediatly if this is NOT visible
	if ( IsSet( OC_GUIMAIN_VISIBLE ) == false )
		return;

// Send the motion event to all objects
	for ( stvector = 0; stvector < this->vectorpguimain.size(); stvector++ ) {
		if ( (pguimain = vectorpguimain[ stvector ]) != NULL )
			pguimain->uiMouseMotion( rcsSDLMouseMotionEvent );
	}
}


   /*=====================================================================*/
void
GUIContainer::uiMouseButton( const SDL_MouseButtonEvent & rcsSDLMouseButtonEvent )
{
	static std::vector<GUIMain*>::size_type stvector;
	static GUIMain* pguimain;

// We return immediatly if this is NOT visible
	if ( IsSet( OC_GUIMAIN_VISIBLE ) == false )
		return;

// Send the mouse button event to all objects
	for ( stvector = 0; stvector < this->vectorpguimain.size(); stvector++ ) {
		if ( (pguimain = vectorpguimain[ stvector ]) != NULL )
			pguimain->uiMouseButton( rcsSDLMouseButtonEvent );
	}
}


   /*=====================================================================*/
void
GUIContainer::uiExpose( const SDL_ExposeEvent & rcsSDLExposeEvent )
{
	this->Display();
}


   /*=====================================================================*/
void
GUIContainer::uiResize( const SDL_ResizeEvent & rcsSDLResizeEvent )
{
	this->uiWinWidth = rcsSDLResizeEvent.w;
	this->uiWinHeight = rcsSDLResizeEvent.h;
}



































