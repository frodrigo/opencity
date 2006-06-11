/***************************************************************************
                          residentialsim.h  -  description
      $Id$
                             -------------------
    begin                : dim sep 21 2003
    copyright            : (C) 2003-2005 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_RESIDENTIALSIM_H_
	#define _OPENCITY_RESIDENTIALSIM_H_ 1

	#include "main.h"

	#include "simulator.h"


   /** This class handles the simulation of residential zones
   */
class ResidentialSim : public Simulator  {
public: 
	ResidentialSim(
		SDL_mutex* mutex,
		BuildingLayer* pblayer,
		Map* pmap );

	virtual ~ResidentialSim();

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






















