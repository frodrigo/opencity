/***************************************************************************
                          mainsim.h  -  description
      $Id: mainsim.h,v 1.1 2006/03/12 23:14:18 neoneurone Exp $
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

#ifndef _OPENCITY_MAINSIM_H_
#define _OPENCITY_MAINSIM_H_ 1

#include "main.h"

#include "simulator.h"


//========================================================================
/** Handles the simulation of the commercial sub-system.
*/
class MainSim : public Simulator  {
public:
	enum OPENCITY_MAINSIM_MICROSIM {
		OC_MICROSIM_RES = 0,
		OC_MICROSIM_COM,
		OC_MICROSIM_IND,
		OC_MICROSIM_ELE,
		OC_MICROSIM_TRA,
		OC_MICROSIM_MAX,		///< Currently, we have 5 micro sims
		OC_MICROSIM_DEFAULT
	};

//========================================================================
/** Constructs a new MainSim object.
	\param mutex A pointer to a SDL_mutex which should be used to prevent
that the gobal simulator's data is overwritten by multiple micro simulators
	\param pblayer A pointer to the BuildingLayer
	\param pmap A pointer to the gobal Map object
	\todo Remove pblayer and pmap. Use global pointer for them
*/
	MainSim(
		SDL_mutex* mutex,
		BuildingLayer* pblayer,
		Map* pmap );


//========================================================================
/** Destructs a MainSim object.
*/
	~MainSim();


//========================================================================
/** Adds a micro simulator to the main simulator's loop
	\param pSim The constant pointer of the micro simulator to add
	\return True if pSim has been added succesfully, false otherwise
*/
	int
	Main();


//========================================================================
/** Notifies the micro simulators that a new structure has been added to the
system
	\param w1,h1,w2,h2 The area used by the new structure
*/
	void
	AddStructure(
		const uint& w1, const uint& h1,
		const uint& w2, const uint& h2,
		const OPENCITY_MAINSIM_MICROSIM& sim = OC_MICROSIM_DEFAULT);


//========================================================================
/** Notify the micro simulators that a structure has been removed from the
system
	\param w1,h1,w2,h2 The area used by the structure
*/
	void
	RemoveStructure(
		const uint& w1, const uint& h1,
		const uint& w2, const uint& h2,
		const OPENCITY_MAINSIM_MICROSIM& sim = OC_MICROSIM_DEFAULT );


//========================================================================
/** Overload the parent's method
	\see Simulator::Run()
*/
	void
	Run();


//========================================================================
/** Overload the parent's method
	\see Simulator::Stop()
*/
	void
	Stop();


//========================================================================
/** Overload the parent's method
	\see Simulator::Return()
*/
	void
	Return();


//========================================================================
/** Return the global total value of the micro simulator
	\param sim The micro simulator to query the information
	\return The value
*/
	const int &
	GetValue(const OPENCITY_MAINSIM_MICROSIM& sim) const;


private:
	Simulator* _tpSimulator[OC_MICROSIM_MAX];		///< Table of pointers to Simulator object
};

#endif

































