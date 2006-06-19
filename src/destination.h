/***************************************************************************
							destination.h  -  description
								-------------------
	begin                : dim mai 16 2004
	copyright            : (C) 2004-2006 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_DESTINATION_H
#define _OPENCITY_DESTINATION_H 1

#include "main.h"


//========================================================================
/** It's just a simple class which contains the informations about the
	OC W,L coordinates, in which direction it is and how many OC kms
	it is far away.
*/
class Destination {
public:
	OPENCITY_DIRECTION _eDir	;	///< Which direction ?
	uint _uiW;						///< The OC W, L coordinates
	uint _uiL;
	int _iHMin, _iHMax;				///< The height of the current square (on the Oy axis)
	uint _uiTime;					///< How many time do I have to move in this direction ?
	OC_UBYTE _ubTraffic;			///< Local traffic at this WL


	Destination();
	~Destination();


   /*=====================================================================*/
   /*                          STATIC    METHODS                          */
   /*=====================================================================*/

//========================================================================
/** Get the direction from A to B. The coordinates designated by the
two destinations must be side by side on the map
	\param rcA First destination
	\param rcB Second destination
	\return The direction from A to B
*/
	static OPENCITY_DIRECTION
	GetDir(
		const Destination & rcA,
		const Destination & rcB);
};

#endif

































