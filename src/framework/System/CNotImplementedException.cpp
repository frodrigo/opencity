/***************************************************************************
                  CNotImplementedException.cpp  -  description
							-------------------
	begin                : February 7th, 2010
	copyright            : (C) 2010 by Duong Khang NGUYEN
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
#include "CNotImplementedException.h"	// System::NotImplementedException class


SPF_NAMESPACE_BEGIN(System)


   /*=====================================================================*/
NotImplementedException::NotImplementedException() {}


NotImplementedException::NotImplementedException(const String& message):
	SystemException(message)
{}


NotImplementedException::~NotImplementedException() {}
   /*=====================================================================*/


SPF_NAMESPACE_END
