/***************************************************************************
						guilabel.h    -  description
							-------------------
	begin                : december, 10th 2006
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

#ifndef _OPENCITY_GUILABEL_H_
#define _OPENCITY_GUILABEL_H_ 1

#include "guimain.h"


//========================================================================
/** A label GUI control is used to display a static text in 2D
*/
class GUILabel : public GUIMain {
public:
	enum OPENCITY_TEXT_ALIGN {
		OC_ALIGN_LEFT,
		OC_ALIGN_RIGHT,
		OC_ALIGN_CENTER
	};


//========================================================================
/** Create a new label control. The default alignment is from the left
and it can not be changed.
	\param ciX,ciY The 2D coordinates of the control
	\param rcsText The text of the control
*/
	GUILabel(
		const int ciX,
		const int ciY,
		const string& rcsText );


//========================================================================
	GUILabel(
		const int ciX,
		const int ciY,
		const uint cuiW,
		const uint cuiH,
		const string& rcsText );


	~GUILabel();


//========================================================================
	void
	SetAlign( const OPENCITY_TEXT_ALIGN align );

	OPENCITY_TEXT_ALIGN
	GetAlign() const;

	void
	SetText( const string& rcsText );

	string
	GetText() const;

	void
	SetForeground( const Color& color );


//========================================================================
// Inherited methods from GUIMain
//========================================================================
	void
	Display() const;


//========================================================================
// Inherited methods from UI
//========================================================================
	void Keyboard( const SDL_KeyboardEvent& rcEvent );
	void MouseMotion( const SDL_MouseMotionEvent& rcEvent );
	void MouseButton( const SDL_MouseButtonEvent& rcEvent );
	void Expose( const SDL_ExposeEvent& rcEvent );
	void Resize( const SDL_ResizeEvent& rcEvent );


private:
	OPENCITY_TEXT_ALIGN	_eAlign;	///< The text alignment attribute
	string	_sText;					///< The text of the label
	Color	_cForeground;			///< The color of the label

	static uint		_uiLabelNumber;
	static GLuint	_uiFontBase;


//========================================================================
// Private methods
//========================================================================
	void _createFont();
	void _deleteFont();
};

#endif
































