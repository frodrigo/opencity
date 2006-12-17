/***************************************************************************
						guibar.h    -  description
							-------------------
	begin                : december, 14th 2006
	copyright            : (C) 2006 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_GUIBAR_H_
#define _OPENCITY_GUIBAR_H_ 1

#include "guimain.h"


//========================================================================
/** A bar GUI control is used to display a color bar in 2D
*/
class GUIBar : public GUIMain {
public:
	enum OPENCITY_VARIATION {
		OC_HORIZONTAL,
		OC_VERTICAL,
		OC_BOTH
	};

	GUIBar();
	GUIBar(
		const int ciX,
		const int ciY,
		const uint cuiW,
		const uint cuiH );
	~GUIBar();


//========================================================================
	void
	SetInitialValue( const float fValue );

	void
	SetValue( const float fValue );

	void
	SetVariation( const OPENCITY_VARIATION eV );




	void
	SetForeground( const Color& color );


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
	OPENCITY_VARIATION _eVariation;

	/** The quantity that the full bar surface represents.
	Must be diffrent than 0
	*/
	float	_fInitialValue;
	float	_fValue;				///< The current quantity of the bar
	float	_fWidthRatio;			///< The ratio of W to display
	float	_fHeightRatio;			///< The ratio of H to display
	Color	_cForeground;			///< The color of the bar
};

#endif
































