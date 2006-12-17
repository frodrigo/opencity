/***************************************************************************
                          buildinglayer.h  -  description
      $Id$
                             -------------------
    begin                : sam sep 20 2003
    copyright            : (C) 2003-2005 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_BUILDINGLAYER_H_
#define _OPENCITY_BUILDINGLAYER_H_ 1

#include "main.h"

#include "layer.h"

class City;

/**
	This class contains all the buildings added by the player
*/
class BuildingLayer : public Layer  {
public:
//constructor, should not be used. Call the constructor with parameters instead.
	//BuildingLayer();

	virtual ~BuildingLayer(); 				//dtor

	BuildingLayer( const City & rcCity );	//ctor with parameters


//========================================================================
/** Save the data to the specified fstream
	\param rfs A reference to a file stream which is ready for writing
	\see Persistence
*/
	void
	SaveTo( std::fstream& rfs );


//========================================================================
/** Load the data from the specified stream
	\param rfs A reference to a file stream which is ready for reading
	\see Persistence
*/
	void
	LoadFrom( std::fstream& rfs );


//========================================================================
/** Is the specified area constructive ?
	Can we build the specified structure on it ?
	\param W1,W2 = OpenGL Ox
	\param L1,L2 = OpenGL Oz
	\param enumStructCode = The code of the structure that we're going to build
	\return True or False
*/
	const bool
	IsConstructive(
		uint W1, uint L1,
		uint W2, uint L2,
		const OPENCITY_STRUCTURE_CODE & enumStructCode ) const;


//========================================================================
/** see "layer.h" for more informations
	\sa Layer::BuildPreview()
*/
	const OPENCITY_ERR_CODE
	BuildPreview(
		const uint & W,
		const uint & L,
		const OPENCITY_STRUCTURE_CODE & enumStructCode,
		OPENCITY_GRAPHIC_CODE & enumGraphicCode ) const;


//========================================================================
/** see "layer.h" for more informations
	\see Layer
*/
	const OPENCITY_ERR_CODE
	BuildStructure(
		const uint & W1,
		const uint & L1,
		const uint & W2,
		const uint & L2,
		const OPENCITY_STRUCTURE_CODE & enumStructCode,
		uint& rCost );


//========================================================================
/** Find the main structure WL coordinates of the given structure pointer
	\param pstruct The structure pointer to search for
	\param w,l The WL coordinates of pstruct
	\return True if found, false otherwise
*/
	const bool
	GetMainWL (
		const Structure* const pstruct,
		uint & w,
		uint & l
	) const;


//========================================================================
/** Resize the surface used by the structure to the new surface thanks to
	its new GraphicCode.
	\param w,l = The structure's map coordinates
	\param oldGC = The old graphic code of the structure
*/
	const OPENCITY_ERR_CODE
	ResizeStructure(
		const uint & w,
		const uint & l,
		const OPENCITY_GRAPHIC_CODE & oldGC );


//========================================================================
/** Destroy all the structures built on the specified surface
	\param W1,W2 = OpenGL Ox
	\param L1,L2 = OpenGL Oz
	\param rCost = the cost needed to clean up the specified surface
	\return Error code
*/
	const OPENCITY_ERR_CODE
	DestroyStructure(
		uint W1, uint L1,
		uint W2, uint L2,
		uint& rCost );


//========================================================================
/** Returns a pointer to a structure indexed by its linear index. The
linear index of a structure is calculated by this formula:
	index = W + (H * city width)
	\param cuiLinearIndex = linear index of the structure to retrieve
	\return A pointer to a structure if it exists NULL otherwise
*/
	Structure*
	GetLinearStructure(
		const uint cuiLinearIndex ) const;


	Structure*
	GetStructure(
		const uint & w,
		const uint & l ) const;


	Structure*
	GetRandomStructure(
		uint & w,
		uint & l,
		const OPENCITY_STRUCTURE_CODE & enumStructCode = OC_STRUCTURE_UNDEFINED ) const;


	const uint
	GetNumberStructure() const;


//========================================================================
/** Set all the structures contained by this layer to the specified status.
	\param status = The new status of the structures
*/
	void
	StructureSet(
		const OC_BYTE & status );


//========================================================================
/** Unset the specified status of all the structures contained
	by this layer.
	\param status = The status of the structures to be unset
*/
	void
	StructureUnset(
		const OC_BYTE & status );


//========================================================================
/** see "layer.h" for more informations
*/
	const bool
	ContainStructure(
		const uint & W1,
		const uint & L1,
		const uint & W2,
		const uint & L2,
		const OPENCITY_STRUCTURE_CODE & enumStructCode ) const;


//========================================================================
/** see "layer.h" for more informations
*/
	const bool
	ContainStructureOnly(
		const uint & W1,
		const uint & L1,
		const uint & W2,
		const uint & L2,
		const OPENCITY_STRUCTURE_CODE & enumStructCode ) const;


//========================================================================
/** see "layer.h" for more informations
*/
	GUIContainer*
	QueryStructure(
		const uint & w,
		const uint & l ) const;


//========================================================================
// Inherited methods from UI
//========================================================================
	void Keyboard( const SDL_KeyboardEvent& rcEvent );
	void MouseMotion( const SDL_MouseMotionEvent& rcEvent );
	void MouseButton( const SDL_MouseButtonEvent& rcEvent );
	void Expose( const SDL_ExposeEvent& rcEvent );
	void Resize( const SDL_ResizeEvent& rcEvent );


private:
	/** All the buildings are here */
	Structure** _tabpStructure;





   /*=====================================================================*/
   /*                        PRIVATE     METHODS                          */
   /*=====================================================================*/
	const bool
	_IsPathConstructive(
		const uint & w,
		const uint & h,
		const OPENCITY_STRUCTURE_CODE & enumStructCode ) const;


//========================================================================
/** Called internally by other methods, don't touch it
*/
	const OPENCITY_ERR_CODE
	_BuildPathStructure(
		uint W1, uint L1,
		uint W2, uint L2,
		const OPENCITY_STRUCTURE_CODE & enumStructCode,
		uint& rCost );


//========================================================================
/** Build a path structure at the specified coordinates
	\note If there's already a structure at the specified coordinates, this
method returns without generating any error
	\param w,h The coordinates
	\param linearIndex The index of the structure in the structure array
	\param enumStructCode The structure code (=STRUCTURE_ROAD)
	\param rCost The cost of the construction
*/
	void
	_BuildPathStructure(
		const uint & w,
		const uint & h,
		const uint & linearIndex,
		const OPENCITY_STRUCTURE_CODE & enumStructCode,
		uint& rCost );


	const OPENCITY_ERR_CODE
	_BuildRCIStructure(
		uint W1, uint L1,
		uint W2, uint L2,
		const OPENCITY_STRUCTURE_CODE & enumStructCode,
		uint& rCost );


	const OPENCITY_ERR_CODE
	_BuildFloraStructure(
		uint W1, uint L1,
		uint W2, uint L2,
		const OPENCITY_STRUCTURE_CODE & enumStructCode,
		uint& rCost );


	const OPENCITY_ERR_CODE
	_BuildWEGStructure(
		uint W1, uint L1,
		const OPENCITY_STRUCTURE_CODE & enumStructCode,
		uint& rCost );


	void
	_BuildWEGStructure(
		uint W1, uint L1,
		uint W2, uint L2,
		const OPENCITY_STRUCTURE_CODE & enumStructCode,
		uint& rCost );


//========================================================================
/** Fast load a structure at the specified coordinates. If the
structure is bigger than 1x1, then extra structures part will be built
automatically. There is not any "Constructive" test.
	\note If there's already a structure at the specified coordinates, this
method returns without generating any error
	\param w1,l1	The structure upper-left map coordinates
	\param pStruct	The pointer to the structure which will be deleted
automatically when the program ends
*/
	void
	_LoadStructure(
		const uint & w1,
		const uint & l1,
		Structure* pStruct );


//========================================================================
/** Destroy the structure built at the specified coordinates
	\param w = OpenGL Ox
	\param l = OpenGL Oz
	\param rCost = the cost needed to clean up the specified cell
	\return Error code
*/
	const OPENCITY_ERR_CODE
	_DestroyStructure(
		const uint & w,
		const uint & l,
		uint& rCost );


	const OPENCITY_ERR_CODE
	_DestroyPathStructure(
		uint w, uint l,
		const OPENCITY_STRUCTURE_CODE & enumStructCode );
};

#endif

























