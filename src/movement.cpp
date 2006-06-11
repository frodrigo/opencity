/***************************************************************************
                          movement.cpp  -  description
      $Id$
                             -------------------
    begin                : dim mai 16 2004
    copyright            : (C) 2004 by Duong-Khang NGUYEN
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


#include "movement.h"


   /*=====================================================================*/
Movement::Movement()
{
	OPENCITY_DEBUG("ctor");
}


   /*=====================================================================*/
Movement::~Movement()
{
	OPENCITY_DEBUG("dtor");
}


   /*=====================================================================*/
void
Movement::SetPath(
	vector<Destination> newPath )
{
   // WARNING: Destination must not use pointers
	this->vdest = newPath;
}


   /*=====================================================================*/
const OPENCITY_GRAPHIC_CODE &
Movement::GetGraphicCode() const
{
	return this->enumGraphicCode;
}


   /*=====================================================================*/
   //                       STATIC     METHODS
   /*=====================================================================*/
void
Movement::Move2Dir(
	Destination & rD )
{
	OPENCITY_DEBUG("called");

   // move to the indicated direction
	switch (rD.enumDirection) {
		case OC_DIR_N:  rD.uiH--;      break;
		case OC_DIR_NE: rD.uiH--; rD.uiW++; break;
		case OC_DIR_E:  rD.uiW++;      break;
		case OC_DIR_SE: rD.uiW++; rD.uiH++; break;
		case OC_DIR_S:  rD.uiH++;      break;
		case OC_DIR_SW: rD.uiH++; rD.uiW--; break;
		case OC_DIR_W:  rD.uiW--;      break;
		case OC_DIR_NW: rD.uiW--; rD.uiH--; break;
		default: break;
	}
}




































