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
	this->enumGraphicCode = OC_VEHICLE_BUS;
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
			this->enumGraphicCode = OC_VEHICLE_STD;
			break;
		case VEHICLE_BUS:
			this->enumGraphicCode = OC_VEHICLE_BUS;
			break;
		case VEHICLE_SPORT:
			this->enumGraphicCode = OC_VEHICLE_SPORT;
			break;
		default:
			OPENCITY_DEBUG("Error game design");
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
		if (this->destCurrent.uiTime > 1) {
			--this->destCurrent.uiTime;
			Movement::Move2Dir( this->destCurrent );
		}
		else {
		   // use next destination
			if (uiCurrentIndex < vdest.size()-1) {
				++uiCurrentIndex;
				this->destCurrent = vdest[ uiCurrentIndex ];
				fCurrentW = destCurrent.uiW;
				fCurrentH = destCurrent.uiH;
				this->vehicleCalculateDelta();
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

   // set the current destination to the first element of
   // the vector
	this->uiCurrentIndex = 0;
	this->destCurrent = this->vdest[ this->uiCurrentIndex ];
	this->fCurrentW = destCurrent.uiW;
	this->fCurrentH = destCurrent.uiH;

	this->vehicleCalculateDelta();
}


   /*=====================================================================*/
void
Vehicle::vehicleCalculateDelta()
{
   // calculate the WH variations to go to next destination if applicable
	if (uiCurrentIndex+1 < vdest.size()) {
	   // calculate the frame per unit
		this->uiFramePerUnit = (uint)(vdest[ uiCurrentIndex+1 ].ubTraffic+5);

		this->fDeltaW = (vdest[ uiCurrentIndex+1 ].uiW - fCurrentW)
			/ uiFramePerUnit;
		this->fDeltaH = (vdest[ uiCurrentIndex+1 ].uiH - fCurrentH)
			/ uiFramePerUnit;
	}
	else {
		this->uiFramePerUnit = OC_VEHICLE_DEFAULT_SPEED;
		this->fDeltaW = 0;
		this->fDeltaH = 0;
	}
}
































