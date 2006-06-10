/***************************************************************************
                          guibutton.h    -  description
    $Id: guibutton.h,v 1.9 2006/03/12 23:14:18 neoneurone Exp $
                             -------------------
    begin                : lun 22 mar 2004
    copyright            : (C) 2004 by Duong-Khang NGUYEN
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

#if !defined(_OPENCITY_GUIBUTTON_H_)
	#define _OPENCITY_GUIBUTTON_H_ 1

	#include "main.h"
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
		const OPENCITY_COLOR & color );

	void
	SetForeground(
		const OPENCITY_COLOR & color );


   //========================================================================
   // Inherited methods from GUIMain
   //========================================================================
	void
	Display() const;


   //========================================================================
   // Inherited methods from UI
   //========================================================================
	void
	uiKeyboard( const SDL_KeyboardEvent & rcsSDLKeyboardEvent );

	void
	uiMouseMotion( const SDL_MouseMotionEvent & rcsSDLMouseMotionEvent );

	void
	uiMouseButton( const SDL_MouseButtonEvent & rcsSDLMouseButtonEvent );

	void
	uiExpose( const SDL_ExposeEvent & rcsSDLExposeEvent );

	void
	uiResize( const SDL_ResizeEvent & rcsSDLResizeEvent );


private:
	GLuint uiTexture, uiTextureOver;

	OPENCITY_COLOR colorForeground;
	OPENCITY_COLOR colorBackground;

};

#endif































