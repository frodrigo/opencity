/***************************************************************************
						electricitysim.h  -  description
							-------------------
	begin                : march 3rd, 2004
	copyright            : (C) 2004-2008 by Duong-Khang NGUYEN
	email                : neoneurone @ users sourceforge net
	internal sim version : 0.0.2

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

#ifndef _OPENCITY_ELECTRICITYSIM_H_
#define _OPENCITY_ELECTRICITYSIM_H_ 1

#include "main.h"
#include "simulator.h"

#include <deque>
#include <vector>


//========================================================================
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


//========================================================================
/** Return the possible max value of the micro simulator.
*/
	const int
	GetMaxValue() const;


private:
	uint								_uiNumberEPlant;
	int									_iValueMax;
	std::deque<std::pair<uint, uint> >	_dequepairui;
	std::vector<std::pair<uint, uint> >	_vectorpairuiEPlant;

   /*=====================================================================*/
	bool
	dequeContain( const std::pair<uint, uint> &);

};

#endif

































