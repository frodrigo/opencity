/***************************************************************************
						CodingStyle.cpp    -  description
							-------------------
	begin                : nov 14th 2005
	copyright            : (C) 2005 by Duong-Khang NGUYEN
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

// Useful enumerations
#include "opencity_direction.h"
#include "opencity_structure_type.h"

// OpenCity headers
#include "CodingStyle.h"
#include "layer.h"
#include "texture.h"					// Terrain texturing

// Global settings
#include "globalvar.h"
extern GlobalVar gVars;

// Libraries headers
#include "xpath_processor.h"
#include "xpath_static.h"

// Standard headers
#include <deque>


   /*=====================================================================*/
CodingStyle::CodingStyle
(
	const bool & rcUseDL = true
)
{
	_boolABool = rcUseDL;
	OPENCITY_DEBUG( "ctor: some debug value:" << _boolABool );
}


   /*=====================================================================*/
CodingStyle::~CodingStyle()
{
	OPENCITY_DEBUG( "dtor" );
}


   /*=====================================================================*/
bool
CodingStyle::GetBool() const
{
	return _boolABool;
}


   /*=====================================================================*/
   /*                        PRIVATE     METHODS                          */
   /*=====================================================================*/
void
CodingStyle::_PrivateMethod()
{
	uint uiMyUInt = 0;			///< Variables are _always_ initialized
	int* piMyPInt = NULL;
}


   /*=====================================================================*/
void
CodingStyle::_PrivateMethod2( int x )
{
}


   /*=====================================================================*/
uint
CodingStyle::_PrivateMethod3
(
	uint a,
	float b
)
{
}
















