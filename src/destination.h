/***************************************************************************
                          destination.h  -  description
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

#ifndef _OPENCITY_DESTINATION_H
#define _OPENCITY_DESTINATION_H 1

#include "main.h"

   //========================================================================
   /** It's just a simple class which contains the informations
       about the OC W, H coordinates, in which direction it is
       and how many OC kms it is far away.
   */
class Destination {
public:
	uint uiW;						///< the OC W, H coordinates
	uint uiH;
	OPENCITY_DIRECTION enumDirection;	///< which direction ?
	uint uiTime;						///< how many time do I have to move in this direction ?
	OC_UBYTE ubTraffic;					///< local traffic at this WH
};

#endif

































