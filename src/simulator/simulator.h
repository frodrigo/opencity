/***************************************************************************
						simulator.h  -  description
							-------------------
	begin                : september 21th, 2003
	copyright            : (C) 2003-2010 by Duong Khang NGUYEN
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

// The simulators' contants.
// Bad values can block your system, don't change them !
#define OC_SIMULATOR_UP   70			///< 70% of levelup will be done 30% left mean leveldown
#define OC_SIMULATOR_DOWN 40			///< 40% of leveldown will be done only

// The power you get when you build it
#define OC_EPLANT_COAL_POWER		20000
#define OC_EPLANT_NUCLEAR_POWER		50000

#define OC_DEPT_POWER_CONSUMPTION	50	///< Power comsumption of fire, police, school and hospital departments


enum OPENCITY_STRUCTURE_CODE;
class BuildingLayer;
class Map;
class Structure;


//========================================================================
/** The class from which all the simulators derive. It contains common
	tests used by the micro simulators
*/
class Simulator : public Persistence
{

public:

//========================================================================
/** Each enumeration corresponds to a specific micro simulator
*/
	enum OPENCITY_SIMULATOR {
		OC_RESIDENTIAL = 0,		///< Residential micro simulator
		OC_COMMERCIAL,			///< Commercial micro simulator
		OC_INDUSTRIAL,			///< Industrial micro simulator
		OC_ELECTRIC,			///< Electric micro simulator
		OC_TRAFFIC,				///< Traffic micro simulator
		OC_SIMULATOR_NUMBER,	///< The number of micro simulators that we have
		OC_SIMULATOR_DEFAULT
	};

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
		const uint & h2 ) {}

	virtual void
	RemoveStructure(
		const uint & w1,
		const uint & h1,
		const uint & w2,
		const uint & h2 ) {}


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
		const OPENCITY_STRUCTURE_CODE enumStructCode ) const;


//========================================================================
/** Check if the structure at the specified coodinates can "level up".
Mainly, it consists of structure's size checking
*/
	const bool
	CheckLevelUp(
		const uint w,
		const uint l,
		const Structure* pStruct ) const;


//========================================================================
/** Check if the structure at the specified coodinates can "level down"
Mainly, it consists of structure's size checking
*/
	const bool
	CheckLevelDown(
		const uint w,
		const uint l,
		const Structure* pStruct ) const;


	const int &
	GetVariation() const;


//========================================================================
/** Return the current value of the micro simulator
*/
	const int
	GetValue() const;


//========================================================================
/** Return the possible max value of the micro simulator. Currently,
this method works only on the electricity micro simulator.
*/
	virtual const int
	GetMaxValue() const;


	void
	SetVariation(
		const int rcVariation );


	void
	SetValue(
		const int rcValue );


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

	SIMULATOR_STATE		_eSimState;		///< The current state of the simulator
	SDL_mutex*			_pMutexMain;	///< The global mutex
	BuildingLayer*		_pBuildLayer;	///< The building layer
	Map*				_pMapCity;		///< The city map

	static volatile int _tiVariation[Simulator::OC_SIMULATOR_NUMBER];
};

#endif



































