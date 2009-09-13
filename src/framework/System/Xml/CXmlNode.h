/***************************************************************************
                        CXmlNode.h  -  description
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

#ifndef _OPENCITY_FRAMEWORK_XML_CXMLNODE_H_
#define _OPENCITY_FRAMEWORK_XML_CXMLNODE_H_ 1

// Framework headers
#include "../CObject.h"
#include "../CString.h"

SPF_NAMESPACE_NESTED_BEGIN(System, Xml)

// Framework System::NullValue class forward declaration
class XmlDocument;

using namespace System;


/**
	Represents a single node in the XML document.
*/
class XmlNode : public Object
{
	public:
		XmlNode();
		virtual ~XmlNode();


		/**
			Gets the name of the node with the prefix removed.
		*/
		virtual const String& GetLocalName() const;

		/**
			Gets the qualified name of the node.
		The name returned is dependent on the NodeType of the node.
			\return The qualified name of the node.
		*/
		virtual String GetName() const;

		/**
			Gets the namespace URI of this node.
		*/
		virtual const String& GetNamespaceURI() const;

		/**
			Gets the markup representing this node and all its child nodes.
		*/
		virtual const String GetOuterXml() const;

		/**
			Gets the XmlDocument to which this node belongs.
		*/
		virtual const XmlDocument& GetOwnerDocument() const;

		/**
			Gets the the namespace prefix of this node.
		*/
		virtual const String& GetPrefix() const;

		/**
			Sets the the namespace prefix of this node.
		*/
		virtual void SetPrefix(const String& value);


		// Inherited from System::Object
		virtual String ToString() const;

	protected:
		XmlNode(
			const String& prefix,
			const String& localName,
			const String& namespaceURI,
			XmlDocument& doc
		);

		String msPrefix;
		String msLocalName;
		String msNamespaceURI;
		XmlDocument* poOwnerDocument;
}; // class System::Xml::XmlNode

// namespace System::Xml
SPF_NAMESPACE_NESTED_END

#endif
