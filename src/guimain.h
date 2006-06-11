/***************************************************************************
                          guimain.h    -  description
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

#if !defined(_OPENCITY_GUIMAIN_H_)
	#define _OPENCITY_GUIMAIN_H_ 1

	#include "main.h"
	#include "ui.h"

	#include "SDL_image.h"

   // each GUIMain object has the following attributes
	#define OC_GUIMAIN_VISIBLE    0x01		///< Is the object visible ?
	#define OC_GUIMAIN_CLICKED    0x02		///< Has the object been clicked ?
	#define OC_GUIMAIN_MOUSEOVER  0x04		///< Does the object have the mouse focus ?

	#define OC_GUIMAIN_BLENDING   0x40		///< Blending state for texture


   //========================================================================
   /** The base class of all graphic user interface control
   */
class GUIMain : public UI {
public:
   /** By default, the gui is not visible, the mouse over state is false,
       and the control is not clicked
   */
	GUIMain():ubAttribute( 0 ) {
	}

	virtual ~GUIMain() {}

	virtual void
	Display() const = 0;


	GUIMain* const
	GetContainer() const {
		return this->pguicontainer;
	}

	void
	SetContainer(
		GUIMain* const pguicontain ) {
		this->pguicontainer = pguicontain;
	}

	void
	GetLocation(
		int & riX,
		int & riY ) const {
		riX = this->iX;
		riY = this->iY;
	}

	void
	SetLocation(
		const int & rciX,
		const int & rciY ) {
		this->iX = rciX;
		this->iY = rciY;
	}

	void
	Set(
		const OC_UBYTE & rcubAttribute ) {
		this->ubAttribute |= rcubAttribute;
	}

	void
	Unset(
		const OC_UBYTE & rcubAttribute ) {
		this->ubAttribute &= ~rcubAttribute;
	}

	const bool
	IsSet(
		const OC_UBYTE & rcubAttribute ) const {
		if ( (this->ubAttribute & rcubAttribute) == rcubAttribute )
			return true;
		else
			return false;
	}


   //========================================================================
   //                         STATIC    METHODS
   //========================================================================
/*
TOKILL, moved to "texture.h"
	static const GLuint
	LoadTexture(
		const string & rcFile );

	static void
	Surface2Texture(
		const SDL_Surface* const psurface,
		GLuint & ruiTexture );
*/


protected:
	GUIMain* pguicontainer;		///< points to the container of the control
	int  iX, iY;				///< the upper left corner of the GUI control
	uint uiWidth, uiHeight;	///< width, height
	OC_UBYTE ubAttribute;		///< visible, clicked, mouseover

};

#endif
































