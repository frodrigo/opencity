/***************************************************************************
                        CXmlDocument.cpp  -  description
							-------------------
	begin                : December 29th, 2008
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

// Framework headers
#include "CXmlDocument.h"
#include "../CString.h"
#include "../CException.h"				// FIXME: file not found exception

// libxml headers
#include <libxml/parser.h>

SPF_NAMESPACE_NESTED_BEGIN(System, Xml)


   /*=====================================================================*/
XmlDocument::XmlDocument() :
	mpXmlDocument(NULL)
{
}


XmlDocument::~XmlDocument()
{
	this->Unload();
}


   /*=====================================================================*/
void XmlDocument::Load(String url)
{
	// Unloads previously loaded XML document
	this->Unload();

	// Loads the new XML document
	mpXmlDocument = xmlReadFile(url, NULL, 0);
	if (mpXmlDocument == NULL) {
		throw Exception("File not found");
	}

	/*
	 * this initialize the library and check potential ABI mismatches
	 * between the version it was compiled for and the actual shared
	 * library used.
	 */
	//TODO
	//LIBXML_TEST_VERSION


	// Get the root element node
/*
	root_element = xmlDocGetRootElement(psXmlDoc);
	print_element_names(root_element);
*/

	// Free the global variables that may have been allocated by the parser.
	xmlCleanupParser();
}


   /*=====================================================================*/
String XmlDocument::ToString() const
{
	return String("System::Xml::XmlDocument");
}


   /*=====================================================================*/
const String XmlDocument::GetOuterXml() const
{
	if (mpXmlDocument == NULL)
		return String::Empty;

	// Dumps the XML document to memory
	xmlChar* lpBuffer	= NULL;
	int liBufferSize	= 0;
	xmlDocDumpMemory(mpXmlDocument, &lpBuffer, &liBufferSize);

	// Converts the character buffer to the String type
	String lsDump(lpBuffer);
	xmlFree(lpBuffer);
	lpBuffer = NULL;

	return lsDump;
}


   /*=====================================================================*/
void XmlDocument::Unload()
{
	if (mpXmlDocument != NULL)
	{
		xmlFreeDoc(mpXmlDocument);
		mpXmlDocument = NULL;
	}
}


SPF_NAMESPACE_NESTED_END
