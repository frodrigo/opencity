/***************************************************************************
                       CStream.cpp  -  description
							-------------------
	begin                : January 11th, 2009
	copyright            : (C) 2009 by Duong Khang NGUYEN
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
#include "CStream.h"			// System::IO::Stream class
#include "../CString.h"			// System::String class


SPF_NAMESPACE_NESTED_BEGIN(System, IO)


   /*=====================================================================*/
Stream::Stream() {}


Stream::~Stream() {}


   /*=====================================================================*/
String Stream::ToString() const
{
	return String("System::IO::Stream");
}


SPF_NAMESPACE_NESTED_END
