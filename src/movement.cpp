/***************************************************************************
                          movement.cpp  -  description
      $Id$
                             -------------------
    begin                : dim mai 16 2004
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


#include "movement.h"


   /*=====================================================================*/
Movement::Movement():
_fRY( 0 ), _fRX( 0 ), _fRZ( 0 ),
_fTX( 0 ), _fTY( 0 ), _fTZ( 0 ), 
_eDir( OC_DIR_S )
{
	OPENCITY_DEBUG("ctor");
}


   /*=====================================================================*/
Movement::~Movement()
{
	OPENCITY_DEBUG("dtor");
}


   /*=====================================================================*/
void
Movement::SetPath(
	vector<Destination> newPath )
{
   // WARNING: Destination must not use pointers
	this->vdest = newPath;
}


   /*=====================================================================*/
const OPENCITY_GRAPHIC_CODE &
Movement::GetGraphicCode() const
{
	return _eGC;
}


   /*=====================================================================*/
void
Movement::SetAngle( const Destination & rD )
{
	_eDir = rD._eDir;

	switch (_eDir) {
	case OC_DIR_N:
		_fRY = 180;
		_fTX = 1; _fTY = 0; _fTZ = 1;
		break;

	case OC_DIR_E:
		_fRY =  90;
		_fTX = 0; _fTY = 0; _fTZ = 1;
		break;

	case OC_DIR_S:
		_fRY = 0;
		_fTX = 0; _fTY = 0; _fTZ = 0;
		break;

	case OC_DIR_W:
		_fRY = -90;
		_fTX = 1; _fTY = 0; _fTZ = 0;
		break;

	case OC_DIR_SE:
	case OC_DIR_SW:
	case OC_DIR_NW:
	case OC_DIR_NE:
	default:
		OPENCITY_DEBUG( "Game design error" );
		assert( 0 );
		break;
	}
}


   /*=====================================================================*/
void
Movement::SetSlope(
	const Destination & rcA,
	const Destination & rcB )
{
	static int sign;

	if (rcA._iHMin != rcA._iHMax) {
		if (rcA._iHMin > rcB._iHMin) {
			sign = 1;
		} 
		else if (rcB._iHMin != rcB._iHMax) {
			sign = -1;
		}
		else
			sign = 0;

		switch (_eDir) {
		case OC_DIR_N:
		case OC_DIR_S:
			_fRX =  0; _fRZ = 45;
			break;

		case OC_DIR_E:
		case OC_DIR_W:
			_fRX = sign * 45; _fRZ = 0;
			break;

		case OC_DIR_SE:
		case OC_DIR_SW:
		case OC_DIR_NW:
		case OC_DIR_NE:
		default:
			OPENCITY_DEBUG( "Game design error" );
			assert( 0 );
			break;
		}
	}
	else {
		_fRX = 0; _fRZ = 0;
	}
}


   /*=====================================================================*/
   /*                          STATIC    METHODS                          */
   /*=====================================================================*/
/* unused
void
Movement::Move2Dir(
	Destination & rD )
{
	OPENCITY_DEBUG("called");

// Move to the indicated direction
	switch (rD._eDir) {
		case OC_DIR_N:	rD._uiL--;				break;
		case OC_DIR_NE:	rD._uiL--;	rD._uiW++;	break;
		case OC_DIR_E:	rD._uiW++;				break;
		case OC_DIR_SE:	rD._uiW++;	rD._uiL++;	break;
		case OC_DIR_S:	rD._uiL++;				break;
		case OC_DIR_SW:	rD._uiL++;	rD._uiW--;	break;
		case OC_DIR_W:	rD._uiW--;				break;
		case OC_DIR_NW:	rD._uiW--;	rD._uiL--;	break;
		default: break;
	}
}
*/


































