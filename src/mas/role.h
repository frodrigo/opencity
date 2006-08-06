/***************************************************************************
						role.h  -  description
							-------------------
	begin                : nov 29th 2005
	copyright            : (C) 2005-2006 by Duong-Khang NGUYEN
	email                : neoneurone @ users sourceforge net
	author               : Victor STINNER
	
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


#ifndef _OPENCITY_ROLE_H_
#define _OPENCITY_ROLE_H_ 1


   /*=====================================================================*/
typedef enum {
	ROLE_POLICE=0,
	ROLE_DEMONSTRATOR,
	ROLE_ROBBER
} Role_t;


   /*=====================================================================*/
typedef enum
{
	NORTH=0,
	WEST,
	SOUTH,
	EAST
} direction_t;
#endif

































