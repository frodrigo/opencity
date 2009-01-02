/***************************************************************************
                        CXmlNode.cpp  -  description
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
#include "CXmlNode.h"


SPF_NAMESPACE_NESTED_BEGIN(System, Xml)


   /*=====================================================================*/
XmlNode::XmlNode() {}


XmlNode::XmlNode(const String& name) : msName(name) {}


XmlNode::~XmlNode() {}


   /*=====================================================================*/
String XmlNode::GetName() const
{
	return msName;
}


String XmlNode::ToString() const
{
	return String("System::Xml::XmlNode");
}


SPF_NAMESPACE_NESTED_END
