/***************************************************************************
						propertymanager2.cpp  -  description
							-------------------
	begin                : january 28th, 2007
	copyright            : (C) 2007-2008 by Duong-Khang NGUYEN
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

// OpenCity headers
#include "propertymanager2.h"

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
		"Reading XML metadata file: \"" << ocConfigDirPrefix(OC_METADATA_XML_FILE) << "\""
	);

// Load the XML metadata file list
	string fn = ocConfigDirPrefix(OC_METADATA_XML_FILE);
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

// Allocate memory to store the property array
	_uiNumberProperty = nbFile;
	_aProperty = new Property* [_uiNumberProperty];

// FOR each <file> node DO
	uint i = 0;
	string strAc = string(".ac");
	string strXml = string(".xml");
	string fileGraphism, fileXml;
	string::size_type pos;
	TiXmlElement* pElement = NULL;
	for (i = 0; i < nbFile; i++) {
		pElement = (xpProcessor->XNp_get_xpath_node(i))->ToElement();
		fileGraphism = pElement->GetText();
		fileXml = fileGraphism;

		// Replace ".ac" by ".xml"
		pos = fileXml.rfind( strAc );
		if (pos != fileXml.npos) {
			fileXml.replace( pos, strXml.size(), strXml );
		}

	// Load the properties at the index i
		if (_mapIndex.find(fileGraphism) != _mapIndex.end()) {
			OPENCITY_DEBUG(
				"XML file \"" << fileGraphism <<
				"\" already exists at : " << _mapIndex[fileGraphism]
			);
			continue;
		}

		_aProperty[i] = Property::LoadProperties( ocDataDirPrefix(fileXml) );
		_mapIndex[fileGraphism] = i;
	}

// Clean up
	delete xpProcessor;
	xpProcessor = NULL;		// Safe
}


   /*=====================================================================*/
PropertyManager2::~PropertyManager2()
{
	OPENCITY_DEBUG( "dtor" );

// Delete the whole property array
	delete [] _aProperty;
}


   /*=====================================================================*/
const uint
PropertyManager2::GetNumberProperty() const
{
	return _uiNumberProperty;
}


   /*=====================================================================*/
const Property* const
PropertyManager2::Get(const string& key) const
{
	std::map<string, uint>::const_iterator cit = _mapIndex.find(key);
	return cit != _mapIndex.end() ? _aProperty[cit->second] : NULL;
}


   /*=====================================================================*/
const Property* const
PropertyManager2::Get(const uint index) const
{
	return _aProperty[index];
}


































