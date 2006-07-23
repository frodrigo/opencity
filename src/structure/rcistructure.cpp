/***************************************************************************
                          rcistructure.cpp  -  description
     $Id: rcistructure.cpp 4 2006-06-11 15:26:38Z neoneurone $
                             -------------------
    begin                : mar jun 17 2003
    copyright            : (C) 2003-2006 by Duong-Khang NGUYEN
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

#include "rcistructure.h"
#include "propertymanager.h"


extern PropertyManager* gpPropertyMgr;	// global property manager


   /*=====================================================================*/
RCIStructure::RCIStructure():
Structure()
{
	OPENCITY_DEBUG( "ctor" );
}


   /*=====================================================================*/
RCIStructure::RCIStructure(
	const OPENCITY_STRUCTURE_CODE & enumStructCode):
Structure( enumStructCode )
{
	OPENCITY_DEBUG( "ctor param" );

	_eGC = gpPropertyMgr->GetGC( enumStructCode );
	_eType = gpPropertyMgr->GetST( enumStructCode );
}


   /*=====================================================================*/
RCIStructure::RCIStructure(
	const OPENCITY_STRUCTURE_CODE & enumStructCode,
	Structure * pMain ):
Structure( enumStructCode, pMain )
{
	assert( pMain != NULL );

	_eGC = gpPropertyMgr->GetGC( enumStructCode );
	_eType = gpPropertyMgr->GetST( enumStructCode );
}


   /*=====================================================================*/
RCIStructure::~RCIStructure(){
   OPENCITY_DEBUG( "dtor" );
}


   /*=====================================================================*/
const OPENCITY_GRAPHIC_CODE
RCIStructure::GetNextLevelGraphicCode() const
{
//	OPENCITY_DEBUG("WARNING: Inside RCI");
	if (_uiLevel < OC_STRUCTURE_LEVEL_MAX)
		return (OPENCITY_GRAPHIC_CODE)(this->GetBaseGraphicCode() + ((_uiLevel+1) / 10));
	else
		return _eGC;
}


   /*=====================================================================*/
const OPENCITY_GRAPHIC_CODE
RCIStructure::GetPreviousLevelGraphicCode() const
{
	if (_uiLevel > OC_STRUCTURE_LEVEL_MIN)
		return (OPENCITY_GRAPHIC_CODE)(this->GetBaseGraphicCode() + ((_uiLevel-1) / 10));
	else
		return _eGC;
}


   /*=====================================================================*/
const bool
RCIStructure::LevelUp()
{
	OPENCITY_GRAPHIC_CODE newGC;
	
	newGC = this->GetNextLevelGraphicCode();

	if (this->Structure::LevelUp() == true) {
		_eGC = newGC;
		return true;
	}
	else {
		return false;
	}
}


   /*=====================================================================*/
const bool
RCIStructure::LevelDown()
{
	OPENCITY_GRAPHIC_CODE newGC;
	
	newGC = this->GetPreviousLevelGraphicCode();

	if (this->Structure::LevelDown() == true) {
		_eGC = newGC;
		return true;
	}
	else {
		return false;
	}
}


   /*=====================================================================*/





























