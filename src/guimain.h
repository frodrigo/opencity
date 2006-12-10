/***************************************************************************
						guimain.h    -  description
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

#ifndef _OPENCITY_GUIMAIN_H_
#define _OPENCITY_GUIMAIN_H_ 1

#include "main.h"
#include "ui.h"

#include "SDL_image.h"

// Each GUIMain object has the following attributes
#define OC_GUIMAIN_VISIBLE    0x01		///< Is the object visible ?
#define OC_GUIMAIN_CLICKED    0x02		///< Has the object been clicked ?
#define OC_GUIMAIN_MOUSEOVER  0x04		///< Does the object have the mouse focus ?

#define OC_GUIMAIN_BLENDING   0x40		///< Blending state for texture


/** RGBA byte color structure
*/
struct Color {
	GLubyte r, g, b, a;
};


//========================================================================
/** The base interface class of all graphic user interface control.
	\Note Use "pctr" prefix for container pointers, "pbtn" prefix for
button pointers and "plbl" prefix for label pointers
*/
class GUIMain : public UI {
public:

//========================================================================
/** The default ctor. The new GUI control is not visible, the mouse over
state is false, and not clicked
*/
	GUIMain();

	virtual ~GUIMain();


//========================================================================
/** The derived classes must implement this method in order to receive
the "display" call from the GUIContainer
	\sa GUIContainer
*/
	virtual void
	Display() const = 0;


//========================================================================
/** Get the GUIContainer of the control
	\return The constant pointer to the container of this control
	\sa GUIContainer
*/
	GUIMain* const
	GetContainer() const {
		return _pctr;
	}


//========================================================================
/** Set the GUIContainer of the control to the specified one
	\param pguicontain The constant pointer of the new GUIContainer
	\sa GUIContainer
*/
	void
	SetContainer(
		GUIMain* const pguicontain ) {
		_pctr = pguicontain;
	}


//========================================================================
/** Get the 2D position of the control
	\param riX,riY The 2D XY coordinates of the control
*/
	void
	GetLocation(
		int & riX,
		int & riY ) const {
		riX = _iX;
		riY = _iY;
	}


//========================================================================
/** Set the 2D position of the control
	\param rciX,rciY The 2D XY coordinates of the control
*/
	void
	SetLocation(
		const int & rciX,
		const int & rciY ) {
		_iX = rciX;
		_iY = rciY;
	}


//========================================================================
/** Set the attirbute of the control
	\param rcubAttribute The new attribute value. You can use the bit
operations here.
*/
	void
	Set(
		const OC_UBYTE & rcubAttribute ) {
		_ubAttribute |= rcubAttribute;
	}


//========================================================================
/** Unset the attirbute of the control
	\param rcubAttribute The attributes to unset. You can use the bit
operations here.
*/
	void
	Unset(
		const OC_UBYTE & rcubAttribute ) {
		_ubAttribute &= ~rcubAttribute;
	}


//========================================================================
/** Check if the specified attributes are set on the control
	\param rcubAttribute The attributes to test. You can use the bit
operations here.
	\return True if the specified attributes are set on the control, false
otherwise
*/
	const bool
	IsSet(
		const OC_UBYTE & rcubAttribute ) const {
		if ( (_ubAttribute & rcubAttribute) == rcubAttribute )
			return true;
		else
			return false;
	}


   //========================================================================
   //                         STATIC    METHODS
   //========================================================================


protected:
	GUIMain*	_pctr;					///< points to the container of the control
	int 		_iX, _iY;				///< the upper left corner of the GUI control
	uint		_uiWidth, _uiHeight;	///< width, height
	OC_UBYTE	_ubAttribute;			///< visible, clicked, mouseover

};

#endif
































