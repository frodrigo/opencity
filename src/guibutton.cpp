/***************************************************************************
						guibutton.cpp    -  description
							-------------------
	begin                : march 22th, 2004
	copyright            : (C) 2004-2015 by Duong Khang NGUYEN
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

// OpenCity headers
#include "guibutton.h"
#include "guicontainer.h"


   /*=====================================================================*/
GUIButton::GUIButton():
_uiNumberState(0)
{
	OPENCITY_DEBUG( "Dctor" );
}


   /*=====================================================================*/
GUIButton::GUIButton
(
	const int& rciX,
	const int& rciY,
	const uint& rcuiW,
	const uint& rcuiH,
	const string& strFile,
	uint numberState
):
_uiNumberState(numberState)
{
	OPENCITY_DEBUG( "Pctor" );

// Safe
	_pctr = NULL;

// Initialize the position of the button
// NOTE: it's relative to the postion of the container
	_iX = rciX;
	_iY = rciY;

// The GUI button's W,H may be smaller than the image's W,H
	_uiWidth = rcuiW;
	_uiHeight = rcuiH;

// Load the texture from the image
	moTextureNormal = Texture( strFile + ".png" );
	if (_uiNumberState == 2) {
		moTextureOver = Texture( strFile + "_over.png" );
	}

// Set the default colors
	_cForeground = OPENCITY_PALETTE[ Color::OC_BLACK ];
	_cBackground = OPENCITY_PALETTE[ Color::OC_BLACK ];

// By default the button is visible and displayed with alpha blending
	Set( OC_GUIMAIN_VISIBLE | OC_GUIMAIN_BLENDING );
}


   /*=====================================================================*/
GUIButton::~GUIButton()
{
	OPENCITY_DEBUG( "Dtor" );
}


   /*=====================================================================*/
void
GUIButton::SetBackground( const Color& color )
{
	_cBackground = color;
}


   /*=====================================================================*/
void
GUIButton::SetForeground( const Color& color )
{
	_cForeground = color;
}


   /*=====================================================================*/
void
GUIButton::Display() const
{
// Return immediatly if this is NOT visible
	if ( IsSet( OC_GUIMAIN_VISIBLE ) == false )
		return;

// IF there is nothing to display THEN return
	if (!glIsTexture( moTextureNormal.GetName() ))
		return;

// Translate the button to the correct position
	glPushAttrib( GL_ENABLE_BIT );
	glDisable( GL_LIGHTING );
	glPushMatrix();
	glTranslatef( _iX, _iY, 0.0 );

// Activate the texture 2D processing
	glEnable( GL_TEXTURE_2D );

	if ( IsSet( OC_GUIMAIN_BLENDING ) == true ) {
	// Activate the alpha blending
		glEnable( GL_BLEND );
//		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );	// Already choosen
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	}
	else {
		glColor4ub(
			_cBackground.r,
			_cBackground.g,
			_cBackground.b,
			_cBackground.a );
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
	}

// Select the appropriate texture
// IF the mouse is over THEN choose the over texture
// ELSE, use the normal texture
	if ( IsSet(OC_GUIMAIN_MOUSEOVER) && _uiNumberState > 1 )
		glBindTexture( GL_TEXTURE_2D, moTextureOver.GetName() );
	else
		glBindTexture( GL_TEXTURE_2D, moTextureNormal.GetName() );

	glBegin( GL_QUADS );
	glTexCoord2i( 0, 0 );	glVertex2i( 0, 0 );
	glTexCoord2i( 1, 0 );	glVertex2i( _uiWidth-1, 0 );
	glTexCoord2i( 1, 1 );	glVertex2i( _uiWidth-1, _uiHeight-1 );
	glTexCoord2i( 0, 1 );	glVertex2i( 0, _uiHeight-1 );
	glEnd();

// Restore the old matrix and attribs
	glPopMatrix();
	glPopAttrib();
}


   /*=====================================================================*/
void
GUIButton::Keyboard( const SDL_KeyboardEvent& rcEvent )
{}


   /*=====================================================================*/
void
GUIButton::MouseMotion( const SDL_MouseMotionEvent& rcsMouseEvent )
{
	static int realX, realY;
	static int winW, winH;
	static int myX, myY;

/* Always calculate the mouse over state.
// Return immediatly if this is NOT visible
	if ( !IsSet( OC_GUIMAIN_VISIBLE ) )
		return;
*/

// The button must be contained in a guicontainer !
	assert( _pctr != NULL );

// Calculate the real X,Y from the mouse X,Y;
	((GUIContainer*)_pctr)->GetWinWH( winW, winH );
	realX = rcsMouseEvent.x;
	realY = winH - rcsMouseEvent.y - 1;

// Calculate my absolute coordinates via the container
	_pctr->GetLocation( myX, myY );
	myX += _iX;
	myY += _iY;

//debug
//cout << "realX: " << realX << " / realY: " << realY << endl;

   // if the mouse position is within the range, turn on boolMouseOver
	if ( (realX >= myX) && (realX <= (int)(myX + _uiWidth))
	  && (realY >= myY) && (realY <= (int)(myY + _uiHeight))) {
		Set( OC_GUIMAIN_MOUSEOVER );
	}
	else {
		Unset( OC_GUIMAIN_MOUSEOVER | OC_GUIMAIN_CLICKED );
	}
}


   /*=====================================================================*/
void
GUIButton::MouseButton( const SDL_MouseButtonEvent& buttonEvent )
{
// Return immediatly if this is NOT visible
	if ( IsSet( OC_GUIMAIN_VISIBLE ) == false )
		return;

// IF the user clicked the LMB when the mouse is over the control
// THEN turn on the onClick state
// otherwise turn it off
	if ( buttonEvent.state == SDL_PRESSED ) {
		if ( buttonEvent.button == SDL_BUTTON_LEFT ) {
			if ( IsSet( OC_GUIMAIN_MOUSEOVER ) == true )
				Set( OC_GUIMAIN_CLICKED );
			else
				Unset( OC_GUIMAIN_CLICKED );
		}
	}
}


   /*=====================================================================*/
void
GUIButton::Expose( const SDL_ExposeEvent& rcEvent )
{
	this->Display();
}


   /*=====================================================================*/
void
GUIButton::Resize( const SDL_ResizeEvent& rcEvent )
{
}
































