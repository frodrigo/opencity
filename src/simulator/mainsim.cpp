/***************************************************************************
                          mainsim.cpp  -  description
      $Id: mainsim.cpp,v 1.2 2006/06/05 09:58:06 neoneurone Exp $
                             -------------------
    begin                : 21 feb 2006
    copyright            : (C) 2006 by Duong-Khang NGUYEN
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

#include "mainsim.h"

#include "residentialsim.h"				// RCI, WEG and traffic management
#include "commercialsim.h"
#include "industrialsim.h"
#include "electricitysim.h"
#include "trafficsim.h"

extern PathFinder* gpPathFinder;		// global pathfinder class
extern MovementManager* gpMoveMgr;		// global movement manager


   /*======================================================================*/
MainSim::MainSim(
	SDL_mutex* mutex,
	BuildingLayer* pblayer,
	Map* pmap ):
Simulator( mutex, pblayer, pmap )
{
	OPENCITY_DEBUG( "MainSim param ctor" );

// Simulators' initialization
	_tpSimulator[OC_MICROSIM_RES] = new ResidentialSim( mutex, pblayer, pmap );
	_tpSimulator[OC_MICROSIM_COM] = new CommercialSim( mutex, pblayer, pmap );
	_tpSimulator[OC_MICROSIM_IND] = new IndustrialSim( mutex, pblayer, pmap );
	_tpSimulator[OC_MICROSIM_ELE] = new ElectricitySim( mutex, pblayer, pmap );
	_tpSimulator[OC_MICROSIM_TRA] = new TrafficSim( mutex, pblayer, pmap, gpPathFinder, gpMoveMgr );
}


   /*======================================================================*/
MainSim::~MainSim()
{
	OPENCITY_DEBUG( "MainSim dtor" );

	for (uint ui = 0; ui < OC_MICROSIM_MAX; ui++) {
		delete _tpSimulator[ui];
	}
}


   /*======================================================================*/
int
MainSim::Main()
{
// Call the Main method of each micro simulator
	while (this->enumSimState != SIMULATOR_RETURN) {
		if (this->enumSimState == SIMULATOR_RUNNING) {
			for (uint ui = 0; ui < OC_MICROSIM_MAX; ui++) {
				_tpSimulator[ui]->Main();
			}
		}

	// Wait a bit
		Simulator::RCIDelay();
	} // while

	return 0;
}


   /*======================================================================*/
void
MainSim::AddStructure
(
	const uint& w1, const uint& h1,
	const uint& w2, const uint& h2,
	const OPENCITY_MAINSIM_MICROSIM& sim
)
{
	if (sim == OC_MICROSIM_DEFAULT) {
		for (uint ui = 0; ui < OC_MICROSIM_MAX; ui++) {
			_tpSimulator[ui]->AddStructure( w1, h1, w2, h2 );
		}
	}
	else {
		_tpSimulator[sim]->AddStructure( w1, h1, w2, h2 );
	}
}


   /*======================================================================*/
void
MainSim::RemoveStructure
(
	const uint& w1, const uint& h1,
	const uint& w2, const uint& h2,
	const OPENCITY_MAINSIM_MICROSIM& sim
)
{
	if (sim == OC_MICROSIM_DEFAULT) {
		for (uint ui = 0; ui < OC_MICROSIM_MAX; ui++) {
			_tpSimulator[ui]->RemoveStructure( w1, h1, w2, h2 );
		}
	}
	else {
		_tpSimulator[sim]->RemoveStructure( w1, h1, w2, h2 );
	}
}


   /*======================================================================*/
void
MainSim::Run()
{
	for (uint ui = 0; ui < OC_MICROSIM_MAX; ui++) {
		_tpSimulator[ui]->Run();
	}
	Simulator::Run();
}


   /*======================================================================*/
void
MainSim::Stop()
{
	for (uint ui = 0; ui < OC_MICROSIM_MAX; ui++) {
		_tpSimulator[ui]->Stop();
	}
	Simulator::Stop();
}


   /*======================================================================*/
void
MainSim::Return()
{
	for (uint ui = 0; ui < OC_MICROSIM_MAX; ui++) {
		_tpSimulator[ui]->Return();
	}
	Simulator::Return();
}


   /*======================================================================*/
const int &
MainSim::GetValue
(
	const OPENCITY_MAINSIM_MICROSIM& sim
) const
{
	return _tpSimulator[sim]->GetValue();
}



































