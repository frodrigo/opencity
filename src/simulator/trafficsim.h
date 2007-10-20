/***************************************************************************
						trafficsim.h  -  description
							-------------------
	begin                : may 1st, 2004
	copyright            : (C) 2003-2007 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_TRAFFICSIM_H_
#define _OPENCITY_TRAFFICSIM_H_ 1

#include "main.h"
#include "simulator.h"


// 10% of time, we create a new vehicle
#define OC_TSIM_VEHICLE_CHANCE		10

// Minimum of traffic in order to generate a vehicle starting from a point of the map
#define OC_TSIM_TRAFFIC_MIN			50


class PathFinder;
class MovementManager;


//========================================================================
/** This class generates an artificial traffic in the game. The traffic level
	of each unit (square) depends on the structures' level around it
*/
class TrafficSim : public Simulator  {
public:
	TrafficSim(
		SDL_mutex* mutex,
		BuildingLayer* pblayer,
		Map* pmap,
		PathFinder* pf,
		MovementManager* mm );

	virtual ~TrafficSim();

	int Main();


private:
	PathFinder* ppf;
	MovementManager* pmm;

   /*=====================================================================*/
   /*                        PRIVATE     METHODS                          */
   /*=====================================================================*/
	void trafficsimNewVehicle(
		const uint& w,
		const uint& h );
};

#endif






















