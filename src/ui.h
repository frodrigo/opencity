/***************************************************************************
						ui.h  -  description
							-------------------
	begin                : june 6th, 2003
	copyright            : (C) 2003 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_UI_H_
#define _OPENCITY_UI_H_ 1

#include "macros.h"
#include "SDL.h"

//========================================================================
/** This is the base class for all other User Interface classes. It
offers an interface that the derived class must implement in order
to deal with the user's inputs
*/
class UI {
public:
	UI();
	virtual ~UI();


//========================================================================
/** This method handles the keyboard events such as: key down or up
	\param rcsEvent The SDL keyboard event
*/
	virtual void
	Keyboard( const SDL_KeyboardEvent& rcEvent ) = 0;


//========================================================================
/** This method handles the mouse motion event
	\param rcsEvent The SDL mouse motion event
*/
	virtual void
	MouseMotion( const SDL_MouseMotionEvent& rcEvent ) = 0;


//========================================================================
/** This method handles the mouse action event such as: mouse button
clicks or releases
	\param rcsEvent The SDL mouse action event
*/
	virtual void
	MouseButton( const SDL_MouseButtonEvent& rcEvent ) = 0;


//========================================================================
/** The expose event is received when the display needs to be updated.
	\param rcsEvent The SDL expose event
*/
	virtual void
	Expose( const SDL_ExposeEvent& rcEvent ) = 0;


//========================================================================
/** The resize event is received when display window is resized
	\param rcsEvent The SDL resize
*/
	virtual void
	Resize( const SDL_ResizeEvent& rcEvent )= 0;

};
#endif





































