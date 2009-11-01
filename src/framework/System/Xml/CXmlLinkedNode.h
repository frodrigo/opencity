/***************************************************************************
                        CXmlLinkedNode.h  -  description
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

#ifndef _SPF_XML_CXMLLINKEDNODE_H_
#define _SPF_XML_CXMLLINKEDNODE_H_ 1

// Framework headers
#include "CXmlNode.h"

SPF_NAMESPACE_BEGIN(System)

// Forward System::String class declaration
class String;

SPF_NAMESPACE_BEGIN(Xml)


/**
	Represents a linked XML node.
*/
class XmlLinkedNode : public XmlNode
{
	public:
		XmlLinkedNode();
		virtual ~XmlLinkedNode();

		virtual String ToString() const;

	protected:
		XmlLinkedNode(
			const String& prefix,
			const String& localName,
			const String& namespaceURI,
			XmlDocument& doc
		);

}; // class System::Xml::XmlElement

// namespace System::Xml
SPF_NAMESPACE_END

// namespace System
SPF_NAMESPACE_END

#endif
