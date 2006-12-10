/***************************************************************************
						guicontainer.h    -  description
							-------------------
	begin                : march, 22th 2004
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

#if !defined(_OPENCITY_GUICONTAINER_H_)
#define _OPENCITY_GUICONTAINER_H_ 1

#include "main.h"

#include "guimain.h"

#include <vector>

#define OC_GUICONTAINER_BG		"graphism/gui/toolcircle_bg.png"


//========================================================================
/** A container is a GUI wrapper. It is used to handle the controls
derived from the GUIMain class
*/
class GUIContainer : public GUIMain {
public:
	GUIContainer();
	GUIContainer(
		const int & rciX,
		const int & rciY,
		const uint & rcuiW,
		const uint & rcuiH);
	~GUIContainer();


//========================================================================
/** Add a nex control to the container
*/
	const uint
	Add( GUIMain* const pguimain );


//========================================================================
/** Get the number of the controls in the container
	\return the number of controls added so far
*/
	const uint
	GetNumber() const;


//========================================================================
/** Get the width and the height of the window
	\param riWinW,riWinH The width and the height of the current window
*/
	void
	GetWinWH(
		int & riWinW,
		int & riWinH ) const;


//========================================================================
/** Get the index of the first control which is clicked. The index starts
from 1.
	\return The index of the clicked control. The method returns 0 if
there's no clicked control
*/
	const uint
	GetClick() const;


//========================================================================
/** Set the attribute of a contained GUIMain object. This declaration
hides the prototype of Set inherited from the GUIMain class.
	\param rcuiIndex The index of the control to modify. It must be in
range because there is no error checking.
	\param rcubAttribute The attributes to set
*/
	void
	Set(
		const uint & rcuiIndex,
		const OC_UBYTE & rcubAttribute ) const;


//========================================================================
/**
	\note We need this method because the inherited one is hidden
*/
	void
	Set(
		const OC_UBYTE & rcubAttribute ) {
		GUIMain::Set( rcubAttribute );
	}


//========================================================================
/** Unset the specified attributes of all controls.
	\param rcubAttr The attributes to unset
*/
	void
	ResetAttribute( const OC_UBYTE & rcubAttr ) const;


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
	uint uiWinWidth, uiWinHeight;
	std::vector<GUIMain*> vectorpguimain;


   /*=====================================================================*/
   /*                       STATIC       ATTRIBUTES                       */
   /*=====================================================================*/
private:
	static GLuint	_uiTexture;
	static uint		_uiNumberContainer;
};

#endif































