/***************************************************************************
						guilabel.cpp    -  description
							-------------------
	begin                : december 10th, 2006
	copyright            : (C) 2006 by Duong-Khang NGUYEN
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

#include "guilabel.h"
#include "guicontainer.h"
#include "font_8x8.h"					// 8x8 font definition


uint GUILabel::_uiLabelNumber = 0;
GLuint GUILabel::_uiFontBase = 0;


   /*=====================================================================*/
GUILabel::GUILabel()
{
	OPENCITY_DEBUG( "Default ctor. Not used" );
	assert(0);

	_iX = 0;
	_iY = 0;
}


   /*=====================================================================*/
GUILabel::GUILabel(
	const int ciX,
	const int ciY,
	const string & rcsText ):
_sText( rcsText )
{
	OPENCITY_DEBUG( "Pctor" );

// Safe
	_pctr = NULL;

// Initialize the position of the button
// NOTE: it's relative to the postion of the container
	_iX = ciX;
	_iY = ciY;

	_cForeground.r = 0;		//black
	_cForeground.g = 0;
	_cForeground.b = 0;
	_cForeground.a = 255;

// By default the label is visible
	Set( OC_GUIMAIN_VISIBLE );

// Create the 8x8 font once
	if (GUILabel::_uiLabelNumber++ == 0) {
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		GUILabel::_uiFontBase = glGenLists( 256 );
		for ( uint i = 0; i < 256; i++ ) {
			glNewList( GUILabel::_uiFontBase + i, GL_COMPILE );
			glBitmap( 8, 8, .0, .0, 10., .0, fontdata_8x8 + i*8 );
			glEndList();
		}
	}
}


   /*=====================================================================*/
GUILabel::~GUILabel()
{
	OPENCITY_DEBUG( "Dtor" );

// Destroy GL list
	if (--GUILabel::_uiLabelNumber == 0) {
		if (glIsTexture( GUILabel::_uiFontBase ) == GL_TRUE) {
			glDeleteLists( GUILabel::_uiFontBase, 256 );
			GUILabel::_uiFontBase = 0;		// Safe
		}
	}
}


   /*=====================================================================*/
void
GUILabel::SetText( const string& rcsText )
{
	_sText = rcsText;
}


   /*=====================================================================*/
string
GUILabel::GetText() const
{
	return _sText;
}


   /*=====================================================================*/
void
GUILabel::SetForeground(
	const Color & color )
{
	_cForeground.r = color.r;
	_cForeground.g = color.g;
	_cForeground.b = color.b;
	_cForeground.a = color.a;
}


   /*=====================================================================*/
void
GUILabel::Display() const
{
	assert( _pctr != NULL );

// Return immediatly if this is NOT visible
	if ( IsSet( OC_GUIMAIN_VISIBLE ) == false )
		return;

// Save the current attribs
	glPushAttrib( GL_LIST_BIT | GL_ENABLE_BIT );
	glDisable( GL_LIGHTING );

// Save the list base
	glListBase( GUILabel::_uiFontBase );
	glColor4ub( _cForeground.r, _cForeground.g, _cForeground.b, _cForeground.a );
	glRasterPos2i( _iX, _iY );
	glCallLists( (GLsizei)_sText.size(), GL_UNSIGNED_BYTE, (GLubyte*)_sText.c_str() );

// Restore the old attribs
	glPopAttrib();
}


   /*=====================================================================*/
void
GUILabel::uiKeyboard( const SDL_KeyboardEvent & rcsSDLKeyboardEvent )
{}


   /*=====================================================================*/
void
GUILabel::uiMouseMotion( const SDL_MouseMotionEvent & rcsMouseEvent )
{}


   /*=====================================================================*/
void
GUILabel::uiMouseButton( const SDL_MouseButtonEvent & buttonEvent )
{}


   /*=====================================================================*/
void
GUILabel::uiExpose( const SDL_ExposeEvent & rcsSDLExposeEvent )
{
	this->Display();
}


   /*=====================================================================*/
void
GUILabel::uiResize( const SDL_ResizeEvent & rcsSDLResizeEvent )
{}
































