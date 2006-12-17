/***************************************************************************
						guibutton.h    -  description
							-------------------
	begin                : march 22th, 2004
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

#ifndef _OPENCITY_GUIBUTTON_H_
#define _OPENCITY_GUIBUTTON_H_ 1

#include "guimain.h"


//========================================================================
/** This is a 2-state button: onMouseOver and onMouseOut.
*/
class GUIButton : public GUIMain {
public:
	GUIButton() {};
	GUIButton(
		const int & rciX,
		const int & rciY,
		const uint & rcuiW,
		const uint & rcuiH,
		const string & strFile );
	~GUIButton();


	void
	SetBackground(
		const Color& color );

	void
	SetForeground(
		const Color& color );


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
	GLuint _uiTexNormal, _uiTexOver;

	Color _cForeground;				///< Foreground color
	Color _cBackground;				///< Background color
};

#endif































