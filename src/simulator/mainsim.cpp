/***************************************************************************
						mainsim.cpp  -  description
							-------------------
	begin                : febuary 21th, 2006
	copyright            : (C) 2006 by Duong-Khang NGUYEN
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

#include "mainsim.h"
#include "residentialsim.h"				// RCI, WEG and traffic management
#include "commercialsim.h"
#include "industrialsim.h"
#include "electricitysim.h"
#include "trafficsim.h"
#include "structure.h"
#include "buildinglayer.h"

#include "globalvar.h"
extern GlobalVar gVars;

#include <sstream>						// Debug string stream


   /*======================================================================*/
MainSim::MainSim(
	SDL_mutex* mutex,
	BuildingLayer* pblayer,
	Map* pmap ):
Simulator( mutex, pblayer, pmap )
{
	OPENCITY_DEBUG( "MainSim param ctor" );

// Simulators' initialization
	_tpSimulator[Simulator::OC_RESIDENTIAL]	= new ResidentialSim( mutex, pblayer, pmap );
	_tpSimulator[Simulator::OC_COMMERCIAL]	= new CommercialSim( mutex, pblayer, pmap );
	_tpSimulator[Simulator::OC_INDUSTRIAL]	= new IndustrialSim( mutex, pblayer, pmap );
	_tpSimulator[Simulator::OC_ELECTRIC]	= new ElectricitySim( mutex, pblayer, pmap );
	_tpSimulator[Simulator::OC_TRAFFIC]		= new TrafficSim( mutex, pblayer, pmap, gVars.gpPathFinder, gVars.gpMoveMgr );

	for (uint ui = 0; ui < Simulator::OC_SIMULATOR_NUMBER; ui++) {
		Simulator::_tiVariation[ui] = 0;
	}
}


   /*======================================================================*/
MainSim::~MainSim()
{
	OPENCITY_DEBUG( "MainSim dtor" );

	for (uint ui = 0; ui < Simulator::OC_SIMULATOR_NUMBER; ui++) {
		delete _tpSimulator[ui];
	}
}


   /*======================================================================*/
void
MainSim::SaveTo( std::fstream& rfs )
{
	for (uint ui = 0; ui < Simulator::OC_SIMULATOR_NUMBER; ui++) {
		_tpSimulator[ui]->SaveTo(rfs);
	}
}


   /*======================================================================*/
void
MainSim::LoadFrom( std::fstream& rfs )
{
	for (uint ui = 0; ui < Simulator::OC_SIMULATOR_NUMBER; ui++) {
		_tpSimulator[ui]->LoadFrom(rfs);
	}
}


   /*======================================================================*/
int
MainSim::Main()
{
	static uint times = 0;
	ostringstream oss;

// Call the Main method of each micro simulator
	while (this->enumSimState != SIMULATOR_RETURN) {
		if (this->enumSimState == SIMULATOR_RUNNING) {
			for (uint ui = 0; ui < Simulator::OC_SIMULATOR_NUMBER; ui++) {
				_tpSimulator[ui]->Main();
				oss << ui << "=" << (int)Simulator::_tiVariation[ui] << " ";
			}
		}
		OPENCITY_DEBUG( "RCIET: " << oss.str() );
		oss.str("");

	// Refresh the simulator values every 5 turns
		if (times == 0)
			RefreshSimValue();
	
	// Wait a bit
		Simulator::RCIDelay();
		times = (times+1) % 5;
	} // while

	return 0;
}


   /*======================================================================*/
void
MainSim::AddStructure
(
	const uint w1, const uint l1,
	const uint w2, const uint l2,
	const OPENCITY_SIMULATOR sim
)
{
	if (sim == OC_SIMULATOR_DEFAULT) {
		for (uint ui = 0; ui < Simulator::OC_SIMULATOR_NUMBER; ui++) {
			_tpSimulator[ui]->AddStructure( w1, l1, w2, l2 );
		}
	}
	else {
		_tpSimulator[sim]->AddStructure( w1, l1, w2, l2 );
	}
}


   /*======================================================================*/
void
MainSim::RemoveStructure
(
	const uint w1, const uint l1,
	const uint w2, const uint l2,
	const OPENCITY_SIMULATOR sim
)
{
	if (sim == OC_SIMULATOR_DEFAULT) {
		for (uint ui = 0; ui < Simulator::OC_SIMULATOR_NUMBER; ui++) {
			_tpSimulator[ui]->RemoveStructure( w1, l1, w2, l2 );
		}
	}
	else {
		_tpSimulator[sim]->RemoveStructure( w1, l1, w2, l2 );
	}
}


   /*======================================================================*/
void
MainSim::Run()
{
	for (uint ui = 0; ui < Simulator::OC_SIMULATOR_NUMBER; ui++) {
		_tpSimulator[ui]->Run();
	}
	Simulator::Run();
}


   /*======================================================================*/
void
MainSim::Stop()
{
	for (uint ui = 0; ui < Simulator::OC_SIMULATOR_NUMBER; ui++) {
		_tpSimulator[ui]->Stop();
	}
	Simulator::Stop();
}


   /*======================================================================*/
void
MainSim::Return()
{
	for (uint ui = 0; ui < Simulator::OC_SIMULATOR_NUMBER; ui++) {
		_tpSimulator[ui]->Return();
	}
	Simulator::Return();
}


   /*======================================================================*/
void
MainSim::RefreshSimValue()
{
/*
	Iterate over each structure and calculate the micro simulators' values.
The values of the micro simulators depend on the level of the structure.
The calculation is exact however, the micro simulators can do some
fluctuations in between.
*/
	int resVal = 0, comVal = 0, indVal = 0;
	int eleVal = 0;
	uint linear = 0, maxLinear = 0;
	uint level = 0, eleMultiplier = 0;
	Structure* pstruct = NULL;


// FOR each structre DO
	maxLinear = pbuildlayer->GetMaxLinear();
	for ( linear = 0; linear <= maxLinear; linear++ ) {
		pstruct = pbuildlayer->GetLinearStructure(linear);
		if (pstruct == NULL)
			continue;

		level = pstruct->GetLevel();
		eleMultiplier = (pstruct->IsSet(OC_STRUCTURE_E)) ? 1 : 0;
		switch (pstruct->GetCode()) {
			case OC_STRUCTURE_RES:
				resVal += level;
				eleVal -= level * eleMultiplier;
				break;

			case OC_STRUCTURE_COM:
				comVal += level;
				eleVal -= level * eleMultiplier;
				break;

			case OC_STRUCTURE_IND:
				indVal += level;
				eleVal -= level * eleMultiplier;
				break;

			case OC_STRUCTURE_FIREDEPT:
			case OC_STRUCTURE_POLICEDEPT:
			case OC_STRUCTURE_EDUCATIONDEPT:
			case OC_STRUCTURE_HOSPITALDEPT:
				eleVal -= OC_DEPT_POWER_CONSUMPTION * eleMultiplier;
				break;

			case OC_STRUCTURE_PART:
			case OC_STRUCTURE_ELINE:
				eleVal -= 1 * eleMultiplier;
				break;

			case OC_STRUCTURE_EPLANT_COAL:
				eleVal += OC_EPLANT_COAL_POWER;
				break;
			case OC_STRUCTURE_EPLANT_NUCLEAR:
				eleVal += OC_EPLANT_NUCLEAR_POWER;
				break;

		// Nothing to do here
			case OC_STRUCTURE_PARK:
			case OC_STRUCTURE_FLORA:
			case OC_STRUCTURE_ROAD:
			case OC_STRUCTURE_TEST:			// Development test feature
				break;

			default:
				OPENCITY_DEBUG( "What is this ?" );
				assert( 0 );
		}
	} // for

// Update the micro simulators' values
	_tpSimulator[Simulator::OC_RESIDENTIAL]->SetValue(resVal);
	_tpSimulator[Simulator::OC_COMMERCIAL]->SetValue(comVal);
	_tpSimulator[Simulator::OC_INDUSTRIAL]->SetValue(indVal);
	_tpSimulator[Simulator::OC_ELECTRIC]->SetValue(eleVal);
}


   /*======================================================================*/
const int
MainSim::GetValue
(
	const OPENCITY_SIMULATOR sim
) const
{
	return _tpSimulator[sim]->GetValue();
}


   /*======================================================================*/
const int
MainSim::GetMaxValue
(
	const OPENCITY_SIMULATOR sim
) const
{
	return _tpSimulator[sim]->GetMaxValue();
}



































