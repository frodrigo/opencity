/***************************************************************************
                        CXmlNodeList.h  -  description
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

#ifndef _OPENCITY_FRAMEWORK_XML_CXMLNODELIST_H_
#define _OPENCITY_FRAMEWORK_XML_CXMLNODELIST_H_ 1

// Framework headers
#include "../CObject.h"
#include "CXmlNode.h"

// Standard C++ headers
#include <vector>

namespace System
{
namespace Xml
{
	using namespace System;

	class XmlNodeList : Object
	{
		public:
			XmlNodeList();
			virtual ~XmlNodeList();


			// Inherited from System::Object
			virtual String ToString() const;


		private:
			std::vector<XmlNode> mvNode;

	}; // class System::Xml::XmlNodeList
} // namespace System::Xml
} // namespace System

#endif
