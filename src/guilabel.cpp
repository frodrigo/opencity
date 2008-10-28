/***************************************************************************
						guilabel.cpp   -   description
							-------------------
	begin                : december 10th, 2006
	copyright            : (C) 2006-2008 by Duong Khang NGUYEN
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

#include "guilabel.h"
#include "guicontainer.h"
#include "font_8x8.h"					// 8x8 font definition


uint GUILabel::_uiLabelNumber = 0;
GLuint GUILabel::_uiFontBase = 0;


   /*=====================================================================*/
GUILabel::GUILabel(
	const int ciX,
	const int ciY,
	const string & rcsText ):
_eAlign( OC_ALIGN_LEFT ),
_sText( rcsText )
{
	OPENCITY_DEBUG( "Pctor 1" );

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
		_CreateFont();
	}
}


   /*=====================================================================*/
GUILabel::GUILabel(
	const int ciX,
	const int ciY,
	const uint cuiW,
	const uint cuiH,
	const string& rcsText )
{
	OPENCITY_DEBUG( "Pctor 2" );

// Safe
	_pctr = NULL;

// Initialize the position of the button
// NOTE: it's relative to the postion of the container
	_iX = ciX;
	_iY = ciY;

// Initialize the width and the height of the controls.
// They are used for text alignment.
	_uiWidth = cuiW;
	_uiHeight = cuiH;

	_cForeground.r = 0;		//black
	_cForeground.g = 0;
	_cForeground.b = 0;
	_cForeground.a = 255;

// By default the label is visible
	Set( OC_GUIMAIN_VISIBLE );

// Create the 8x8 font once
	if (GUILabel::_uiLabelNumber++ == 0) {
		_CreateFont();
	}
}


   /*=====================================================================*/
GUILabel::~GUILabel()
{
	OPENCITY_DEBUG( "Dtor" );

// Destroy GL list
	if (--GUILabel::_uiLabelNumber == 0) {
		_DeleteFont();
	}
}


   /*=====================================================================*/
void
GUILabel::SetAlign( const OPENCITY_TEXT_ALIGN align )
{
	_eAlign = align;
}


   /*=====================================================================*/
GUILabel::OPENCITY_TEXT_ALIGN
GUILabel::GetAlign() const
{
	return _eAlign;
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
	uint x, y;

	assert( _pctr != NULL );

// Return immediatly if this is NOT visible
	if ( IsSet( OC_GUIMAIN_VISIBLE ) == false )
		return;

// Calculate the 2D position according to the alignment
	x = _iX;
	y = _iY;
	if ( _eAlign == OC_ALIGN_RIGHT ) {
		x += _uiWidth - FONT_8x8_X_MOVE*_sText.size();
	} else
	if ( _eAlign == OC_ALIGN_CENTER ) {
		x += (_uiWidth - FONT_8x8_X_MOVE*_sText.size()) / 2;
	}

// Save the current attribs
	glPushAttrib( GL_LIST_BIT | GL_ENABLE_BIT );
	glDisable( GL_LIGHTING );

// Save the list base
	glListBase( GUILabel::_uiFontBase );
	glColor4ub( _cForeground.r, _cForeground.g, _cForeground.b, _cForeground.a );
	glRasterPos2i( x, y );
	glCallLists( (GLsizei)_sText.size(), GL_UNSIGNED_BYTE, (GLubyte*)_sText.c_str() );

// Restore the old attribs
	glPopAttrib();
}


   /*=====================================================================*/
void
GUILabel::Keyboard( const SDL_KeyboardEvent& rcEvent )
{}


   /*=====================================================================*/
void
GUILabel::MouseMotion( const SDL_MouseMotionEvent& rcEvent )
{}


   /*=====================================================================*/
void
GUILabel::MouseButton( const SDL_MouseButtonEvent& rcEvent )
{}


   /*=====================================================================*/
void
GUILabel::Expose( const SDL_ExposeEvent& rcEvent )
{
	this->Display();
}


   /*=====================================================================*/
void
GUILabel::Resize( const SDL_ResizeEvent& rcEvent )
{}


   /*=====================================================================*/
void
GUILabel::_CreateFont()
{
// Save the current unpack alignment value
	GLint alignment;
	glGetIntegerv( GL_UNPACK_ALIGNMENT, &alignment );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	GUILabel::_uiFontBase = glGenLists( 256 );
	for ( uint i = 0; i < 256; i++ ) {
		glNewList( GUILabel::_uiFontBase + i, GL_COMPILE );
		glBitmap(
			FONT_8x8_WIDTH, FONT_8x8_HEIGHT,
			.0, .0,
			FONT_8x8_X_MOVE, FONT_8x8_Y_MOVE,
			fontdata_8x8 + i*8
		);
		glEndList();
	}

// Restore the previous unpack alignment value
	glPixelStorei( GL_UNPACK_ALIGNMENT, alignment );
}


   /*=====================================================================*/
void
GUILabel::_DeleteFont()
{
	if (glIsTexture( GUILabel::_uiFontBase ) == GL_TRUE) {
		glDeleteLists( GUILabel::_uiFontBase, 256 );
		GUILabel::_uiFontBase = 0;		// Safe
	}
}





























