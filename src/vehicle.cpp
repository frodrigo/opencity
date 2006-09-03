/***************************************************************************
                          vehicle.cpp  -  description
                             -------------------
    begin                : dim mai 16 2004
    modified             : jun 9 2004
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

#include "vehicle.h"

   /*=====================================================================*/
Vehicle::Vehicle():
uiCapacity( 0 ),
enumType( VEHICLE_BUS )
{
	OPENCITY_DEBUG("ctor");
	_eGC = OC_VEHICLE_BUS;
}


   /*=====================================================================*/
Vehicle::Vehicle(
	const VEHICLE_TYPE & type ):
uiCapacity( 0 ),
enumType( type )
{
	OPENCITY_DEBUG("ctor");

	switch (type) {
		case VEHICLE_STD:
			_eGC = OC_VEHICLE_STD;
			break;
		case VEHICLE_BUS:
			_eGC = OC_VEHICLE_PICKUP;
			break;
		case VEHICLE_SPORT:
			_eGC = OC_VEHICLE_ROBBER;
			break;
		default:
			OPENCITY_DEBUG("Error game design");
			assert( 0 );
			break;
	}
}


   /*=====================================================================*/
Vehicle::~Vehicle()
{
	OPENCITY_DEBUG("dtor");
}


   /*=====================================================================*/
const bool
Vehicle::Move()
{
// Animate the vehicle each frame;
	_fCurrentW += _fDeltaW;
	_fCurrentL += _fDeltaL;
	_fCurrentH += _fDeltaH;

// Move to next WH unit if we have counted "uiFramePerUnit" times
	if (++uiNumberOfFrame == uiFramePerUnit) {
	// Move to direction as much time as specified by "uiTime"
		if (this->destCurrent._uiTime > 1) {
			--this->destCurrent._uiTime;
		//	Movement::Move2Dir( this->destCurrent );
		}
		else {
		// Use next destination
			if (uiCurrentIndex < vdest.size()-1) {
				++uiCurrentIndex;
				this->destCurrent = vdest[ uiCurrentIndex ];
				_fCurrentW = destCurrent._uiW;
				_fCurrentL = destCurrent._uiL;
				_fCurrentH = destCurrent._iHMin;
				_CalculateDelta();
			}
			else {
			// Finished, notify the caller
				OPENCITY_DEBUG("done");
				return false;
			}
		}

//debug
/*
cout << "vehicle gogogo, W: " << fCurrentW << " / H: " << fCurrentH
     << "dW : " << fDeltaW << " / dH: " << fDeltaH << endl;
*/

	   // reset the number of frame
		this->uiNumberOfFrame = 0;
	}

   // yes, we have successfully moved
	return true;
}


   /*=====================================================================*/
void
Vehicle::Start()
{
// reset the counted number of frame
	this->uiNumberOfFrame = 0;

// set the current destination to the first element of the vector
	this->uiCurrentIndex = 0;
	this->destCurrent = this->vdest[ this->uiCurrentIndex ];
	_fCurrentW = destCurrent._uiW;
	_fCurrentL = destCurrent._uiL;
	_fCurrentH = destCurrent._iHMin;
/*
	OPENCITY_DEBUG( "W: " << destCurrent._uiW << " / L: " << destCurrent._uiL <<
					"/ H: " << _fCurrentH << " / delta: " << _fDeltaH <<
					"/ Min: " << destCurrent._iHMin << " / Max: " << destCurrent._iHMax );
*/
	_CalculateDelta();
}


   /*=====================================================================*/
void
Vehicle::_CalculateDelta()
{
	static Destination destNext;		// Static to reduce memory manipulation

// Calculate the WH variations to go to next destination if applicable
	if (uiCurrentIndex+1 < vdest.size()) {
		destNext = vdest[ uiCurrentIndex+1 ];

	// Calculate the frame per unit
		this->uiFramePerUnit = (uint)(destNext._ubTraffic+10);

	// Calculate the differences of coordinates between each frame
		_fDeltaW = (destNext._uiW - _fCurrentW) / uiFramePerUnit;
		_fDeltaL = (destNext._uiL - _fCurrentL) / uiFramePerUnit;
		_fDeltaH = (float)(this->destCurrent._iHMax - this->destCurrent._iHMin) / uiFramePerUnit;

	// Set the model's rotation angle according to the next destination
		SetAngle( this->destCurrent );

	// Set the slope according to the next destination
		SetSlope( this->destCurrent, destNext );

/*
		OPENCITY_DEBUG( "W: " << destCurrent._uiW << " / L: " << destCurrent._uiL <<
						"/ H: " << _fCurrentH << " / delta: " << _fDeltaH <<
						"/ Min: " << destCurrent._iHMin << " / Max: " << destCurrent._iHMax );
*/
	}
	else {
		this->uiFramePerUnit = OC_VEHICLE_DEFAULT_SPEED;
		_fDeltaW = 0;
		_fDeltaL = 0;
		_fDeltaH = 0;
	}
}
































