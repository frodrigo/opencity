/***************************************************************************
						propertymanager2.cpp  -  description
							-------------------
	begin                : january 28th, 2007
	copyright            : (C) 2007 by Duong-Khang NGUYEN
	email                : neoneurone @ users sourceforge net

	$Id: propertymanager.cpp 115 2006-12-30 23:38:43Z neoneurone $
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
// Initialize the default values
	sProperty->uiBuildCost		= 0;
	sProperty->uiDestroyCost	= 0;
	sProperty->uiSupportCost	= 0;
	sProperty->uiIncome			= 0;
// Get the <cost> node and fill the structure with the model cost properties
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "build",	(int*)&sProperty->uiBuildCost );
	pElement->QueryIntAttribute( "destroy",	(int*)&sProperty->uiDestroyCost );
	pElement->QueryIntAttribute( "support",	(int*)&sProperty->uiSupportCost );
	pElement->QueryIntAttribute( "income",	(int*)&sProperty->uiIncome );

// Select the "/object/property/direction[@value]" attribute
	pAttribute = XAp_xpath_attribute(pRoot, OC_METADATA_DIRECTION_ATTRIBUTE);
	assert( pAttribute != NULL );
	sProperty->eDirection = _Str2Enum(pAttribute->ValueStr());

// Select the "/object/model" node
	pNode = XNp_xpath_node(pRoot, OC_METADATA_MODEL_NODE);
	assert( pNode != NULL );
// Get the <model> node and fill the structure with the model dimension
	pElement = pNode->ToElement();
	pElement->QueryIntAttribute( "width",	(int*)&sProperty->uiWidth );
	pElement->QueryIntAttribute( "length",	(int*)&sProperty->uiLength );
	pElement->QueryFloatAttribute( "height", &sProperty->fHeight );



/* old code, kept for reference, feb 11th, 07
	uint nbNode = 0;
	xpath_processor* xpProcessor = NULL;
	xpProcessor = new xpath_processor(pRoot, OC_METADATA_MODEL_NODE);
	assert( xpProcessor != NULL );
	nbNode = xpProcessor->u_compute_xpath_node_set();
	assert( nbNode == 1 );	// There must be only 1 node
	pElement = (xpProcessor->XNp_get_xpath_node(0))->ToElement();
	delete xpProcessor;
	xpProcessor = NULL;
*/

// Debug
	OPENCITY_DEBUG(
		"W/L/H: " << sProperty->uiWidth << "/" << sProperty->uiLength << "/" << sProperty->fHeight << " | " <<
		"B/D/S/I: " << sProperty->uiBuildCost << "/" << sProperty->uiDestroyCost << "/" << sProperty->uiSupportCost << "/" << sProperty->uiIncome << " | " <<
		"Dir: " << sProperty->eDirection
	);
}


   /*=====================================================================*/
const OPENCITY_DIRECTION
PropertyManager2::_Str2Enum(const string& rcstrDir)
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




































