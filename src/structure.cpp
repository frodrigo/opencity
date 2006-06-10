/***************************************************************************
                          structure.cpp  -  description
       $Id: structure.cpp,v 1.25 2006/06/05 09:58:06 neoneurone Exp $
                             -------------------
    begin                : june 16th, 2003
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

#include "structure.h"

uint Structure::uiNumberStructure = 0;		///< Number of structures created so far


   /*======================================================================*/
const uint &
Structure::GetNumber()
{
	return Structure::uiNumberStructure;
}


   /*======================================================================*/
void
Structure::SetNumber( const uint & rcuiNb )
{
	Structure::uiNumberStructure = rcuiNb;
}


   /*======================================================================*/
Structure::Structure(  ):
enumGraphicCode( OC_EMPTY ),
enumStructureCode( OC_STRUCTURE_UNDEFINED ),
_enumType( OC_STRUCTURE_UNUSEDTYPE ),
_uiStatus( 0 ),
_uiLevel( OC_STRUCTURE_LEVEL_START ),
_pstructureMain( NULL )
{
	OPENCITY_DEBUG( "ctor, warning undefined structure code" );
	uiNumberStructure++;
}


   /*======================================================================*/
Structure::Structure
(
	const OPENCITY_STRUCTURE_CODE enumStructCode
):
enumStructureCode( enumStructCode ),
_uiStatus( 0 ),
_uiLevel( OC_STRUCTURE_LEVEL_START ),
_pstructureMain( NULL )
{
	OPENCITY_DEBUG( "ctor 1" );
	uiNumberStructure++;
}


   /*======================================================================*/
Structure::Structure
(
	const OPENCITY_STRUCTURE_CODE enumStructCode,
	Structure* pMain
):
enumStructureCode( enumStructCode ),
_uiStatus( 0 ),
_uiLevel( OC_STRUCTURE_LEVEL_START ),
_pstructureMain( pMain )
{
	OPENCITY_DEBUG( "ctor 2" );
	uiNumberStructure++;
}


   /*======================================================================*/
/* TOKILL
Structure::Structure( const Structure & rcSource )
{
	OPENCITY_DEBUG( "ctor copy" );

	this->enumGraphicCode = rcSource.enumGraphicCode;
	this->enumStructureCode = rcSource.enumStructureCode;
	_enumType = rcSource._enumType;
	_pstructureMain = rcSource._pstructureMain;
	_uiStatus = rcSource._uiStatus;

	uiNumberStructure++;
	assert( 0 );
}
*/


   /*======================================================================*/
/* TOKILL
Structure
Structure::operator=( const Structure & rcSource )
{
	OPENCITY_DEBUG( "operator =" );

   // check for something like x = x;
	if (this == &rcSource) {
		return (*this);
	}
	this->enumGraphicCode = rcSource.enumGraphicCode;
	this->enumStructureCode = rcSource.enumStructureCode;
	_enumType = rcSource._enumType;
	_pstructureMain = rcSource._pstructureMain;
	_uiStatus = rcSource._uiStatus;

	return (*this);
}
*/


   /*======================================================================*/
 Structure::~Structure(  )
{
	OPENCITY_DEBUG( "Structure dtor" );

	uiNumberStructure--;
}


   /*=====================================================================*/
void
Structure::SaveTo( std::fstream& rfs )
{
	OPENCITY_DEBUG( __PRETTY_FUNCTION__ << "saving" );

	rfs << enumGraphicCode << std::endl;
	rfs << enumStructureCode << std::endl;
	rfs << _enumType << std::endl;
	rfs << _uiStatus << std::endl;
	rfs << _uiLevel << std::endl;
}


   /*=====================================================================*/
void
Structure::LoadFrom( std::fstream& rfs )
{
	OPENCITY_DEBUG( __PRETTY_FUNCTION__ << "loading" );
	uint t;

	rfs >> t; rfs.ignore(); enumGraphicCode = (OPENCITY_GRAPHIC_CODE)t;
	rfs >> t; rfs.ignore(); enumStructureCode = (OPENCITY_STRUCTURE_CODE)t;
	rfs >> t; rfs.ignore(); _enumType= (OPENCITY_STRUCTURE_TYPE)t;
	rfs >> _uiStatus; rfs.ignore();
	rfs >> _uiLevel; rfs.ignore();
}


   /*======================================================================*/
void
Structure::Set(
	const uint wegBit )
{
	_uiStatus |= wegBit;
}


   /*======================================================================*/
void
Structure::Unset(
	const uint wegBit )
{
	_uiStatus &= ~wegBit;
}


   /*======================================================================*/
const bool
Structure::IsSet(
	const uint wegBit ) const
{
	if ((_uiStatus & wegBit) == wegBit)
		return true;
	else
		return false;
}


   /*======================================================================*/
const OPENCITY_GRAPHIC_CODE
Structure::GetGraphicCode() const
{
	return this->enumGraphicCode;
}


   /*======================================================================*/
const OPENCITY_GRAPHIC_CODE
Structure::GetBaseGraphicCode() const
{
	OPENCITY_GRAPHIC_CODE tempGC;

	switch (this->enumStructureCode) {
		case OC_STRUCTURE_RES:
			tempGC = OC_RES_ZONE0;
			break;

		case OC_STRUCTURE_COM:
			tempGC = OC_COM_ZONE0;
			break;

		case OC_STRUCTURE_IND:
			tempGC = OC_IND_ZONE0;
			break;

		default:
			OPENCITY_DEBUG("Not implemented yet");
			assert(0);
			break;
	}

	return tempGC;
}


   /*======================================================================*/
const OPENCITY_GRAPHIC_CODE
Structure::GetNextLevelGraphicCode() const
{
//	OPENCITY_DEBUG("WARNING: Inside Structure");
// WARNING: we don't have such information in a general structure
	return this->enumGraphicCode;
}


   /*======================================================================*/
const OPENCITY_GRAPHIC_CODE
Structure::GetPreviousLevelGraphicCode() const
{
// WARNING: we don't have such information in a general structure
	return this->enumGraphicCode;
}


   /*======================================================================*/
const OPENCITY_STRUCTURE_CODE
Structure::GetCode() const
{
	return this->enumStructureCode;
}


   /*======================================================================*/
Structure*
Structure::GetMain() const
{
	return _pstructureMain;
}


   /*======================================================================*/
void
Structure::SetMain( Structure* const pstruct )
{
	_pstructureMain = pstruct;
}


   /*======================================================================*/
const bool
Structure::LevelUp(  )
{
//	OPENCITY_DEBUG("WARNING: Inside Structure");
	if (_uiLevel < OC_STRUCTURE_LEVEL_MAX) {
		_uiLevel++;
		return true;
	}

	return false;
}


   /*======================================================================*/
const bool
Structure::LevelDown(  )
{
	if (_uiLevel > OC_STRUCTURE_LEVEL_MIN) {
		_uiLevel--;
		return true;
	}

	return false;
}


   /*======================================================================*/
const uint
Structure::GetLevel() const
{
	return _uiLevel;
}


   /*======================================================================*/
void
Structure::SetLevel(
	const uint uiLevel )
{
	_uiLevel = uiLevel;
}


   /*======================================================================*/
const OPENCITY_STRUCTURE_TYPE
Structure::GetType() const
{
	return _enumType;
}


   /*======================================================================*/
/* TOKILL
void
Structure::SetType(
	const uint & rcuiType )
{
	_enumType = rcuiType;
}
*/
























