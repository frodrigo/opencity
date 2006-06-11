/***************************************************************************
                          electricitysim.h  -  description
      $Id$
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


//========================================================================
/** Overload the base method in order to save our properties
	\param rfs A reference to a file stream which is ready for writing
*/
	void
	SaveTo( std::fstream& rfs );


//========================================================================
/** Overload the base method in order to load our properties
	\param rfs A reference to a file stream which is ready for reading
*/
	void
	LoadFrom( std::fstream& rfs );


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
	uint	_uiNumberEPlant;
	int		_iValueMax;
	deque<pair<uint, uint> >	dequepairui;
	vector<pair<uint, uint> >	vectorpairuiEPlant;

   /*=====================================================================*/
	bool
	dequeContain( const pair<uint, uint> &);

};

#endif

































