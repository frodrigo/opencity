/***************************************************************************
                        CXmlNodeList.cpp  -  description
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
#include "CXmlNodeList.h"
#include "../CString.h"


   /*=====================================================================*/
namespace System
{
namespace Xml
{
	XmlNodeList::XmlNodeList() {}

	XmlNodeList::~XmlNodeList() {}

	String XmlNodeList::ToString() const
	{
		return String("System.Xml.XmlNodeList");
	}
} // namespace Xml
} // namespace System
