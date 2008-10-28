/***************************************************************************
						property.cpp  -  description
							-------------------
	begin                : may 18th, 2008
	copyright            : (C) 2008 by Duong Khang NGUYEN
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

// OpenCity headers
#include "property.h"

// Libraries headers
#include "xpath_processor.h"
#include "xpath_static.h"



using namespace TinyXPath;


   /*=====================================================================*/
   /*                        PRIVATE     METHODS                          */
   /*=====================================================================*/
Property*
Property::LoadProperties( string filename )
{
// Parameters checking
	OPENCITY_DEBUG( "Loading file: " << filename );
	assert( filename != "" );

// Load the XML metadata file
	TiXmlDocument xml;
	if (!xml.LoadFile(filename) || xml.Error()) {
		OPENCITY_FATAL( xml.ErrorDesc() );
		abort();
	}

// Get the root element
	TiXmlNode* pRoot = xml.RootElement();
	if (pRoot == NULL) {
		OPENCITY_FATAL( xml.ErrorDesc() );
		abort();
	}

// Initialize few work variables
	TiXmlNode* pNode = NULL;
	TiXmlElement* pElement = NULL;
	TiXmlAttribute* pAttribute = NULL;
	Property* pProperty = new Property();

// Select the "/object/property/cost" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_COST_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "build",	(int*)&pProperty->uiBuildCost );
	pElement->QueryIntAttribute( "destroy",	(int*)&pProperty->uiDestroyCost );
	pElement->QueryIntAttribute( "support",	(int*)&pProperty->uiSupportCost );
	pElement->QueryIntAttribute( "income",	(int*)&pProperty->uiIncome );

// RCI ========================================================================
// Select the "/object/property/r/need" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_NEED_R_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&pProperty->sResidence.mmNeed.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&pProperty->sResidence.mmNeed.iMax);

// Select the "/object/property/r/provide" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROVIDE_R_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&pProperty->sResidence.mmProvide.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&pProperty->sResidence.mmProvide.iMax);

// Select the "/object/property/c/need" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_NEED_C_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&pProperty->sCommerce.mmNeed.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&pProperty->sCommerce.mmNeed.iMax);

// Select the "/object/property/c/provide" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROVIDE_C_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&pProperty->sCommerce.mmProvide.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&pProperty->sCommerce.mmProvide.iMax);

// Select the "/object/property/i/need" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_NEED_I_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&pProperty->sIndustry.mmNeed.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&pProperty->sIndustry.mmNeed.iMax);

// Select the "/object/property/i/provide" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROVIDE_I_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&pProperty->sIndustry.mmProvide.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&pProperty->sIndustry.mmProvide.iMax);

// WEG ========================================================================
// Select the "/object/property/w/need" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_NEED_W_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&pProperty->sWater.mmNeed.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&pProperty->sWater.mmNeed.iMax);

// Select the "/object/property/w/provide" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROVIDE_W_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&pProperty->sWater.mmProvide.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&pProperty->sWater.mmProvide.iMax);

// Select the "/object/property/e/need" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_NEED_E_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&pProperty->sElectricity.mmNeed.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&pProperty->sElectricity.mmNeed.iMax);

// Select the "/object/property/e/provide" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROVIDE_E_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&pProperty->sElectricity.mmProvide.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&pProperty->sElectricity.mmProvide.iMax);

// Select the "/object/property/g/need" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_NEED_G_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&pProperty->sGas.mmNeed.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&pProperty->sGas.mmNeed.iMax);

// Select the "/object/property/e/provide" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROVIDE_G_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&pProperty->sGas.mmProvide.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&pProperty->sGas.mmProvide.iMax);

// TN  ========================================================================
// Select the "/object/property/t/need" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_NEED_T_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&pProperty->sTraffic.mmNeed.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&pProperty->sTraffic.mmNeed.iMax);

// Select the "/object/property/t/provide" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROVIDE_T_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&pProperty->sTraffic.mmProvide.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&pProperty->sTraffic.mmProvide.iMax);

// Select the "/object/property/nature/need" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_NEED_NATURE_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&pProperty->sNature.mmNeed.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&pProperty->sNature.mmNeed.iMax);

// Select the "/object/property/nature/provide" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROVIDE_NATURE_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&pProperty->sNature.mmProvide.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&pProperty->sNature.mmProvide.iMax);

// ??? ========================================================================
// Select the "/object/property" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROPERTY_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "inhabitant",	(int*)&pProperty->uiInhabitant );
	pElement->QueryIntAttribute( "worker",		(int*)&pProperty->uiWorker );
	pElement->QueryIntAttribute( "radius",		(int*)&pProperty->uiRadius );

// Select the "/object/property/@type" attribute
	pAttribute = XAp_xpath_attribute(pRoot, OC_METADATA_STRUCTURE_TYPE_ATTRIBUTE);
	assert( pAttribute != NULL );
	pProperty->eStructureType = _Str2Type(pAttribute->ValueStr());

// Select the "/object/property/direction/@value" attribute
	pAttribute = XAp_xpath_attribute(pRoot, OC_METADATA_DIRECTION_ATTRIBUTE);
	assert( pAttribute != NULL );
	pProperty->eDirection = _Str2Direction(pAttribute->ValueStr());

// Select the "/object/model" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_MODEL_NODE);
	assert( pNode != NULL );
// Get the <model> node and fill the structure with the model dimension
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "width",	(int*)&pProperty->uiWidth );
	pElement->QueryIntAttribute( "length",	(int*)&pProperty->uiLength );
	pElement->QueryFloatAttribute( "height", &pProperty->fHeight );

// Debug
/*
	OPENCITY_DEBUG(
		endl <<
		"W/L/H: " <<
			pProperty->uiWidth << "/" << pProperty->uiLength << "/" << pProperty->fHeight << " | " <<
		"B/D/S/I: " <<
			pProperty->uiBuildCost << "/" << pProperty->uiDestroyCost << "/" << pProperty->uiSupportCost << "/" << pProperty->uiIncome << " | " <<
		"Dir: " <<
			pProperty->eDirection << " | " <<
		"Type: " <<
			pProperty->eStructureType << " | " <<
		"i/w/r: " <<
			pProperty->uiInhabitant << "/" << pProperty->uiWorker << "/" << pProperty->uiRadius << " | " <<
			endl <<
		"r/c/i: " <<
			pProperty->sResidence.mmNeed.iMin << "-" << pProperty->sResidence.mmNeed.iMax << " " <<
			pProperty->sResidence.mmProvide.iMin << "-" << pProperty->sResidence.mmProvide.iMax << " / " <<
			pProperty->sCommerce.mmNeed.iMin << "-" << pProperty->sCommerce.mmNeed.iMax << " " <<
			pProperty->sCommerce.mmProvide.iMin << "-" << pProperty->sCommerce.mmProvide.iMax << " / " <<
			pProperty->sIndustry.mmNeed.iMin << "-" << pProperty->sIndustry.mmNeed.iMax << " " <<
			pProperty->sIndustry.mmProvide.iMin << "-" << pProperty->sIndustry.mmProvide.iMax << " / " <<
			endl <<
		"w/e/g: " <<
			pProperty->sWater.mmNeed.iMin << "-" << pProperty->sWater.mmNeed.iMax << " " <<
			pProperty->sWater.mmProvide.iMin << "-" << pProperty->sWater.mmProvide.iMax << " / " <<
			pProperty->sElectricity.mmNeed.iMin << "-" << pProperty->sElectricity.mmNeed.iMax << " " <<
			pProperty->sElectricity.mmProvide.iMin << "-" << pProperty->sElectricity.mmProvide.iMax << " / " <<
			pProperty->sGas.mmNeed.iMin << "-" << pProperty->sGas.mmNeed.iMax << " " <<
			pProperty->sGas.mmProvide.iMin << "-" << pProperty->sGas.mmProvide.iMax << " / " <<
			endl <<
		"t/n: " <<
			pProperty->sTraffic.mmNeed.iMin << "-" << pProperty->sTraffic.mmNeed.iMax << " " <<
			pProperty->sTraffic.mmProvide.iMin << "-" << pProperty->sTraffic.mmProvide.iMax << " / " <<
			pProperty->sNature.mmNeed.iMin << "-" << pProperty->sNature.mmNeed.iMax << " " <<
			pProperty->sNature.mmProvide.iMin << "-" << pProperty->sNature.mmProvide.iMax
	);
*/

	return pProperty;
}


   /*=====================================================================*/
const OPENCITY_STRUCTURE_TYPE
Property::_Str2Type(const string& rcstrType)
{
	OPENCITY_STRUCTURE_TYPE type = OC_TYPE_UNDEFINED;

	if (rcstrType =="r") type = OC_TYPE_RESIDENCE; else
	if (rcstrType =="c") type = OC_TYPE_COMMERCE; else
	if (rcstrType =="i") type = OC_TYPE_INDUSTRY; else
	if (rcstrType =="w") type = OC_TYPE_WATER; else
	if (rcstrType =="e") type = OC_TYPE_ELECTRICITY; else
	if (rcstrType =="g") type = OC_TYPE_GAS; else
	if (rcstrType =="government") type = OC_TYPE_GOVERNMENT; else
	if (rcstrType =="path") type = OC_TYPE_PATH; else
	if (rcstrType =="tree") type = OC_TYPE_TREE; else
	if (rcstrType =="vehicle") type = OC_TYPE_VEHICLE;

	return type;
}


   /*=====================================================================*/
const OPENCITY_DIRECTION
Property::_Str2Direction(const string& rcstrDir)
{
	OPENCITY_DIRECTION dir = OC_DIR_UNDEFINED;

	if (rcstrDir == "on") dir = OC_DIR_O_N; else
	if (rcstrDir == "oe") dir = OC_DIR_O_E; else
	if (rcstrDir == "os") dir = OC_DIR_O_S; else
	if (rcstrDir == "ow") dir = OC_DIR_O_W; else
	if (rcstrDir == "sn") dir = OC_DIR_S_N; else
	if (rcstrDir == "we") dir = OC_DIR_W_E; else
	if (rcstrDir == "ne") dir = OC_DIR_N_E; else
	if (rcstrDir == "nw") dir = OC_DIR_N_W; else
	if (rcstrDir == "se") dir = OC_DIR_S_E; else
	if (rcstrDir == "sw") dir = OC_DIR_S_W; else
	if (rcstrDir == "sne") dir = OC_DIR_S_N_E; else
	if (rcstrDir == "swe") dir = OC_DIR_S_W_E; else
	if (rcstrDir == "snw") dir = OC_DIR_S_N_W; else
	if (rcstrDir == "nwe") dir = OC_DIR_N_W_E; else
	if (rcstrDir == "snwe") dir = OC_DIR_S_N_W_E;

	return dir;
}


































