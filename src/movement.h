/***************************************************************************
                          movement.h  -  description
         $Id: movement.h,v 1.9 2006/03/12 23:14:18 neoneurone Exp $
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

#ifndef _OPENCITY_MOVEMENT_H_
#define _OPENCITY_MOVEMENT_H_ 1

	#include "main.h"
	#include "destination.h"

	#include <vector>

using std::vector;

   //========================================================================
   /** This is a base class for everything that can move in OpenCity.
       It contains general informations about the current
       OC W, H coordinates of the moving (or not) object
       and a path which is a vector of "Destination" objects
   */
class Movement {
public:
	Movement();
	virtual ~Movement();

	virtual const bool
	Move() = 0;

	virtual void
	Start() = 0;

	void
	SetPath(
		vector<Destination> newPath );

	const OPENCITY_GRAPHIC_CODE &
	GetGraphicCode() const;

   /*=====================================================================*/
   //                       STATIC     METHODS
   /*=====================================================================*/
	static void
	Move2Dir(
		Destination & rD );


public:
	OC_FLOAT fCurrentW;				///< current OC W, H coordinates
	OC_FLOAT fCurrentH;
	OC_FLOAT fDeltaW;				///< W,H variations for next destination
	OC_FLOAT fDeltaH;

protected:
	OPENCITY_DIRECTION enumCurrentDirection;
   /// current number of times that we've moved so far into the current direction
	uint uiCurrentTime;
	int iCurrentSpeed;
	uint uiNumberOfFrame;		///< Number of frames counted till now
   /// Number of frames to count before going to next W/H coordinates
	uint uiFramePerUnit;
	vector<Destination> vdest;		///< this contains a path that this movement follows
	uint uiCurrentIndex;			///< point to the current destination
	Destination destCurrent;		///< current copy of vdest[ uiCurrentIndex ]
	OPENCITY_GRAPHIC_CODE enumGraphicCode;
};

#endif
































