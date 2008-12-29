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


namespace System
{
namespace Xml
{
	using namespace System;

	class XmlNode : Object
	{
		public:
			XmlNode();
			XmlNode(const String& name);
			virtual ~XmlNode();

			virtual String GetName() const;

			// Inherited from System::Object
			virtual String ToString() const;


		private:
			/// XmlNode name
			String msName;

	}; // class System::String
} // namespace Xml
} // namespace System

#endif
