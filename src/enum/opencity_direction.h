/***************************************************************************
						opencity_direction.h  -  description
							-------------------
	begin                : february 21th, 2007
	copyright            : (C) 2007 by Duong Khang NGUYEN
	email                : neoneurone @ gmail com
	
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

#ifndef _OPENCITY_DIRECTION_ENUM_H_
#define _OPENCITY_DIRECTION_ENUM_H_ 1


/** The directions used in map calculation, vehicle movement, path placement etc...
*/
enum OPENCITY_DIRECTION {
	OC_DIR_O_N = 0,					///< North
	OC_DIR_O_E,						///< East
	OC_DIR_O_S,						///< South
	OC_DIR_O_W,						///< West
	OC_DIR_S_N,
	OC_DIR_W_E,     // +5
	OC_DIR_N_E,
	OC_DIR_N_W,
	OC_DIR_S_E,
	OC_DIR_S_W,
	OC_DIR_S_N_E,   // +10
	OC_DIR_S_W_E,
	OC_DIR_S_N_W,
	OC_DIR_N_W_E,
	OC_DIR_S_N_W_E,
	OC_DIR_UNDEFINED				///< What the heck is this ?
};


#endif
