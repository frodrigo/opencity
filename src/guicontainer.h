/***************************************************************************
						guicontainer.h    -  description
							-------------------
	begin                : march 22th, 2004
	copyright            : (C) 2004-2007 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_GUICONTAINER_H_
#define _OPENCITY_GUICONTAINER_H_ 1

#include "guimain.h"

#include <vector>


//========================================================================
/** A container is a GUI wrapper. It is used to handle the controls
derived from the GUIMain class
*/
class GUIContainer : public GUIMain {
public:
	GUIContainer();


//========================================================================
/** Pctor 1.
	\param ciX,ciY The 2D XY window coordinates of the container
	\param cuiW,cuiH The width and height of the container
*/
	GUIContainer(
		const int ciX,
		const int ciY,
		const uint cuiW,
		const uint cuiH );


//========================================================================
/** Pctor 2.
	\param ciX,ciY The 2D XY window coordinates of the container
	\param cuiW,cuiH The width and height of the container
	\param rcsTexFile The name of the texture file which will be used as
background.
*/
	GUIContainer(
		const int ciX,
		const int ciY,
		const uint cuiW,
		const uint cuiH,
		const string & rcsTexFile );


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
	void Keyboard( const SDL_KeyboardEvent& rcEvent );
	void MouseMotion( const SDL_MouseMotionEvent& rcEvent );
	void MouseButton( const SDL_MouseButtonEvent& rcEvent );
	void Expose( const SDL_ExposeEvent& rcEvent );
	void Resize( const SDL_ResizeEvent& rcEvent );


private:
	uint	_uiWinWidth, _uiWinHeight;
	std::vector<GUIMain*> vectorpguimain;
	GLuint	_uiTexBackground;
};

#endif































