/***************************************************************************
                          simulator.cpp  -  description
      $Id: simulator.cpp,v 1.13 2006/06/05 09:58:06 neoneurone Exp $
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

#include "simulator.h"

#include "buildinglayer.h"
#include "../map.h"				// We need to specify the path because we want our own map.h
#include "structure.h"

#include "propertymanager.h"

#include <cmath>				// For log10


	extern OC_FLOAT gfMsSimDelayMax;
	extern PropertyManager* gpPropertyMgr;


   /*=====================================================================*/
Simulator::Simulator():
iVariation( 0 ),
iValue( 0 ),
enumSimState( SIMULATOR_STOPED ),
mutexMain( NULL )
{
	OPENCITY_DEBUG( "Sim ctor , do not use" );
}


   /*=====================================================================*/
Simulator::Simulator( SDL_mutex* mutex, BuildingLayer* pblayer, Map* pmap ):
iVariation( 0 ),
iValue( 0 ),
enumSimState( SIMULATOR_STOPED ),
mutexMain( mutex ),
pbuildlayer( pblayer ),
pmapOfCity( pmap )
{
	OPENCITY_DEBUG( "Sim param ctor" );

   // it is critical to have a functional mutex
	assert( mutex != NULL );
}


   /*=====================================================================*/
Simulator::~Simulator()
{
	OPENCITY_DEBUG( "Sim dtor" );
	mutexMain = NULL;
}


   /*======================================================================*/
void
Simulator::Run()
{
	this->enumSimState = SIMULATOR_RUNNING;
}


   /*======================================================================*/
void
Simulator::Stop()
{
	this->enumSimState = SIMULATOR_STOPED;
}


   /*======================================================================*/
void
Simulator::Return()
{
	this->enumSimState = SIMULATOR_RETURN;
}


   /*======================================================================*/
const bool
Simulator::CheckRange(
	const uint & w,
	const uint & l,
	const uint & range,
	const OPENCITY_STRUCTURE_CODE & enumStructCode ) const
{
	static uint W1, L1, W2, L2;

   // calculate the possible W1,L1 and W2, L2
	W1 = w; L1 = l;
	W2 = w; L2 = l;
	pmapOfCity->GetPossibleWH( W1, L1, -range, -range );
	pmapOfCity->GetPossibleWH( W2, L2,  range,  range );

   // check if the requested structure is within the range
	return this->pbuildlayer->ContainStructure(
		W1, L1, W2, L2, enumStructCode );
}


   /*======================================================================*/
const bool
Simulator::CheckLevelUp(
	const uint & w,
	const uint & l,
	const Structure* pStruct ) const
{
	OPENCITY_DEBUG("CheckLevelUp");

	static OPENCITY_GRAPHIC_CODE nextGC;
	static uint ow, ol, oh, nw, nl, nh;		// Old, and New WLH

	assert( pStruct != NULL );			// Ya, we need it !
	nextGC = pStruct->GetNextLevelGraphicCode();

// IF it's the same THEN ok
	if (nextGC == pStruct->GetGraphicCode()) {
		return true;
	}

// Get the old WLH
	gpPropertyMgr->GetWLH(
		pStruct->GetGraphicCode(),
		ow, 0,
		ol, 0,
		oh, 0 );
	assert( ow != 0 );

// Get the new WLH
	gpPropertyMgr->GetWLH(
		nextGC,
		nw, 0,
		nl, 0,
		nh, 0 );
	assert( nw != 0 );

// IF they are the same THEN ok
	if ((ow == nw) && (ol == nl) && (oh == nh)) {
		return true;
	}

// IF the old ones are bigger THEN shout out
	if ((ow > nw) || (ol > nl) || (oh > nh)) {
		OPENCITY_DEBUG("The higher level should have bigger dimensions");
	}

// The old WLH are smaller
// IF the new required surface does not contains only the required structure THEN
	if (this->pbuildlayer->ContainStructureOnly(
		w, l,
//		w + nw - ow, l + nl - ol,
		w + nw - 1, l + nl - 1,
		pStruct->GetCode()) == false ) {
		return false;
	}

// OK, next level has passed all the tests
	return true;
}


   /*======================================================================*/
const bool
Simulator::CheckLevelDown(
	const uint & w,
	const uint & l,
	const Structure* pStruct ) const
{
	OPENCITY_DEBUG("CheckLevelDown");

	static OPENCITY_GRAPHIC_CODE prevGC;
	static uint ow, ol, oh, nw, nl, nh;		// Old, and New WLH

	assert( pStruct != NULL );			// Ya, we need it !
	prevGC = pStruct->GetPreviousLevelGraphicCode();

// IF it's the same THEN ok
	if (prevGC == pStruct->GetGraphicCode()) {
		return true;
	}

// Get the old WLH
	gpPropertyMgr->GetWLH(
		pStruct->GetGraphicCode(),
		ow, 0,
		ol, 0,
		oh, 0 );
	assert( ow != 0 );

// Get the new WLH
	gpPropertyMgr->GetWLH(
		prevGC,
		nw, 0,
		nl, 0,
		nh, 0 );
	assert( nw != 0 );

// IF they are the same THEN ok
	if ((ow == nw) && (ol == nl) && (oh == nh)) {
		return true;
	}

// IF the old ones are smaller THEN shout out
	if ((ow < nw) || (ol < nl) || (oh < nh)) {
		OPENCITY_DEBUG("The lower level should have smaller dimensions");
	}

// The old WLH are bigger
// IF the new required surface does not contains only the required structure THEN
	if (this->pbuildlayer->ContainStructureOnly(
		w, l,
		w + nw - 1, l + nl - 1,
		pStruct->GetCode()) == false ) {
		return false;
	}

// OK, next level has passed all the tests
	return true;
}


   /*======================================================================*/
const int &
Simulator::GetVariation() const
{
	return this->iVariation;
}


   /*======================================================================*/
const int &
Simulator::GetValue() const
{
	return this->iValue;
}


   /*======================================================================*/
void
Simulator::SetVariation(
	const int & rciVariation )
{
	this->iVariation = rciVariation;
}


   /*======================================================================*/
void
Simulator::SetValue(
	const int & rciValue )
{
	this->iValue = rciValue;
}


   /*=====================================================================*/
   /*                        STATIC      METHODS                          */
   /*=====================================================================*/
int
Simulator::ThreadWrapper(
	void* pSim )
{
	return ((Simulator*)pSim)->Main();
}


   /*======================================================================*/
void
Simulator::RCIDelay( void )
{
// Added +1 to avoid log10(0)
	SDL_Delay((uint)
		(gfMsSimDelayMax - log10((OC_FLOAT)Structure::GetNumber() + 1)
		*OC_MS_STRUCTURE_LOG_FACTOR )
		);
}


















