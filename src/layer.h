/***************************************************************************
						layer.h  -  description
							-------------------
	begin                : september 20th, 2003
	copyright            : (C) 2003-2008 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_LAYER_H_
#define _OPENCITY_LAYER_H_ 1

#include "main.h"

#include "persistence.h"
#include "ui.h"

class Structure;
class GUIContainer;
class GUIButton;


//========================================================================
/** This is the interface of all other layer classes. A layer is
persistent and should handle the user interface commands.
*/
class Layer : public Persistence, public UI
{
public:
	Layer(  );

	virtual
	~Layer(  );


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
	\param w,l The NorthWest (top left) coordinates of the structure
	\param oldGC The old graphic code
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
/** Fast access to the structure by its internal linear index. There's
no error checking.
	\param cuiLinearIndex The linear index of the structure to access.
It starts from 0.
*/
	virtual Structure*
	GetLinearStructure( const uint cuiLinearIndex ) const = 0;


//========================================================================
/** Access to the structure specified by its WL coordinates
	\param w,l The structure NorthWest (top left) coordinates
*/
	virtual Structure*
	GetStructure(
		const uint & w,
		const uint & l ) const = 0;


//========================================================================
/** Random access to the structure of the specified type
	\param w,l The structure NorthWest (top left) coordinates
	\param enumStructCode The structure code
	\return The randomly selected structure pointer
*/
	virtual Structure*
	GetRandomStructure(
		uint & w,
		uint & l,
		const OPENCITY_STRUCTURE_CODE & enumStructCode = OC_STRUCTURE_UNDEFINED ) const = 0;


//========================================================================
/** Get the number of the structures built on the layer
*/
	virtual const uint
	GetNumberStructure() const = 0;


	const uint
	GetMaxLinear() const;


//========================================================================
/** Get the layer size.
	\note This method is obsolete
	\see GlobalVar
*/
	void
	GetLayerSize( uint& w, uint& l) const;


//========================================================================
/** Set the status of all structures
	\param status The new status of the structures
*/
	virtual void
	StructureSet( const OC_BYTE& status ) = 0;


//========================================================================
/** Unset the status of all structures
	\param status The status to unset
*/
	virtual void
	StructureUnset( const OC_BYTE& status ) = 0;


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
/** This methods returns a pointer to a container which contains the
queried informations. The user must call "delete pointer" to destroy
that pointer once he/she've done with it.
*/
	virtual GUIContainer*
	QueryStructure(
		const uint & w,
		const uint & l ) const = 0;


protected:
	uint _uiLayerWidth;
	uint _uiLayerLength;


protected:
   /*=====================================================================*/
   /*                       STATIC       ATTRIBUTES                       */
   /*=====================================================================*/

   /// total derived classes
	static uint uiNumberLayer;

//========================================================================
/** Used by the "Query" function
these controls are shared by all the derived layers
*/
	static GUIButton* pbtnQRo, *pbtnQRf;			///< Residential query button
	static GUIButton* pbtnQCo, *pbtnQCf;			///< Commercial query button
	static GUIButton* pbtnQIo, *pbtnQIf;			///< Industrial query button
	static GUIButton* pbtnQWo, *pbtnQWf;			///< Water query button
	static GUIButton* pbtnQEo, *pbtnQEf;			///< Electricity query button
	static GUIButton* pbtnQGo, *pbtnQGf;			///< Gas query button
	static GUIButton* pbtnQP;						///< Path query button
};

#endif






































