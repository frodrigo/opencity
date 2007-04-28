/***************************************************************************
						trafficsim.cpp  -  description
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

// Useful enumerations
#include "opencity_direction.h"
#include "opencity_structure_type.h"

// OpenCity headers
#include "trafficsim.h"
#include "buildinglayer.h"
#include "../map.h"					// We want our own map.h
#include "pathstructure.h"
#include "pathfinder.h"
#include "movementmanager.h"
#include "vehicle.h"


   /*=====================================================================*/
TrafficSim::TrafficSim(
	SDL_mutex* mutex,
	BuildingLayer* pblayer,
	Map* pmap,
	PathFinder* pf,
	MovementManager* mm ):
Simulator( mutex, pblayer, pmap ),
ppf( pf ),
pmm( mm )
{
	OPENCITY_DEBUG( "TSim param ctor" );
}


   /*=====================================================================*/
TrafficSim::~TrafficSim()
{
	OPENCITY_DEBUG( "TSim dtor" );
}


   /*=====================================================================*/
int
TrafficSim::Main()
{
	static uint w, h;
	static uint w1, h1, w2, h2;
	static uint startW, startH;
	static Structure* pstruct;
	static PathStructure* ppathstruct;
	static int iTrafficValue;
	static int iNumberPath;


	if (this->enumSimState != SIMULATOR_RUNNING)
		return 0;


// get a random road structure
	pstruct = pbuildlayer->GetRandomStructure(w, h, OC_STRUCTURE_ROAD );
	if (pstruct == NULL)
		return 0;

// Try to lock the mutex to prevent the others from deleting the structure
// pointed by "pstruct" while we're playing with
	SDL_LockMutex( this->mutexMain );

// Save the starting point for pathfinding
	startW = w;	startH = h;

// Convert the pstruct to the correct structure
	ppathstruct = (PathStructure*)pstruct;

// Get the surface around the structure we have
	w1 = w; h1 = h;
	this->pmapOfCity->GetPossibleWH(
		w1, h1, -OC_P_RCIP_RANGE, -OC_P_RCIP_RANGE );
	w2 = w; h2 = h;
	this->pmapOfCity->GetPossibleWH(
		w2, h2,  OC_P_RCIP_RANGE,  OC_P_RCIP_RANGE );

// Calculate the traffic
	iTrafficValue = 0;
	iNumberPath = 0;	// this one is always > 0
	for ( w = w1; w <= w2; w++ ) {
		for ( h = h1; h <= h2; h++ ) {
		// we reuse the "pstruct" variable here
			pstruct = pbuildlayer->GetStructure( w, h );
			if (pstruct != NULL)
			switch (pstruct->GetCode()) {
				case OC_STRUCTURE_ROAD:
					iNumberPath++;
					break;
				case OC_STRUCTURE_RES:
				case OC_STRUCTURE_COM:
				case OC_STRUCTURE_IND:
					iTrafficValue += pstruct->GetLevel();
					break;

				default:
				// keep gcc happy
					break;
			}
		} // for h
	} // for w

// Since "ubNumberPath" is always > 0
	iTrafficValue = iTrafficValue / iNumberPath;

// give the current path structure the traffic value
// it's always >= 0 since
// since the structure's level is always >= 0
	ppathstruct->SetTraffic((OC_UBYTE)iTrafficValue );

// let the others run !
	SDL_UnlockMutex( this->mutexMain );

// WARNING: the pathfinderShortestPath() need the unlocked mutex !
// Are we going to create a new vehicle ?
	if ((iTrafficValue > OC_TSIM_TRAFFIC_MIN) and ((rand() % 100) < OC_TSIM_VEHICLE_CHANCE )) {
		trafficsimNewVehicle(startW, startH);
		_tiVariation[Simulator::OC_TRAFFIC]--;
	}


	return 0;
}


   /*=====================================================================*/
void TrafficSim::trafficsimNewVehicle(
	const uint& w,
	const uint& h )
{
	vector<Destination> vdest;
	PathStructure* pstruct;
	uint w2, h2;
	int iRandom;
	Vehicle* pvehicle;

// IF the manager is full THEN
	if (pmm->IsFull())
		return;

// try to get a destination
// NOTE: here we are sure that GetRandomStructure() returns
//       a PathStructure* or NULL
//       That's why whe doesn't need to use dynamic_cast<>
	pstruct = (PathStructure*)pbuildlayer->GetRandomStructure(w2, h2, OC_STRUCTURE_ROAD );
	if (pstruct == NULL)
		return;

// buses prefer short distance
	iRandom = rand() % Vehicle::VEHICLE_NUMBER;
	if ( iRandom == Vehicle::VEHICLE_BUS) {
		this->ppf->findShortestPath(
			w, h, w2, h2,
			vdest,
			PathFinder::OC_DISTANCE );
	}
// sport vehicle prefer less traffic
	else if ( (iRandom == Vehicle::VEHICLE_SPORT) || (iRandom == Vehicle::VEHICLE_STD) ) {
		this->ppf->findShortestPath(
			w, h, w2, h2,
			vdest,
			PathFinder::OC_TRAFFIC );
	}

// now create the new vehicle if a path was found
	if ( vdest.size() > 0 ) {
		pvehicle = new Vehicle((Vehicle::VEHICLE_TYPE)iRandom );
		pvehicle->SetPath( vdest );		// path init
		pvehicle->Start();				// vehicle init
		if (pmm->Add( pvehicle ) < 0) {
			OPENCITY_DEBUG("WARNING: The movement manager is full");
			delete pvehicle;
		}
	}
}


























