/***************************************************************************
						property.h  -  description
							-------------------
	begin                : january 28th, 2007
	copyright            : (C) 2007 by Duong-Khang NGUYEN
	email                : neoneurone @ users sourceforge net
	
	$Id: propertymanager.h 63 2006-10-17 20:45:12Z neoneurone $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#ifndef _OPENCITY_PROPERTY_H_
#define _OPENCITY_PROPERTY_H_ 1


//========================================================================
/** The properties of each building (structure) in OpenCity are
encapsulated in this structure.
*/
struct Property
{
	uint	uiWidth, uiLength;			// Width and length of the building
	float	fHeight;
};

#endif
































