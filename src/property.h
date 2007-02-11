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


// Object's properties XPath expressions
#define OC_METADATA_PROPERTY_NODE		"/object/property"
#define OC_METADATA_COST_NODE			"/object/property/cost"
#define OC_METADATA_DIRECTION_ATTRIBUTE	"/object/property/direction/@value"

#define OC_METADATA_MODEL_NODE			"/object/model"


enum OPENCITY_DIRECTION;


//========================================================================
/** The properties of each building (structure) in OpenCity are
encapsulated in this structure.
*/
struct Property
{
/** The financial aspects of the object
"//object/property/cost"
*/
	uint uiBuildCost, uiDestroyCost, uiSupportCost, uiIncome;

/** The dimensions of the model
"//object/model"
*/
	uint	uiWidth, uiLength;
	float	fHeight;

/** The directions to which the object is connected to.
"//object/property/direction[@value]"
*/
	OPENCITY_DIRECTION	eDirection;
};

#endif
































