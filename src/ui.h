/***************************************************************************
                          ui.h  -  description
          $Id: ui.h,v 1.5 2005/05/21 06:20:31 neoneurone Exp $
                             -------------------
    begin                : ven jun 6 2003
    copyright            : (C) 2003 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_UI_H_
	#define _OPENCITY_UI_H_

	#include "macros.h"

	#include "SDL.h"

   //========================================================================
   /** This is the base class for all other User Interface classes. It
       offers an interface that the derived class must implement in order
       to handle users' inputs
   */
class UI {
public:
	UI();
	virtual ~UI();


	virtual void
	uiKeyboard( const SDL_KeyboardEvent & rcsSDLKeyboardEvent ) = 0;


	virtual void
	uiMouseMotion( const SDL_MouseMotionEvent & rcsSDLMouseMotionEvent ) = 0;


	virtual void
	uiMouseButton( const SDL_MouseButtonEvent & rcsSDLMouseButtonEvent ) = 0;


	virtual void
	uiExpose( const SDL_ExposeEvent & rcsSDLExposeEvent ) = 0;


	virtual void
	uiResize( const SDL_ResizeEvent & rcsSDLResizeEvent )= 0;

};
#endif





































