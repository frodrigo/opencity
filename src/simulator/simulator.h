/***************************************************************************
                          simulator.h  -  description
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

#ifndef _OPENCITY_SIMULATOR_H_
#define _OPENCITY_SIMULATOR_H_ 1

#include "main.h"

#include "persistence.h"

// the maximum range of other structures
#define OC_R_C_RANGE 4
#define OC_R_I_RANGE 7
#define OC_R_P_RANGE 3

#define OC_C_R_RANGE 4
#define OC_C_I_RANGE 8
#define OC_C_P_RANGE 2

#define OC_I_R_RANGE 7
#define OC_I_C_RANGE 8
#define OC_I_P_RANGE 4

#define OC_E_E_RANGE   1	// for electricity simulation
#define OC_E_RCI_RANGE 2

#define OC_P_RCIP_RANGE 2	// for traffic simulation

// simulators' defines.
// bad values can block your system, don't change them !
#define OC_SIMULATOR_UP   70			///< 70% of levelup will be done 30% left mean leveldown
#define OC_SIMULATOR_DOWN 40			///< 40% of leveldown will be done only

#define OC_EPLANT_COAL_POWER 1000		///< The power you get when you build it
#define OC_EPLANT_COAL_RANGE 2			///< For building and destroying


enum OPENCITY_STRUCTURE_CODE;
class BuildingLayer;
class Map;
class Structure;

//========================================================================
/** The class from which all the simulators derive. It contains common
	tests used by the micro simulators
*/
class Simulator : public Persistence {
public:
	enum SIMULATOR_STATE {
		SIMULATOR_RUNNING,
		SIMULATOR_STOPED,
		SIMULATOR_RETURN
	};

	Simulator();		///< do NOT use this
	Simulator(
		SDL_mutex* mutex,
		BuildingLayer* pblayer,
		Map* pmap );
	virtual ~Simulator();


//========================================================================
/** Save the data to the specified fstream
	\param rfs A reference to a file stream which is ready for writing
*/
	void
	SaveTo( std::fstream& rfs );


//========================================================================
/** Load the data from the specified stream
	\param rfs A reference to a file stream which is ready for reading
*/
	void
	LoadFrom( std::fstream& rfs );


	virtual int
	Main() = 0;


	virtual void
	AddStructure(
		const uint & w1,
		const uint & h1,
		const uint & w2,
		const uint & h2 ) {};

	virtual void
	RemoveStructure(
		const uint & w1,
		const uint & h1,
		const uint & w2,
		const uint & h2 ) {};


	void
	Run();

	void
	Stop();

	void
	Return();


//========================================================================
/** Given the (w, l) coordinates and a range in integer, this method
	return "true" if there's at least one required structure within
	the range and "false" otherwise
*/
	const bool
	CheckRange(
		const uint & w,
		const uint & l,
		const uint & range,
		const OPENCITY_STRUCTURE_CODE & enumStructCode ) const;


//========================================================================
/** Check if the structure at the specified coodinates can "level up"
*/
	const bool
	CheckLevelUp(
		const uint & w,
		const uint & l,
		const Structure* pStruct ) const;


//========================================================================
/** Check if the structure at the specified coodinates can "level down"
*/
	const bool
	CheckLevelDown(
		const uint & w,
		const uint & l,
		const Structure* pStruct ) const;


//========================================================================
/* not used
Given the surface (w, l, w2, l2) this method checks if it contains
	only the specified structure or nothing.
\sa Layer::ContainStructureOnly()
\sa BuildingLayer::ContainStructureOnly();

	const bool
	ContainStructureOnly(
		const uint & w,
		const uint & l,
		const uint & w2,
		const uint & l2,
		const OPENCITY_STRUCTURE_CODE & enumStructCode ) const;
*/

	const int &
	GetVariation() const;


//========================================================================
/** Return the global total value of the micro simulator
*/
	const int &
	GetValue() const;

	
	void
	SetVariation(
		const int & rciVariation );

		
	void
	SetValue(
		const int & rciValue );


   /*=====================================================================*/
   /*                        STATIC      METHODS                          */
   /*=====================================================================*/
	static int
	ThreadWrapper(
		void* pSim );

		
//========================================================================
/** Each RCI (residential, commercial, industrial) micro simulator should
	call this at the end of each treatement so that the main thread can
	execute.
*/
	static void
	RCIDelay( void );


protected:
	int _iVariation;	///< The average variation of the structures stimulated
	int _iValue;		///< The current global value of the simulator

	SIMULATOR_STATE enumSimState;			///< The current state of the simulator
	SDL_mutex* mutexMain;					///< Points to the global mutex
	BuildingLayer* pbuildlayer;
	Map* pmapOfCity;
};

#endif



































