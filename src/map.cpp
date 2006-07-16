/***************************************************************************
							map.cpp  -  description
								-------------------
	begin                : january 24th, 2004
	copyright            : (C) 2004-2006 Duong-Khang NGUYEN
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


#include "map.h"
#include "layer.h"

#include "mapmaker.h"			// Map generator


   /*=====================================================================*/
Map::Map(
	const uint & width,
	const uint & height ):
_uiMapWidth( width ),
_uiMapHeight( height ),
_pclayer( NULL )
{
	OPENCITY_DEBUG("ctor");

	uint size = (width + 1) * (height + 1);
	_btabSquareHeight = new OC_BYTE [ size ];

	mapgen::MapMaker mapMaker = mapgen::MapMaker( width+1, height+1, mapgen::MapMaker::PLAIN );
	int* heightMap = mapMaker.getMap();

	for (uint i = 0; i < size; i++)
		_btabSquareHeight[i] = heightMap[i];

	delete heightMap;
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
	OPENCITY_DEBUG( __PRETTY_FUNCTION__ << "saving...");

	rfs << _uiMapWidth << endl;
	rfs << _uiMapHeight << endl;
	uint size = (_uiMapWidth + 1) * (_uiMapHeight + 1);

	for (uint i = 0; i < size; i++)
		rfs << _btabSquareHeight[i];
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
	_btabSquareHeight = new OC_BYTE [size];
	for (uint i = 0; i < size; i++)
		rfs >> _btabSquareHeight[i];
}


   /*=====================================================================*/
OPENCITY_ERR_CODE
Map::ChangeHeight(
	const uint & rcuiW,
	const uint & rcuiH,
	const OPENCITY_MAP_VARIATION & enumVar )
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

	OC_BYTE tabH [4];
	OC_BYTE refH;
	uint i;
	uint linear;

	GetSquareHeight( rcuiW, rcuiH, tabH );
   // check for maximum variation
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
const OC_BYTE &
Map::GetLinearHeight( const uint & rcuiIndex ) const
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
	const uint & rcuiW,
	const uint & rcuiH,
	OC_BYTE btabH[] ) const
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
	const uint & rcuiW,
	const uint & rcuiL ) const
{
	OC_BYTE btabH [4];
	OC_BYTE minH = 127;

	GetSquareHeight( rcuiW, rcuiL, btabH );
	for (uint i = 0; i < 4; i++)
		if (btabH[i] < minH)
			minH = btabH[i];
//debug cout << "minH : " << (int)minH << endl;
	return minH;
}


   /*=====================================================================*/
const signed char
Map::GetSquareMaxHeight(
	const uint & rcuiW,
	const uint & rcuiH ) const
{
	OC_BYTE btabH [4];
	signed char maxH = -127;

	GetSquareHeight( rcuiW, rcuiH, btabH );
	for (uint i = 0; i < 4; i++)
		if (btabH[i] > maxH)
			maxH = btabH[i];

	return maxH;
}


   /*=====================================================================*/
const bool
Map::IsSquarePlane(
	const uint & rcuiW,
	const uint & rcuiH ) const
{
	OC_BYTE btabH [4];

	GetSquareHeight( rcuiW, rcuiH, btabH );

	return   ((btabH[0] == btabH[1])
		&&(btabH[0] == btabH[2])
		&&(btabH[0] == btabH[3]));
}


   /*=====================================================================*/
const bool
Map::IsSquareLinearPlane(
	const uint & rcuiLinearIndex )
{
	uint w, h;

	_Linear2WH( rcuiLinearIndex, w, h );

	return IsSquarePlane( w, h );
}


   /*=====================================================================*/
const bool
Map::GetNeighbourWH(
	const uint & rcuiMapW,
	const uint & rcuiMapH,
	uint & ruiNbrW,
	uint & ruiNbrH,
	const OPENCITY_DIRECTION & enumDir ) const
{
	int w = rcuiMapW;
	int h = rcuiMapH;

/*debug
	cout << "w : " << w
	     << "/ h : " << h << endl;
*/

	switch (enumDir) {
		case OC_DIR_N:  h--;      break;
		case OC_DIR_NE: h--; w++; break;
		case OC_DIR_E:  w++;      break;
		case OC_DIR_SE: w++; h++; break;
		case OC_DIR_S:  h++;      break;
		case OC_DIR_SW: h++; w--; break;
		case OC_DIR_W:  w--;      break;
		case OC_DIR_NW: w--; h--; break;
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
	uint & rW,
	uint & rH,
	const int & deltaW,
	const int & deltaH ) const
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
Map::_Linear2WH( const uint & linear,
		      uint & w,
		      uint & h ) const
{
//debug
cout << "not implemented yet " << endl;
assert(0);
}


   /*=====================================================================*/
inline uint
Map::_WH2Linear(
	const uint & w,
	const uint & h,
	uint & linear ) const
{
	return (linear = h * (_uiMapWidth + 1) + w);
}


























