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

// Project includes
#include "propertymanager2.h"
#include "property.h"

// Libraries includes
#include "xpath_processor.h"

#define OC_METADATA_XML_FILE		"config/graphism.xml"
#define OC_METADATA_FILE_NODE		"//graphism/file"
#define OC_METADATA_MODEL_NODE		"//object/model"

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
		_loadProperties( i, ocHomeDirPrefix(filename) );
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
PropertyManager2::_loadProperties
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

// Select the "//object/model" node
	uint nbNode = 0;
	xpath_processor* xpProcessor = new xpath_processor(pRoot, OC_METADATA_MODEL_NODE);
	assert( xpProcessor != NULL );
	nbNode = xpProcessor->u_compute_xpath_node_set();
	assert( nbNode == 1 );	// There must be only 1 node

// FOR each <file> node DO
	TiXmlElement* pElement = NULL;
	pElement = (xpProcessor->XNp_get_xpath_node(0))->ToElement();

// Fill the structure
	Property* sProperty = &_aProperty[index];
	pElement->QueryIntAttribute( "width", (int*)&sProperty->uiWidth );
	pElement->QueryIntAttribute( "length", (int*)&sProperty->uiLength );
	pElement->QueryFloatAttribute( "height", &sProperty->fHeight );
	OPENCITY_DEBUG( "W/L/H" << sProperty->uiWidth << "/" << sProperty->uiLength << "/" << sProperty->fHeight );

// Clean up
	delete xpProcessor;
	xpProcessor = NULL;		// Safe
}
