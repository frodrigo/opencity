/***************************************************************************
                        CXmlDocument.h  -  description
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

#ifndef _OPENCITY_FRAMEWORK_XML_CXMLDOCUMENT_H_
#define _OPENCITY_FRAMEWORK_XML_CXMLDOCUMENT_H_ 1

// Framework headers
#include "CXmlNode.h"

SPF_NAMESPACE_BEGIN(System)

// Forward System::String class declaration
class String;

SPF_NAMESPACE_BEGIN(Xml)

class XmlDocument : XmlNode
{
	public:
		XmlDocument();
		virtual ~XmlDocument();

		virtual String ToString() const;

}; // class System::Xml::XmlDocument

// namespace System::Xml
SPF_NAMESPACE_END

// namespace System
SPF_NAMESPACE_END

#endif
