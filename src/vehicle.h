/***************************************************************************
                          vehicle.h  -  description
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

#ifndef _OPENCITY_VEHICLE_H_
#define _OPENCITY_VEHICLE_H_ 1

#include "movement.h"

#define OC_VEHICLE_DEFAULT_SPEED	10

   //========================================================================
   /** Euh, can a Vehicle do any Movement ?
   */
class Vehicle: public Movement {
public:
	enum VEHICLE_TYPE {
		VEHICLE_STD,				///< standard vehicle
		VEHICLE_BUS,				///< a bus
		VEHICLE_SPORT,				///< high speed sport vehicle
		VEHICLE_NUMBER
	};

	Vehicle();
	Vehicle( const VEHICLE_TYPE & type );
	virtual ~Vehicle();


	const bool
	Move();

	void
	Start();


protected:
	uint uiCapacity;
	VEHICLE_TYPE enumType;


private:
	void vehicleCalculateDelta();

};

#endif
































