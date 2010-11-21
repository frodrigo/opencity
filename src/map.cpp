/***************************************************************************
						map.cpp  -  description
							-------------------
	begin                : january 24th, 2004
	copyright            : (C) 2004-2006 Duong Khang NGUYEN
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

// Useful enumerations
#include "opencity_direction.h"
#include "opencity_structure_type.h"

// OpenCity headers
#include "map.h"
#include "layer.h"
#include "mapmaker.h"			// Map generator

// Global settings
#include "globalvar.h"
extern GlobalVar gVars;


   /*=====================================================================*/
Map::Map(
	const uint width,
	const uint height ):
_uiMapWidth( width ),
_uiMapHeight( height ),
_pclayer( NULL )
{
	OPENCITY_DEBUG("ctor");

	uint size = (width + 1) * (height + 1);
	_btabSquareHeight = new signed char[ size ];

	assert(gVars.gpMapMaker != NULL);
	int* heightMap = gVars.gpMapMaker->getMap();
	for (uint i = 0; i < size; i++)
		_btabSquareHeight[i] = heightMap[i];

	delete [] heightMap;
}


   /*=====================================================================*/
Map::~Map()
{
	OPENCITY_DEBUG("dtor");

	delete [] _btabSquareHeight;
	_btabSquareHeight = NULL;
}


   /*=====================================================================*/
void
Map::SaveTo( std::fstream& rfs )
{
	OPENCITY_DEBUG( __PRETTY_FUNCTION__ << " saving...");

	rfs << _uiMapWidth << std::ends;
	rfs << _uiMapHeight << std::ends;
	uint size = (_uiMapWidth + 1) * (_uiMapHeight + 1);

	for (uint i = 0; i < size; i++)
		rfs << (int)_btabSquareHeight[i] << std::ends;
}


   /*=====================================================================*/
void
Map::LoadFrom( std::fstream& rfs )
{
	OPENCITY_DEBUG( __PRETTY_FUNCTION__ << "loading...");

	rfs >> _uiMapWidth; rfs.ignore();
	rfs >> _uiMapHeight; rfs.ignore();
	uint size = (_uiMapWidth + 1) * (_uiMapHeight + 1);

	delete [] _btabSquareHeight;
	_btabSquareHeight = new signed char [size];

	int temp = 0;
	for (uint i = 0; i < size; i++) {
		rfs >> temp; rfs.ignore();
		_btabSquareHeight[i] = (signed char)temp;
	} // for
}


   /*=====================================================================*/
OPENCITY_ERR_CODE
Map::ChangeHeight(
	const uint& rcuiW,
	const uint& rcuiH,
	const OPENCITY_MAP_VARIATION& enumVar )
{
   // debugging
	assert(_pclayer!= NULL);

	if (_pclayer != NULL) {
		uint w1 = rcuiW;
		uint h1 = rcuiH;
		uint w2 = rcuiW;
		uint h2 = rcuiH;
	   // if the surface contains something, we return
		GetPossibleWH( w1, h1, -1, -1 );
		GetPossibleWH( w2, h2,  1,  1 );
		if (_pclayer->ContainStructure(
			w1, h1, w2, h2, OC_STRUCTURE_ANY ) == true)
				return OC_ERR_SOMETHING;
	}

	signed char tabH[4];
	char refH = 0;
	uint i = 0;
	uint linear = 0;

	this->GetSquareHeight( rcuiW, rcuiH, tabH );

// Check for maximum variation
	switch (enumVar) {
		case OC_MAP_UP:
			refH = GetSquareMinHeight( rcuiW, rcuiH );
			if (refH == OC_MAP_HEIGHT_MAX)
				return OC_ERR_SOMETHING;
			break;
		case OC_MAP_DOWN:
			refH = GetSquareMaxHeight( rcuiW, rcuiH );
			if (refH == OC_MAP_HEIGHT_MIN )
				return OC_ERR_SOMETHING;
			break;
		default:
			break;		// Keep GCC happy
	}

	for (i = 0; i < 4; i++)
		if (tabH[i] == refH)
			tabH[i] += enumVar;

   //--- left-up and right-up heights ---
	_WH2Linear( rcuiW, rcuiH, linear );
	_btabSquareHeight[ linear ] = tabH[0];
	_btabSquareHeight[ linear+1 ] = tabH[3];
   //--- left-down and right-down heights ---
	_WH2Linear( rcuiW, rcuiH+1, linear );
	_btabSquareHeight[ linear ] = tabH[1];
	_btabSquareHeight[ linear+1 ] = tabH[2];

	return OC_ERR_FREE;
}


   /*=====================================================================*/
void
Map::SetLayer( const Layer*  layer )
{
	_pclayer = layer;
}


   /*=====================================================================*/
const char
Map::GetLinearHeight( const uint& rcuiIndex ) const
{
	return _btabSquareHeight[rcuiIndex];
}


   /*=====================================================================*/
const uint
Map::GetMaxLinear() const
{
	return ((_uiMapWidth + 1) * (_uiMapHeight + 1) - 1);
}


   /*=====================================================================*/
void
Map::GetSquareHeight(
	const uint& rcuiW,
	const uint& rcuiH,
	signed char btabH[] ) const
{
	uint linear;

   /*--- left-up and right-up heights ---*/
	_WH2Linear( rcuiW, rcuiH, linear );
	btabH[0] = _btabSquareHeight[ linear ];
	btabH[3] = _btabSquareHeight[ linear+1 ];
   /*--- left-down and right-down heights ---*/
	_WH2Linear( rcuiW, rcuiH+1, linear );
	btabH[1] = _btabSquareHeight[ linear ];
	btabH[2] = _btabSquareHeight[ linear+1 ];
}


   /*=====================================================================*/
const signed char
Map::GetSquareMinHeight(
	const uint& rcuiW,
	const uint& rcuiL ) const
{
	signed char btabH[4];
	signed char minH = 127;

	this->GetSquareHeight( rcuiW, rcuiL, btabH );
	for (int i = 0; i < 4; i++) {
		if (btabH[i] < minH)
			minH = btabH[i];

//debug cout << "minH : " << (int)minH << endl;
	} // for

	return minH;
}


   /*=====================================================================*/
const signed char
Map::GetSquareMaxHeight(
	const uint& rcuiW,
	const uint& rcuiH ) const
{
	signed char btabH[4];
	signed char maxH = -127;

	this->GetSquareHeight( rcuiW, rcuiH, btabH );
	for (int i = 0; i < 4; i++) {
		if (btabH[i] > maxH)
			maxH = btabH[i];
	} // for

	return maxH;
}


   /*=====================================================================*/
const bool
Map::IsSquarePlane(
	const uint w,
	const uint l ) const
{
	signed char btabH[4];

	this->GetSquareHeight( w, l, btabH );
	bool isPlane = (btabH[0] == btabH[1]) and (btabH[0] == btabH[2]) and (btabH[0] == btabH[3]);

	return isPlane;
}


   /*=====================================================================*/
const bool
Map::IsSurfacePlane(
	const uint w1, const uint l1,
	const uint w2, const uint l2 ) const
{
	bool isPlane = true;
	uint w = w1, l = l1;

	while (isPlane and l <= l2) {
		w = w1;
		while (isPlane and w <= w2) {
			isPlane = this->IsSquarePlane( w, l );
			w++;
		} // while
		l++;
	} // while

	return isPlane;
}


   /*=====================================================================*/
const bool
Map::IsSquareLinearPlane(
	const uint& rcuiLinearIndex )
{
	uint w, h;

	_Linear2WH( rcuiLinearIndex, w, h );

	return this->IsSquarePlane( w, h );
}


   /*=====================================================================*/
const bool
Map::GetNeighbourWH(
	const uint& rcuiMapW,
	const uint& rcuiMapH,
	uint& ruiNbrW,
	uint& ruiNbrH,
	const OPENCITY_DIRECTION& enumDir ) const
{
	int w = rcuiMapW;
	int h = rcuiMapH;

/*debug
	cout << "w : " << w
	     << "/ h : " << h << endl;
*/

	switch (enumDir) {
		case OC_DIR_O_N: h--;      break;
		case OC_DIR_N_E: h--; w++; break;
		case OC_DIR_O_E: w++;      break;
		case OC_DIR_S_E: w++; h++; break;
		case OC_DIR_O_S: h++;      break;
		case OC_DIR_S_W: h++; w--; break;
		case OC_DIR_O_W: w--;      break;
		case OC_DIR_N_W: w--; h--; break;
		default: break;
	}

/*debug
	cout << "w : " << w
	     << "/ h : " << h << endl;
*/

	if ((w < 0) || (w >= (int)_uiMapWidth)
	 || (h < 0) || (h >= (int)_uiMapHeight)) {
		return false;
	}
	else {
		ruiNbrW = w;
		ruiNbrH = h;
		return true;
	}
}


   /*=====================================================================*/
void
Map::GetPossibleWH(
	uint& rW,
	uint& rH,
	const int& deltaW,
	const int& deltaH ) const
{
	int newW = rW + deltaW;
	int newH = rH + deltaH;

	if (newW < 0)
		newW = 0;
	if (newW >= (int)_uiMapWidth)
		newW = _uiMapWidth - 1;
	if (newH < 0)
		newH = 0;
	if (newH >= (int)_uiMapHeight)
		newH = _uiMapHeight - 1;

	rW = newW;
	rH = newH;
}


   /*=====================================================================*/
void
Map::_Linear2WH( const uint& linear,
		      uint& w,
		      uint& h ) const
{
//debug
cout << "not implemented yet " << endl;
assert(0);
}


   /*=====================================================================*/
inline uint
Map::_WH2Linear(
	const uint& w,
	const uint& h,
	uint& linear ) const
{
	return (linear = h * (_uiMapWidth + 1) + w);
}


























