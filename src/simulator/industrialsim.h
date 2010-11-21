/***************************************************************************
						industrialsim.h  -  description
							-------------------
	begin                : feb 1st, 2004
	copyright            : (C) 2004 by Duong Khang NGUYEN
	email                : neoneurone @ gmail com

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

#ifndef _OPENCITY_INDUSTRIALSIM_H_
#define _OPENCITY_INDUSTRIALSIM_H_ 1

#include "main.h"
#include "simulator.h"


/** This class handles the simulation of industrial zones
*/
class IndustrialSim : public Simulator
{
public:
	IndustrialSim(
		SDL_mutex* mutex,
		BuildingLayer* pblayer,
		Map* pmap );
	~IndustrialSim();

	int
	Main();

	void
	RemoveStructure(
		const uint & w1,
		const uint & h1,
		const uint & w2,
		const uint & h2 );


};

#endif
