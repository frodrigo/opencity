/***************************************************************************
						guimain.h    -  description
							-------------------
	begin                : march 22th, 2004
	copyright            : (C) 2004-2015 by Duong Khang NGUYEN
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


//========================================================================
/** RGBA byte color structure.
	\sa OPENCITY_PALETTE
*/
struct Color
{
	enum OPENCITY_COLOR {
		OC_BLACK,
		OC_RED,
		OC_GREEN,
		OC_BLUE,
		OC_YELLOW,
		OC_PINK,
		OC_WHITE,
		OC_COLOR_NUMBER
	};

	GLubyte r, g, b, a;
};


//========================================================================
/** Few predefined colors
*/
const Color OPENCITY_PALETTE [Color::OC_COLOR_NUMBER] =
{
	{0,     0,   0, 255},			// black
	{255,   0,   0, 255},			// red
	{0,   255,   0, 255},			// green
	{0,     0, 255, 255},			// blue
	{255, 255,   0, 255},			// yellow
	{255,   0, 255, 255},			// pink
	{255, 255, 255, 255}			// white
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
/** Set the new size of the control
	\param rcuiW,rcuiH The width and height of the container
*/
	void
	SetSize(
		const uint& rcuiW,
		const uint& rcuiH ) {
		_uiWidth = rcuiW;
		_uiHeight = rcuiH;
	}


//========================================================================
/** Set the attirbute of the control
	\param rcubAttribute The new attribute value. You can use the bit
operations here.
*/
	void
	Set(
		const OC_BYTE & rcubAttribute ) {
		_ubAttribute |= rcubAttribute;
	}


//========================================================================
/** Unset the attirbute of the control
	\param rcubAttribute The attributes to unset. You can use the bit
operations here.
*/
	void
	Unset(
		const OC_BYTE & rcubAttribute ) {
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
		const OC_BYTE & rcubAttribute ) const {
		if ( (_ubAttribute & rcubAttribute) == rcubAttribute )
			return true;
		else
			return false;
	}


//========================================================================
/** Check if the specified 2D position is inside the control.
	\param rciX,rciY The 2D position to check.
	\return True if specified position is inside the control.
*/
	bool
	IsInside(
		const int& rciX,
		const int& rciY ) {
		bool isInside = (rciX >= _iX && rciX <= _iX+(int)_uiWidth) && (rciY >= _iY && rciY <= _iY+(int)_uiHeight);
		return isInside;
	}


   //========================================================================
   //                         STATIC    METHODS
   //========================================================================


protected:
	GUIMain*	_pctr;					///< points to the container of the control
	int 		_iX, _iY;				///< the upper left corner of the GUI control
	uint		_uiWidth, _uiHeight;	///< width, height
	OC_BYTE		_ubAttribute;			///< visible, clicked, mouseover

};

#endif
































