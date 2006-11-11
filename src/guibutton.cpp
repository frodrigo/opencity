/***************************************************************************
						guibutton.cpp    -  description
							-------------------
	begin                : lun 22 mar 2004
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

#include "guibutton.h"
#include "guicontainer.h"

#include "texture.h"		// textures loading

   /*=====================================================================*/
GUIButton::GUIButton(
	const int & rciX,
	const int & rciY,
	const uint & rcuiW,
	const uint & rcuiH,
	const string & strFile )
{
	OPENCITY_DEBUG( "New GUI button with image" );


	this->pguicontainer = NULL;

   // initialize the position of the button
   // note: it's relative to the postion of the container
	iX = rciX;
	iY = rciY;

   // the GUI button's W,H may be smaller than the image's W,H
	this->uiWidth = rcuiW;
	this->uiHeight = rcuiH;

   // load the texture from the image
	this->uiTexture = Texture::Load( strFile + ".png" );
	this->uiTextureOver = Texture::Load( strFile + "_over.png" );

   // set the default colors
	this->colorBackground.r = 0;		//black
	this->colorBackground.g = 0;
	this->colorBackground.b = 0;
	this->colorBackground.a = 255;

	this->colorForeground.r = 0;		//black
	this->colorForeground.g = 0;
	this->colorForeground.b = 0;
	this->colorForeground.a = 255;

   // By default the button is visible and displayed with alpha blending
	Set( OC_GUIMAIN_VISIBLE | OC_GUIMAIN_BLENDING );
}


   /*=====================================================================*/
GUIButton::~GUIButton()
{
	OPENCITY_DEBUG( "GUI button deleted" );

// Free the associated texture if there is one
	if (glIsTexture( this->uiTexture ) == GL_TRUE) {
		glDeleteTextures( 1, &this->uiTexture );
	}

	if (glIsTexture( this->uiTextureOver ) == GL_TRUE) {
		glDeleteTextures( 1, &this->uiTextureOver );
	}
}


   /*=====================================================================*/
void
GUIButton::SetBackground(
	const Color & color )
{
	this->colorBackground.r = color.r;
	this->colorBackground.g = color.g;
	this->colorBackground.b = color.b;
	this->colorBackground.a = color.a;
}


   /*=====================================================================*/
void
GUIButton::SetForeground(
	const Color & color )
{
	this->colorForeground.r = color.r;
	this->colorForeground.g = color.g;
	this->colorForeground.b = color.b;
	this->colorForeground.a = color.a;
}


   /*=====================================================================*/
void
GUIButton::Display() const
{
// Return immediatly if this is NOT visible
	if ( IsSet( OC_GUIMAIN_VISIBLE ) == false )
		return;

// Translate the button to the correct position
	glPushAttrib( GL_ENABLE_BIT );
	glDisable( GL_LIGHTING );
	glPushMatrix();
	glTranslatef( this->iX, this->iY, 0.0 );

	if (glIsTexture( this->uiTexture ) == GL_TRUE ) {
	   // activate the texture 2D processing
		glEnable( GL_TEXTURE_2D );

		if ( IsSet( OC_GUIMAIN_BLENDING ) == true ) {
		   // activate the alpha blending
			glEnable( GL_BLEND );
//			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );	// Already choosen
			glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

//test
//			glBlendFunc( GL_ZERO, GL_ONE );
//			glBlendColor( 1., 0.5, 1., 0. );
//test
//			glTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, env_color );
		}
		else {
			glColor4ub(
				colorBackground.r,
				colorBackground.g,
				colorBackground.b,
				colorBackground.a );
			glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
		}

	// Select the appropriate texture
	// IF the mouse is over THEN choose the over texture
	// ELSE, use the normal texture
		if ( IsSet( OC_GUIMAIN_MOUSEOVER ) == true )
			glBindTexture( GL_TEXTURE_2D, this->uiTextureOver );
		else
			glBindTexture( GL_TEXTURE_2D, this->uiTexture );

		glBegin( GL_QUADS );
		glTexCoord2i( 0, 0 );	glVertex2i( 1, 1 );
		glTexCoord2i( 1, 0 );	glVertex2i( this->uiWidth, 1 );
		glTexCoord2i( 1, 1 );	glVertex2i( this->uiWidth, this->uiHeight );
		glTexCoord2i( 0, 1 );	glVertex2i( 1, this->uiHeight );
		glEnd();
	}

// Restore the old matrix and attribs
	glPopMatrix();
	glPopAttrib();
}


   /*=====================================================================*/
void
GUIButton::uiKeyboard( const SDL_KeyboardEvent & rcsSDLKeyboardEvent )
{}


   /*=====================================================================*/
void
GUIButton::uiMouseMotion( const SDL_MouseMotionEvent & rcsMouseEvent )
{
	static int realX, realY;
	static int winW, winH;
	static int myX, myY;

// Return immediatly if this is NOT visible
	if ( IsSet( OC_GUIMAIN_VISIBLE ) == false )
		return;

// The button must be contained in a guicontainer !
	assert( pguicontainer != NULL );

// Calculate the real X,Y from the mouse X,Y;
	((GUIContainer*)pguicontainer)->GetWinWH( winW, winH );
	realX = rcsMouseEvent.x;
	realY = winH - rcsMouseEvent.y - 1;

// Calculate my absolute coordinates via the container
	pguicontainer->GetLocation( myX, myY );
	myX += this->iX;
	myY += this->iY;

//debug
//cout << "realX: " << realX << " / realY: " << realY << endl;

   // if the mouse position is within the range, turn on boolMouseOver
	if ( (realX >= myX) && (realX <= (int)(myX + uiWidth))
	  && (realY >= myY) && (realY <= (int)(myY + uiHeight))) {
		Set( OC_GUIMAIN_MOUSEOVER );
	}
	else {
		Unset( OC_GUIMAIN_MOUSEOVER | OC_GUIMAIN_CLICKED );
	}
}


   /*=====================================================================*/
void
GUIButton::uiMouseButton( const SDL_MouseButtonEvent & buttonEvent )
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
GUIButton::uiExpose( const SDL_ExposeEvent & rcsSDLExposeEvent )
{
	this->Display();
}


   /*=====================================================================*/
void
GUIButton::uiResize( const SDL_ResizeEvent & rcsSDLResizeEvent )
{
}
































