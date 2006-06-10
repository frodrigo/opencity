/***************************************************************************
                          industrialsim.h  -  description
      $Id: industrialsim.h,v 1.5 2006/03/12 23:14:18 neoneurone Exp $
                             -------------------
    begin                : dim fév 1 2004
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

#ifndef _OPENCITY_INDUSTRIALSIM_H_
	#define _OPENCITY_INDUSTRIALSIM_H_ 1

	#include "main.h"

	#include "simulator.h"


   /** This class handles the simulation of industrial zones
   */
class IndustrialSim : public Simulator  {
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



































