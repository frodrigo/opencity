/***************************************************************************
						property.h  -  description
							-------------------
	begin                : january 28th, 2007
	copyright            : (C) 2007 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_PROPERTY_H_
#define _OPENCITY_PROPERTY_H_ 1

//#include "main.h"			// OPENCITY_DEBUG macro


// Object's properties XPath expressions
#define OC_METADATA_PROPERTY_NODE				"/object/property"
#define OC_METADATA_COST_NODE					"/object/property/cost"

#define OC_METADATA_NEED_R_NODE					"/object/property/r/need"
#define OC_METADATA_PROVIDE_R_NODE				"/object/property/r/provide"
#define OC_METADATA_NEED_C_NODE					"/object/property/c/need"
#define OC_METADATA_PROVIDE_C_NODE				"/object/property/c/provide"
#define OC_METADATA_NEED_I_NODE					"/object/property/i/need"
#define OC_METADATA_PROVIDE_I_NODE				"/object/property/i/provide"
#define OC_METADATA_NEED_W_NODE					"/object/property/w/need"
#define OC_METADATA_PROVIDE_W_NODE				"/object/property/w/provide"
#define OC_METADATA_NEED_E_NODE					"/object/property/e/need"
#define OC_METADATA_PROVIDE_E_NODE				"/object/property/e/provide"
#define OC_METADATA_NEED_G_NODE					"/object/property/g/need"
#define OC_METADATA_PROVIDE_G_NODE				"/object/property/g/provide"

#define OC_METADATA_NEED_T_NODE					"/object/property/t/need"
#define OC_METADATA_PROVIDE_T_NODE				"/object/property/t/provide"
#define OC_METADATA_NEED_NATURE_NODE			"/object/property/nature/need"
#define OC_METADATA_PROVIDE_NATURE_NODE			"/object/property/nature/provide"

#define OC_METADATA_STRUCTURE_TYPE_ATTRIBUTE	"/object/property/@type"
#define OC_METADATA_INHABITANT_ATTRIBUTE		"/object/property/@inhabitant"
#define OC_METADATA_RADIUS_ATTRIBUTE			"/object/property/@radius"
#define OC_METADATA_WORKER_ATTRIBUTE			"/object/property/@worker"
#define OC_METADATA_DIRECTION_ATTRIBUTE			"/object/property/direction/@value"

#define OC_METADATA_MODEL_NODE					"/object/model"


enum OPENCITY_DIRECTION;
enum OPENCITY_STRUCTURE_TYPE;


//========================================================================
/** A simple min/max type */
class MinMax
{
public:
	int iMin, iMax;

	MinMax( int min, int max):
	iMin(min), iMax(max)
	{}
};


//========================================================================
/** Each "need", "provide" property is limited by its min and max values.
*/
class NeedProvide
{
public:
	MinMax mmNeed, mmProvide;

	NeedProvide():
	mmNeed(0, INT_MAX), mmProvide(0, 0)
	{}
};


//========================================================================
/** The properties of each building (structure) in OpenCity are
encapsulated in this structure.
*/
class Property
{
public:
/** The financial aspects of the object
*/
	uint uiBuildCost, uiDestroyCost, uiSupportCost, uiIncome;

/** The need/provide properties
*/
	NeedProvide sResidence, sCommerce, sIndustry;
	NeedProvide sWater, sElectricity, sGas;
	NeedProvide sTraffic, sNature;

/** The dimensions of the model
*/
	uint	uiWidth, uiLength;
	float	fHeight;

/** The generated number of inhabitant, worker and the radius of the influence
upon the other objects
*/
	uint	uiInhabitant, uiWorker, uiRadius;

/** The OpenCity type of the object. This type is used for un/serialization
*/
	OPENCITY_STRUCTURE_TYPE	eStructureType;

/** The directions to which the object is connected to.
*/
	OPENCITY_DIRECTION		eDirection;


//========================================================================
/** Default ctor. Initialize all the member variables with their default
values.
*/
	Property():
	uiBuildCost(0), uiDestroyCost(0), uiSupportCost(0), uiIncome(0),
	uiWidth(0), uiLength(0),
	fHeight(0),
	uiInhabitant(0), uiWorker(0), uiRadius(0),
	eStructureType(OC_TYPE_UNDEFINED),
	eDirection(OC_DIR_UNDEFINED)
	{
		//OPENCITY_DEBUG( "ctor" );
	}

	~Property()
	{
		//OPENCITY_DEBUG( "dtor" );
	}
};

#endif
































