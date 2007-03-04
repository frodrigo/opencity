/***************************************************************************
						propertymanager2.cpp  -  description
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

// Useful enumerations
#include "opencity_direction.h"
#include "opencity_structure_type.h"

// OpenCity headers
#include "propertymanager2.h"
#include "property.h"

// Libraries headers
#include "xpath_processor.h"
#include "xpath_static.h"

// Local defines
#define OC_METADATA_XML_FILE		"config/graphism.xml"
#define OC_METADATA_FILE_NODE		"//graphism/file"


using namespace TinyXPath;


   /*=====================================================================*/
PropertyManager2::PropertyManager2()
{
	OPENCITY_DEBUG( "ctor" );

	TiXmlDocument fileList;			// The graphism file list (in XML)

// Now try to open the config file then read it
	OPENCITY_INFO(
		"Reading XML metadata file: \"" << ocHomeDirPrefix(OC_METADATA_XML_FILE) << "\""
	);

// Load the XML metadata file list
	string fn = ocHomeDirPrefix(OC_METADATA_XML_FILE);
	if (!fileList.LoadFile(fn) || fileList.Error()) {
		OPENCITY_FATAL( fileList.ErrorDesc() );
		abort();
	}

// Get the root element
	TiXmlNode* pRoot = fileList.RootElement();
	if (pRoot == NULL) {
		OPENCITY_FATAL( fileList.ErrorDesc() );
		abort();
	}

// Select all the "//graphism/file" nodes
	uint nbFile = 0;
	xpath_processor* xpProcessor = new xpath_processor(pRoot, OC_METADATA_FILE_NODE);
	assert( xpProcessor != NULL );
	nbFile = xpProcessor->u_compute_xpath_node_set();
	assert( nbFile > 0 );
	OPENCITY_DEBUG( "Number of files expected: " << nbFile );

// Allocate the array to hold all the object properties
	_aProperty = new Property[nbFile];

// FOR each <file> node DO
	uint i = 0;
	string strAc = string(".ac");
	string strOcm = string(".ocm");
	string strXml = string(".xml");
	string filename;
	string::size_type pos;
	TiXmlElement* pElement = NULL;
	for (i = 0; i < nbFile; i++) {
		pElement = (xpProcessor->XNp_get_xpath_node(i))->ToElement();
		filename = pElement->GetText();

		// Replace ".ac" and ".ocm" by ".xml"
		pos = filename.rfind( strAc );
		if (pos != filename.npos ) {
			filename.replace( pos, strXml.size(), strXml );
		}
		else {
			pos = filename.rfind( strOcm );
			if (pos != filename.npos ) {
				filename.replace( pos, strXml.size(), strXml );
			}
		}

	// Load the properties at the index i
		_LoadProperties( i, ocHomeDirPrefix(filename) );
	}

// Clean up
	delete xpProcessor;
	xpProcessor = NULL;		// Safe
}


   /*=====================================================================*/
PropertyManager2::~PropertyManager2()
{
	OPENCITY_DEBUG( "dtor" );

	delete [] _aProperty;
}


   /*=====================================================================*/
   /*                        PRIVATE     METHODS                          */
   /*=====================================================================*/
void
PropertyManager2::_LoadProperties
(
	uint index,
	string filename
)
{
// Parameters checking
	OPENCITY_DEBUG( "Load file: " << filename );
	assert( index >= 0 );
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
	Property* sProperty = &_aProperty[index];

// Select the "/object/property/cost" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_COST_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "build",	(int*)&sProperty->uiBuildCost );
	pElement->QueryIntAttribute( "destroy",	(int*)&sProperty->uiDestroyCost );
	pElement->QueryIntAttribute( "support",	(int*)&sProperty->uiSupportCost );
	pElement->QueryIntAttribute( "income",	(int*)&sProperty->uiIncome );

// RCI ========================================================================
// Select the "/object/property/r/need" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_NEED_R_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&sProperty->sResidence.mmNeed.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&sProperty->sResidence.mmNeed.iMax);

// Select the "/object/property/r/provide" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROVIDE_R_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&sProperty->sResidence.mmProvide.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&sProperty->sResidence.mmProvide.iMax);

// Select the "/object/property/c/need" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_NEED_C_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&sProperty->sCommerce.mmNeed.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&sProperty->sCommerce.mmNeed.iMax);

// Select the "/object/property/c/provide" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROVIDE_C_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&sProperty->sCommerce.mmProvide.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&sProperty->sCommerce.mmProvide.iMax);

// Select the "/object/property/i/need" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_NEED_I_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&sProperty->sIndustry.mmNeed.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&sProperty->sIndustry.mmNeed.iMax);

// Select the "/object/property/i/provide" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROVIDE_I_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&sProperty->sIndustry.mmProvide.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&sProperty->sIndustry.mmProvide.iMax);

// WEG ========================================================================
// Select the "/object/property/w/need" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_NEED_W_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&sProperty->sWater.mmNeed.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&sProperty->sWater.mmNeed.iMax);

// Select the "/object/property/w/provide" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROVIDE_W_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&sProperty->sWater.mmProvide.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&sProperty->sWater.mmProvide.iMax);

// Select the "/object/property/e/need" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_NEED_E_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&sProperty->sElectricity.mmNeed.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&sProperty->sElectricity.mmNeed.iMax);

// Select the "/object/property/e/provide" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROVIDE_E_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&sProperty->sElectricity.mmProvide.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&sProperty->sElectricity.mmProvide.iMax);

// Select the "/object/property/g/need" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_NEED_G_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&sProperty->sGas.mmNeed.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&sProperty->sGas.mmNeed.iMax);

// Select the "/object/property/e/provide" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROVIDE_G_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&sProperty->sGas.mmProvide.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&sProperty->sGas.mmProvide.iMax);

// TN  ========================================================================
// Select the "/object/property/t/need" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_NEED_T_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&sProperty->sTraffic.mmNeed.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&sProperty->sTraffic.mmNeed.iMax);

// Select the "/object/property/t/provide" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROVIDE_T_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&sProperty->sTraffic.mmProvide.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&sProperty->sTraffic.mmProvide.iMax);

// Select the "/object/property/nature/need" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_NEED_NATURE_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&sProperty->sNature.mmNeed.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&sProperty->sNature.mmNeed.iMax);

// Select the "/object/property/nature/provide" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROVIDE_NATURE_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "min",	(int*)&sProperty->sNature.mmProvide.iMin);
	pElement->QueryIntAttribute( "max",	(int*)&sProperty->sNature.mmProvide.iMax);

// ??? ========================================================================
// Select the "/object/property" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_PROPERTY_NODE);
	assert( pNode != NULL );
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "inhabitant",	(int*)&sProperty->uiInhabitant );
	pElement->QueryIntAttribute( "worker",		(int*)&sProperty->uiWorker );
	pElement->QueryIntAttribute( "radius",		(int*)&sProperty->uiRadius );

// Select the "/object/property/@type" attribute
	pAttribute = XAp_xpath_attribute(pRoot, OC_METADATA_STRUCTURE_TYPE_ATTRIBUTE);
	assert( pAttribute != NULL );
	sProperty->eStructureType = _Str2Type(pAttribute->ValueStr());

// Select the "/object/property/direction/@value" attribute
	pAttribute = XAp_xpath_attribute(pRoot, OC_METADATA_DIRECTION_ATTRIBUTE);
	assert( pAttribute != NULL );
	sProperty->eDirection = _Str2Direction(pAttribute->ValueStr());

// Select the "/object/model" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_MODEL_NODE);
	assert( pNode != NULL );
// Get the <model> node and fill the structure with the model dimension
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "width",	(int*)&sProperty->uiWidth );
	pElement->QueryIntAttribute( "length",	(int*)&sProperty->uiLength );
	pElement->QueryFloatAttribute( "height", &sProperty->fHeight );


// Debug
	OPENCITY_DEBUG(
		endl <<
		"W/L/H: " <<
			sProperty->uiWidth << "/" << sProperty->uiLength << "/" << sProperty->fHeight << " | " <<
		"B/D/S/I: " <<
			sProperty->uiBuildCost << "/" << sProperty->uiDestroyCost << "/" << sProperty->uiSupportCost << "/" << sProperty->uiIncome << " | " <<
		"Dir: " <<
			sProperty->eDirection << " | " <<
		"Type: " <<
			sProperty->eStructureType << " | " <<
		"i/w/r: " <<
			sProperty->uiInhabitant << "/" << sProperty->uiWorker << "/" << sProperty->uiRadius << " | " <<
			endl <<
		"r/c/i: " <<
			sProperty->sResidence.mmNeed.iMin << "-" << sProperty->sResidence.mmNeed.iMax << " " <<
			sProperty->sResidence.mmProvide.iMin << "-" << sProperty->sResidence.mmProvide.iMax << " / " <<
			sProperty->sCommerce.mmNeed.iMin << "-" << sProperty->sCommerce.mmNeed.iMax << " " <<
			sProperty->sCommerce.mmProvide.iMin << "-" << sProperty->sCommerce.mmProvide.iMax << " / " <<
			sProperty->sIndustry.mmNeed.iMin << "-" << sProperty->sIndustry.mmNeed.iMax << " " <<
			sProperty->sIndustry.mmProvide.iMin << "-" << sProperty->sIndustry.mmProvide.iMax << " / " <<
			endl <<
		"w/e/g: " <<
			sProperty->sWater.mmNeed.iMin << "-" << sProperty->sWater.mmNeed.iMax << " " <<
			sProperty->sWater.mmProvide.iMin << "-" << sProperty->sWater.mmProvide.iMax << " / " <<
			sProperty->sElectricity.mmNeed.iMin << "-" << sProperty->sElectricity.mmNeed.iMax << " " <<
			sProperty->sElectricity.mmProvide.iMin << "-" << sProperty->sElectricity.mmProvide.iMax << " / " <<
			sProperty->sGas.mmNeed.iMin << "-" << sProperty->sGas.mmNeed.iMax << " " <<
			sProperty->sGas.mmProvide.iMin << "-" << sProperty->sGas.mmProvide.iMax << " / " <<
			endl <<
		"t/n: " <<
			sProperty->sTraffic.mmNeed.iMin << "-" << sProperty->sTraffic.mmNeed.iMax << " " <<
			sProperty->sTraffic.mmProvide.iMin << "-" << sProperty->sTraffic.mmProvide.iMax << " / " <<
			sProperty->sNature.mmNeed.iMin << "-" << sProperty->sNature.mmNeed.iMax << " " <<
			sProperty->sNature.mmProvide.iMin << "-" << sProperty->sNature.mmProvide.iMax
	);
}


   /*=====================================================================*/
const OPENCITY_STRUCTURE_TYPE
PropertyManager2::_Str2Type(const string& rcstrType)
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
PropertyManager2::_Str2Direction(const string& rcstrDir)
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


































