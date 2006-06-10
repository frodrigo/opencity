/***************************************************************************
                          wegstructure.cpp  -  description
     $Id: wegstructure.cpp,v 1.16 2006/06/05 09:58:06 neoneurone Exp $
                             -------------------
    begin                : june 17th, 2003
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

#include "wegstructure.h"
#include "propertymanager.h"


extern PropertyManager* gpPropertyMgr;	// global property manager


   /*=====================================================================*/
WEGStructure::WEGStructure()
{
	OPENCITY_DEBUG( "ctor" );
}


   /*=====================================================================*/
WEGStructure::WEGStructure(
	const OPENCITY_STRUCTURE_CODE & enumStructCode ):
Structure( enumStructCode )
{
	this->enumGraphicCode = gpPropertyMgr->GetGC( enumStructCode );
	_enumType = gpPropertyMgr->GetST( enumStructCode );

// IF this is an electric plant THEN turn on the electricity bit
	if (enumStructCode == OC_STRUCTURE_EPLANT_COAL) {
		Set( OC_STRUCTURE_E );
	}
}


   /*=====================================================================*/
WEGStructure::WEGStructure(
	const OPENCITY_STRUCTURE_CODE & enumStructCode,
	Structure * pMain ):
Structure( enumStructCode, pMain )
{
	assert( pMain != NULL );

	switch (enumStructCode) {
		case OC_STRUCTURE_PART:
			this->enumGraphicCode = OC_EMPTY;
		   // If this is part of an electric plant
		   // Then turn the E bit on
			if ( (pMain!= NULL) && (pMain->GetCode() == OC_STRUCTURE_EPLANT_COAL) )
				this->Set( OC_STRUCTURE_E );
			break;

/* TOKILL
// NOTE: do we need this ? oct 14th, 05
		case OC_STRUCTURE_EPLANT_COAL:
			this->enumGraphicCode = OC_EPLANT_COAL_BOX;
		   // turn on the electricity bit
			Set( OC_STRUCTURE_E );
			break;
*/

		default:
			OPENCITY_DEBUG( "Unknown structure" );
			assert(0);
			break;
	}

	_enumType = gpPropertyMgr->GetST( enumStructCode );
}


   /*=====================================================================*/
WEGStructure::~WEGStructure(){
   OPENCITY_DEBUG( "dtor" );
}































