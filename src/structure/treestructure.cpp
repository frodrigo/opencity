/***************************************************************************
						treetructure.cpp  -  description
							-------------------
	begin                : july 23th, 2006
	copyright            : (C) 2006 by Duong-Khang NGUYEN
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

#include "treestructure.h"

#include "globalvar.h"
extern GlobalVar gVars;


   /*=====================================================================*/
TreeStructure::TreeStructure():
Structure()
{
	OPENCITY_DEBUG( "ctor" );
}


   /*=====================================================================*/
TreeStructure::TreeStructure(
	const OPENCITY_STRUCTURE_CODE & enumStructCode):
Structure( enumStructCode )
{
	OPENCITY_DEBUG( "ctor param" );

	_eGC = OPENCITY_GRAPHIC_CODE(OC_TREE_BEGIN + rand() % (OC_TREE_END - OC_TREE_BEGIN + 1));
//	_eGC = gVars.gpPropertyMgr->GetGC( enumStructCode );
	_eType = gVars.gpPropertyMgr->GetST( enumStructCode );
}


   /*=====================================================================*/
/* not used atm
TreeStructure::TreeStructure(
	const OPENCITY_STRUCTURE_CODE & enumStructCode,
	Structure * pMain ):
Structure( enumStructCode, pMain )
{
	assert( pMain != NULL );

	_eGC = gVars.gpPropertyMgr->GetGC( enumStructCode );
	_eType = gVars.gpPropertyMgr->GetST( enumStructCode );
}
*/


   /*=====================================================================*/
TreeStructure::~TreeStructure(){
   OPENCITY_DEBUG( "dtor" );
}


   /*=====================================================================*/
/* not used atm
const OPENCITY_GRAPHIC_CODE
TreeStructure::GetNextLevelGraphicCode() const
{
//	OPENCITY_DEBUG("WARNING: Inside Tree");
	if (_uiLevel < OC_STRUCTURE_LEVEL_MAX)
		return (OPENCITY_GRAPHIC_CODE)(this->GetBaseGraphicCode() + ((_uiLevel+1) / 10));
	else
		return _eGC;
}
*/


   /*=====================================================================*/
/* not used atm
const OPENCITY_GRAPHIC_CODE
TreeStructure::GetPreviousLevelGraphicCode() const
{
	if (_uiLevel > OC_STRUCTURE_LEVEL_MIN)
		return (OPENCITY_GRAPHIC_CODE)(this->GetBaseGraphicCode() + ((_uiLevel-1) / 10));
	else
		return _eGC;
}
*/


   /*=====================================================================*/
/* not used atm
const bool
TreeStructure::LevelUp()
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
*/


   /*=====================================================================*/
/* not used atm
const bool
TreeStructure::LevelDown()
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
*/


   /*=====================================================================*/





























