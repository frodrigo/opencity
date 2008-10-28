/***************************************************************************
						guibutton.h    -  description
							-------------------
	begin                : march 22th, 2004
	copyright            : (C) 2004-2008 by Duong Khang NGUYEN
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

#ifndef _OPENCITY_GUIBUTTON_H_
#define _OPENCITY_GUIBUTTON_H_ 1

#include "guimain.h"
#include "texture.h"

#define GUIBUTTON_POSITION_1		19, 73, 30, 30
#define GUIBUTTON_POSITION_2		19, 33, 30, 30
#define GUIBUTTON_POSITION_3		55, 13, 30, 30
#define GUIBUTTON_POSITION_4		91, 33, 30, 30
#define GUIBUTTON_POSITION_5		91, 73, 30, 30
#define GUIBUTTON_POSITION_6		55, 93, 30, 30


//========================================================================
/** One or two states button: onMouseOver and onMouseOut. The default
is a two state button
*/
class GUIButton : public GUIMain {
public:
	GUIButton();
	GUIButton(
		const int& rciX,
		const int& rciY,
		const uint& rcuiW,
		const uint& rcuiH,
		const string& strFile,
		uint numberState = 2 );
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
	uint		_uiNumberState;				///< Number of state (default is 2)
	Texture		moTextureNormal;			///< Inactive button texture
	Texture		moTextureOver;				///< On mouse over button texture

	Color		_cForeground;				///< Foreground color
	Color		_cBackground;				///< Background color
};

#endif































