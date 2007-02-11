/***************************************************************************
                          movement.h  -  description
         $Id$
                             -------------------
    begin                : dim mai 16 2004
    copyright            : (C) 2004-2006 by Duong-Khang NGUYEN
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
/** This is a base class for everything that can move in OpenCity. It
	contains general informations about the current OC W, H coordinates
	of the moving (or not) object and a path which is a vector of
	"Destination" objects
*/
class Movement {
public:
	Movement();
	virtual ~Movement();


//========================================================================
/** Tell the object to move
	\return True if the object has moved successfully, false otherwise
*/
	virtual const bool
	Move() = 0;


//========================================================================
/** Initialize the movement. It should be called before any call to
the Move() method
	\sa Move()
*/
	virtual void
	Start() = 0;

	void
	SetPath( vector<Destination> newPath );

	const OPENCITY_GRAPHIC_CODE &
	GetGraphicCode() const;


//========================================================================
/** Set the rotation of the model according to the destination's direction.
This method also sets the compensation translation vector in order to help
the GraphicManager render method to make a "in place" rotation
	\param rcD The destination object
	\see GraphicManager::Display()
*/
	void
	SetAngle( const Destination & rcD );


//========================================================================
/** Set the slope of the model. It shoulds be called after a call to
SetAngle()
	\see GraphicManager::Display()
	\see SetAngle()
*/
	void
	SetSlope(
		const Destination & rcA,
		const Destination & rcB );


public:
	OC_FLOAT _fCurrentW;			///< current OC W, L, H coordinates
	OC_FLOAT _fCurrentL;
	OC_FLOAT _fCurrentH;
	OC_FLOAT _fDeltaW;				///< W, L, H variations for next destination
	OC_FLOAT _fDeltaL;
	OC_FLOAT _fDeltaH;

	GLfloat _fRY;					///< The model's rotation angle
	GLfloat _fRX, _fRZ;				///< The model's X and Z rotation slope
	GLfloat _fTX, _fTY, _fTZ;		///< The rotation compensation translation vector

protected:
	OPENCITY_DIRECTION		_eDir;	///< Last vehicle direction. According to the graphic code convention, it's OC_DIR_S by default
	OPENCITY_GRAPHIC_CODE	_eGC;	///< Vehicle's graphic code

	uint uiCurrentTime;				///< current number of times that we've moved so far into the current direction
	int iCurrentSpeed;
	uint uiNumberOfFrame;			///< Number of frames counted till now
	uint uiFramePerUnit;			///< Number of frames to count before going to next W/H coordinates
	vector<Destination> vdest;		///< this contains a path that this movement follows
	uint uiCurrentIndex;			///< point to the current destination
	Destination destCurrent;		///< current copy of vdest[ uiCurrentIndex ]
};

#endif
































