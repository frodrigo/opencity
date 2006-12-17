/***************************************************************************
						simulator.cpp  -  description
							-------------------
	begin                : dim sep 21 2003
	copyright            : (C) 2003-2006 by Duong-Khang NGUYEN
	email                : neoneurone @ users sourceforge net
	
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

#include "simulator.h"
#include "buildinglayer.h"
#include "structure.h"

#include "globalvar.h"
extern GlobalVar gVars;

#include <cmath>				// For log10


// This shared table is initialized by MainSim
// WARNING: this may not be thread safe !
volatile int Simulator::_tiVariation[Simulator::OC_SIMULATOR_NUMBER];


   /*=====================================================================*/
Simulator::Simulator():
_iVariation( 0 ),
_iValue( 0 ),
enumSimState( SIMULATOR_STOPED ),
mutexMain( NULL )
{
	OPENCITY_DEBUG( "Sim ctor , do not use" );
}


   /*=====================================================================*/
Simulator::Simulator( SDL_mutex* mutex, BuildingLayer* pblayer, Map* pmap ):
_iVariation( 0 ),
_iValue( 0 ),
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
Simulator::SaveTo( std::fstream& rfs )
{
	OPENCITY_DEBUG( __PRETTY_FUNCTION__ << "saving" );

	rfs << _iVariation << std::ends;
	rfs << _iValue << std::ends;
}


   /*======================================================================*/
void
Simulator::LoadFrom( std::fstream& rfs )
{
	OPENCITY_DEBUG( __PRETTY_FUNCTION__ << "loading" );

	rfs >> _iVariation; rfs.ignore();
	rfs >> _iValue; rfs.ignore();
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
	const uint w,
	const uint l,
	const Structure* pStruct ) const
{
	static OPENCITY_GRAPHIC_CODE nextGC;
	static uint ow = 0, ol = 0, oh = 0, nw = 0, nl = 0, nh = 0;		// Old, and New WLH
	static uint w2 = 0, l2 = 0;
	static uint layerW = 0, layerL = 0;

	assert( pStruct != NULL );			// Ya, we need it !
	nextGC = pStruct->GetNextLevelGraphicCode();

// IF it's the same THEN ok
	if (nextGC == pStruct->GetGraphicCode()) {
		return true;
	}

// Get the old WLH
	gVars.gpPropertyMgr->GetWLH(
		pStruct->GetGraphicCode(),
		ow, 0,
		ol, 0,
		oh, 0 );
	assert( ow != 0 );

// Get the new WLH
	gVars.gpPropertyMgr->GetWLH(
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
		OPENCITY_DEBUG("The higher level should have bigger dimensions - "
			<< "oW/oL/oH " << ow << "/" << ol << "/" << oh
			<< " - nW/nL/nH " << nw << "/" << nl << "/" << nh );
	}

// IF the new structure is out of the map THEN
	w2 = w + nw - 1;
	l2 = l + nl - 1;
	pbuildlayer->GetLayerSize( layerW, layerL );
	if ( w2 >= layerW || l2 >= layerL ) {
//		OPENCITY_DEBUG( "Sorry, it's out of the map !" );
		return false;
	}

// IF the new surface is not plane THEN return false?
	if (!gVars.gpMapMgr->IsSurfacePlane( w, l, w2, l2 )) {
		return false;
	}

// The old WLH are smaller
// IF the new required surface does not contains only the required structure THEN
	if (this->pbuildlayer->ContainStructureOnly(
		w, l, w2, l2, pStruct->GetCode()) == false ) {
//		OPENCITY_DEBUG( "ContainStructureOnly failed - W/L/W2/L2 "
//			<< w << "/" << l << "/" << w2 << "/" << l2 );
		return false;
	}

// OK, next level has passed all the tests
	OPENCITY_DEBUG( "CheckLevelUp OK - W/L/Level - " << w << "/" << l << "/" << pStruct->GetLevel() );
	return true;
}


   /*======================================================================*/
const bool
Simulator::CheckLevelDown(
	const uint w,
	const uint l,
	const Structure* pStruct ) const
{
	static OPENCITY_GRAPHIC_CODE prevGC;
	static uint ow = 0, ol = 0, oh = 0, nw = 0, nl = 0, nh = 0;		// Old, and New WLH
	static uint w2 = 0, l2 = 0;
	static uint layerW = 0, layerL = 0;

	assert( pStruct != NULL );			// Ya, we need it !
	prevGC = pStruct->GetPreviousLevelGraphicCode();

// IF it's the same THEN ok
	if (prevGC == pStruct->GetGraphicCode()) {
		return true;
	}

// Get the old WLH
	gVars.gpPropertyMgr->GetWLH(
		pStruct->GetGraphicCode(),
		ow, 0,
		ol, 0,
		oh, 0 );
	assert( ow != 0 );

// Get the new WLH
	gVars.gpPropertyMgr->GetWLH(
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

// IF the new structure is out of the map THEN
	w2 = w + nw - 1;
	l2 = l + nl - 1;
	pbuildlayer->GetLayerSize( layerW, layerL );
	if ( w2 >= layerW || l2 >= layerL ) {
		return false;
	}

// IF the new surface is not plane THEN return false?
	if (!gVars.gpMapMgr->IsSurfacePlane( w, l, w2, l2 )) {
		return false;
	}

// The old WLH are bigger
// IF the new required surface does not contains only the required structure THEN
	if (this->pbuildlayer->ContainStructureOnly(
		w, l, w2, l2,
		pStruct->GetCode()) == false ) {
		return false;
	}

// OK, next level has passed all the tests
	OPENCITY_DEBUG("CheckLevelDown OK");
	return true;
}


   /*======================================================================*/
const int &
Simulator::GetVariation() const
{
	return this->_iVariation;
}


   /*======================================================================*/
const int
Simulator::GetValue() const
{
	return _iValue;
}


   /*======================================================================*/
void
Simulator::SetVariation(
	const int rciVariation )
{
	_iVariation = rciVariation;
}


   /*======================================================================*/
void
Simulator::SetValue(
	const int rciValue )
{
	_iValue = rciValue;
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
		(gVars.gfMsSimDelayMax - log10((OC_FLOAT)Structure::GetNumber() + 1)
		*OC_MS_STRUCTURE_LOG_FACTOR )
		);
}


















