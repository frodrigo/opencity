/***************************************************************************
                          guicontainer.h    -  description
    $Id$
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

#if !defined(_OPENCITY_GUICONTAINER_H_)
	#define _OPENCITY_GUICONTAINER_H_ 1

	#include "main.h"

	#include "guimain.h"

	#include <vector>

   //========================================================================
   /** Contains all stuff derived from GUIMain class
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
	const uint
	Add( GUIMain* const pguimain );

   //========================================================================
   /**
   \return the number of controls added so far
   */
	const uint
	GetNumber() const;

	void
	GetWinWH(
		int & riWinW,
		int & riWinH ) const;

	const uint
	GetClick() const;


   //========================================================================
   /** Set the attribute of a contained GUIMain object. This declaration
       hides the prototype of Set inherited from the GUIMain class
   */
	void
	Set(
		const uint & rcuiIndex,
		const OC_UBYTE & rcubAttribute ) const;

	void
	Set(
		const OC_UBYTE & rcubAttribute ) {
		GUIMain::Set( rcubAttribute );
	}


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

};

#endif































