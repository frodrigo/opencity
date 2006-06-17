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
   // animate the vehicle each frame;
	this->fCurrentW += this->fDeltaW;
	this->fCurrentH += this->fDeltaH;

   // move to next WH unit if we have counted "uiFramePerUnit" times
	if (++uiNumberOfFrame == uiFramePerUnit) {
	   // move to direction as much time as specified by "uiTime"
		if (this->destCurrent._uiTime > 1) {
			--this->destCurrent._uiTime;
			Movement::Move2Dir( this->destCurrent );
		}
		else {
		   // use next destination
			if (uiCurrentIndex < vdest.size()-1) {
				++uiCurrentIndex;
				this->destCurrent = vdest[ uiCurrentIndex ];
				fCurrentW = destCurrent._uiW;
				fCurrentH = destCurrent._uiL;
				_CalculateDelta();
			}
			else {
			   // finished, notify the caller
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
	this->fCurrentW = destCurrent._uiW;
	this->fCurrentH = destCurrent._uiL;

	_CalculateDelta();
}


   /*=====================================================================*/
void
Vehicle::_CalculateDelta()
{
// calculate the WH variations to go to next destination if applicable
	if (uiCurrentIndex+1 < vdest.size()) {
	// calculate the frame per unit
		this->uiFramePerUnit = (uint)(vdest[ uiCurrentIndex+1 ]._ubTraffic+5);

		this->fDeltaW = (vdest[ uiCurrentIndex+1 ]._uiW - fCurrentW)
			/ uiFramePerUnit;
		this->fDeltaH = (vdest[ uiCurrentIndex+1 ]._uiL - fCurrentH)
			/ uiFramePerUnit;

	// Set the model's rotation angle according to the next destination
		SetAngle( this->destCurrent );
	}
	else {
		this->uiFramePerUnit = OC_VEHICLE_DEFAULT_SPEED;
		this->fDeltaW = 0;
		this->fDeltaH = 0;
	}
}
































