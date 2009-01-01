/***************************************************************************
                        CXmlElement.cpp  -  description
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
#include "CXmlElement.h"
#include "../CString.h"


   /*=====================================================================*/
namespace System
{
namespace Xml
{
	XmlElement::XmlElement() {}


	XmlElement::XmlElement(const String& name) : XmlLinkedNode(name) {}


	XmlElement::~XmlElement() {}


	String XmlElement::ToString() const
	{
		return String("System::Xml::XmlElement");
	}
} // namespace Xml
} // namespace System
