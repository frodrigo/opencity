/***************************************************************************
                        CException.cpp  -  description
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
#include "CException.h"			// Exception class


SPF_NAMESPACE_BEGIN(System)


   /*=====================================================================*/
Exception::Exception() {}


Exception::Exception(const String& message) : msMessage(message) {}


Exception::~Exception() {}


   /*=====================================================================*/
const String& Exception::GetMessage() const
{
	return msMessage;
}

   /*=====================================================================*/
String Exception::ToString() const
{
	return msMessage;
}


SPF_NAMESPACE_END
