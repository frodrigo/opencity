/***************************************************************************
                          layer.h  -  description
          $Id$
                             -------------------
    begin                : september 20th, 2003
    copyright            : (C) 2003-2006 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_LAYER_H_
#define _OPENCITY_LAYER_H_ 1

#include "main.h"

#include "persistence.h"
#include "ui.h"

class Structure;
class GUIContainer;
class GUIButton;


/** This is the interface of all other *layer classes. 
*/
class Layer : public Persistence, public UI
{

public:
	Layer(  );

	virtual
	~Layer(  );

	// design notes: the derived class should implement the following pure
	// virtual functions too.
	/*-- begin here --
	virtual void SaveTo( fstream& rfs ) = 0;
	virtual void LoadFrom( fstream& rfs ) = 0;

	virtual void uiKeyboard( const SDL_KeyboardEvent & rcsSDLKeyboardEvent ) = 0;
	virtual void uiMouseMotion( const SDL_MouseMotionEvent & 
rcsSDLMouseMotionEvent ) = 0;
	virtual void uiMouseButton( const
SDL_MouseButtonEvent & rcsSDLMouseButtonEvent ) = 0;
	virtual void uiExpose( const SDL_ExposeEvent & rcsSDLExposeEvent ) = 0;
	virtual void uiResize( const SDL_ResizeEvent & rcsSDLResizeEvent )= 0;
	*-- end here --*/


//========================================================================
/** Is the surface ( w1, l1, w2, l2 ) constructive ?
	Is there something built on it already ?
*/
	virtual const bool
	IsConstructive(
		uint W1, uint L1,
		uint W2, uint L2,
		const OPENCITY_STRUCTURE_CODE & enumStructCode ) const = 0;


//========================================================================
/** Preview the structure which is about to be built on the specified
	surface.
	\param W,L The upper left corner of the surface which will be used 
to build the bulding.
	\param enumGraphicCode Will contain the OC graphic code of the building
	\param enumStructCode Contain the structure code
	\sa IsConstructive()
	\sa BuildStructure()
*/
	virtual const OPENCITY_ERR_CODE
	BuildPreview(
		const uint & W,
		const uint & L,
		const OPENCITY_STRUCTURE_CODE & enumStructCode,
		OPENCITY_GRAPHIC_CODE & enumGraphicCode ) const = 0;


//========================================================================
/** Build the specified structure on the surface
	Precondition: the specified surface must be "constructive". This is
	checked by the method "IsConstructive()"
	\sa IsConstructive()
*/
	virtual const OPENCITY_ERR_CODE
	BuildStructure(
		const uint & W1,
		const uint & L1,
		const uint & W2,
		const uint & L2,
		const OPENCITY_STRUCTURE_CODE & enumStructCode,
		uint& rCost ) = 0;


//========================================================================
/** Resize the surface used by the structure to the new surface thanks to
	its new GraphicCode.
*/
	virtual const OPENCITY_ERR_CODE
	ResizeStructure(
		const uint & w,
		const uint & l,
		const OPENCITY_GRAPHIC_CODE & oldGC ) = 0;


//========================================================================
/** Destroy _all_ the structures built on the surface. If the specified
	surface is a part of a bigger structure, the big main structure is
	also destroyed.
*/
	virtual const OPENCITY_ERR_CODE
	DestroyStructure(
		uint W1, uint L1,
		uint W2, uint L2,
		uint& rCost ) = 0;


//========================================================================
/** Access to the structure by its internal linear index. There's no error
	checking.
*/
	virtual Structure*
	GetLinearStructure( const uint & rcuiLinearIndex ) const = 0;


	virtual Structure*
	GetStructure(
		const uint & w,
		const uint & l ) const = 0;


	virtual Structure*
	GetRandomStructure(
		uint & w,
		uint & l,
		const OPENCITY_STRUCTURE_CODE & enumStructCode = OC_STRUCTURE_UNDEFINED ) const = 0;


	virtual const uint
	GetNumberStructure() const = 0;


	const uint
	GetMaxLinear() const;


	virtual void
	StructureSet(
		const OC_BYTE & status ) = 0;


	virtual void
	StructureUnset(
		const OC_BYTE & status ) = 0;


//========================================================================
/** Does the specified surface contain _at least_ one required structure ?
*/
	virtual const bool
	ContainStructure(
		const uint & W1,
		const uint & L1,
		const uint & W2,
		const uint & L2,
		const OPENCITY_STRUCTURE_CODE & enumStructCode ) const = 0;


//========================================================================
/** Does the specified surface contain _only_ the required structure or
	nothing ?
*/
	virtual const bool
	ContainStructureOnly(
		const uint & W1,
		const uint & L1,
		const uint & W2,
		const uint & L2,
		const OPENCITY_STRUCTURE_CODE & enumStructCode ) const = 0;


//========================================================================
/** This methods returns a pointer to a container which contains
	the queried informations. The user must call "delete pointer" to
	destroy that pointer once he/she've done with it.
*/
	virtual GUIContainer*
	QueryStructure(
		const uint & w,
		const uint & l ) const = 0;



protected:
	uint _uiLayerWidth;
	uint _uiLayerLength;

   /// total derived classes
	static uint uiNumberLayer;

//========================================================================
/** Used by the "Query" function
	these controls are shared by all the derived layers
*/
	static GUIButton* pbtnQW;			///< water
	static GUIButton* pbtnQE;			///< electricity
	static GUIButton* pbtnQG;			///< gas
	static GUIButton* pbtnQR;			///< residential
	static GUIButton* pbtnQC;			///< commercial
	static GUIButton* pbtnQI;			///< industrial
	static GUIButton* pbtnQP;			///< path
};

#endif






































