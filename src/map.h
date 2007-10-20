/***************************************************************************
						map.h  -  description
							-------------------
	begin                : january 24th, 2004
	copyright            : (C) 2004-2007 Duong-Khang NGUYEN
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

#ifndef _OPENCITY_MAP_H_
#define _OPENCITY_MAP_H_ 1

#include "main.h"
#include "persistence.h"

enum OPENCITY_DIRECTION;
class Layer;


//========================================================================
/** A "Map" object contains an array of square's OY height. Each square
	has 4 values of heights. They match the map square's corners.
*/
class Map: public Persistence {
public:
	Map(
		const uint width,
		const uint height );

	~Map();


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
/** Change the square's OY heights. The square is designated
	by its W,H coordinates. ( OX, OZ OpenGL coordinates )
*/
	OPENCITY_ERR_CODE
	ChangeHeight(
		const uint & rcuiW,
		const uint & rcuiH,
		const OPENCITY_MAP_VARIATION & enumVar );


	void
	SetLayer(
		const Layer*  layer );


	const OC_BYTE &
	GetLinearHeight(
		const uint & rcuiIndex ) const;


	const uint
	GetMaxLinear() const;


//========================================================================
/** Get all the 4 heights of a square designated by its W,H.
The corner's height is returned in the following order:
left-up, left-down, right-down, right-up
	\param rcuiW,rcuiH The W, L (x, y) map coordinates
	\param tabH The height table
*/
	void
	GetSquareHeight(
		const uint & rcuiW,
		const uint & rcuiH,
		OC_BYTE tabH [] ) const;


	const signed char
	GetSquareMinHeight(
		const uint & rcuiW,
		const uint & rcuiL ) const;


	const signed char
	GetSquareMaxHeight(
		const uint & rcuiW,
		const uint & rcuiH ) const;


//========================================================================
/** Check if the specified square is plane
	\param w,l The WL coordinates of the square to check
	\return True if it's plane, false otherwise
*/
	const bool
	IsSquarePlane(
		const uint w,
		const uint l ) const;


	const bool
	IsSurfacePlane(
		const uint w1, const uint l1,
		const uint w2, const uint l2 ) const;


	const bool
	IsSquareLinearPlane(
		const uint & rcuiLinearIndex );


//========================================================================
/** Calculate the WH coordinates of the neighbour square given the direction
	\return true if the WH of the neighbour were found false otherwise
	(the current square is located at the edge of the map)
*/
	const bool
	GetNeighbourWH(
		const uint & rcuiMapW,
		const uint & rcuiMapH,
		uint & ruiNbrW,
		uint & ruiNbrH,
		const OPENCITY_DIRECTION & enumDir ) const;


//========================================================================
/** Calculate the new possible WH given the changes. If the delta values 
are too big to be incorporated in the new WH, the new WH values are 
cliped to the map edges
*/
	void
	GetPossibleWH(
		uint & rW,
		uint & rH,
		const int & deltaW,
		const int & deltaH ) const;


private:
	uint _uiMapWidth;
	uint _uiMapHeight;

	OC_BYTE* _btabSquareHeight;

/// we need this to know if a square can be safely raised up or lowered down
	const Layer* _pclayer;



   /*=====================================================================*/
   /*                        PRIVATE     METHODS                          */
   /*=====================================================================*/

//========================================================================
/** Convert the linear index to the square's W,H coordinates
*/
	void
	_Linear2WH(
		const uint & linear,
		uint & w,
		uint & h ) const;


//========================================================================
/** Convert the W,H coordinates to the linear index according to the 
datas stocked inside the class
	\note Must be dishtinguished from structure linear index
*/
	inline uint
	_WH2Linear(
		const uint & w,
		const uint & h,
		uint & linear ) const;
};

#endif



























