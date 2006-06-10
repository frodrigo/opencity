/***************************************************************************
                          electricitysim.h  -  description
      $Id: electricitysim.h,v 1.7 2006/03/12 23:14:18 neoneurone Exp $
                             -------------------
    begin                : mar fev 3 2004
    copyright            : (C) 2004 by Duong-Khang NGUYEN
    email                : neoneurone @ users sourceforge net
    internal sim version : 0.0.2
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#ifndef _OPENCITY_ELECTRICITYSIM_H_
#define _OPENCITY_ELECTRICITYSIM_H_ 1

	#include "main.h"
	#include "simulator.h"

	#include <deque>
	#include <vector>

using namespace std;

   /** This class is a threaded mini-simulator which handles the transmission
       of electricity in the game
   */
class ElectricitySim : public Simulator  {
public:
	ElectricitySim(
		SDL_mutex* mutex,
		BuildingLayer* pblayer,
		Map* pmap );
	~ElectricitySim();

	int
	Main();

	void
	AddStructure(
		const uint & w1,
		const uint & h1,
		const uint & w2,
		const uint & h2 );

	void
	RemoveStructure(
		const uint & w1,
		const uint & h1,
		const uint & w2,
		const uint & h2 );


private:
	uint uiNumberEPlant;
	int iValueMax;
	deque<pair<uint, uint> > dequepairui;
	vector<pair<uint, uint> > vectorpairuiEPlant;

   /*=====================================================================*/
	bool
	dequeContain( const pair<uint, uint> &);

};

#endif

































