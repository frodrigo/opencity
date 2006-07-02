/***************************************************************************
                          guicontainer.cpp    -  description
     $Id$
                             -------------------
    begin                : lun 22 mar 2004
    copyright            : (C) 2004-2005 by Duong-Khang NGUYEN
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

#include "guicontainer.h"


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

   // delete all the pointers in the "vector"
   // note: the pointed memory is not freed
	this->vectorpguimain.clear();
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


































