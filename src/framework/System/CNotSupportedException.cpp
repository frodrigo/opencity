/***************************************************************************
                  CNotSupportedException.cpp  -  description
							-------------------
	begin                : February 16th, 2010
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
#include "CNotSupportedException.h"	// System::NotSupportedException class


SPF_NAMESPACE_BEGIN(System)


   /*=====================================================================*/
NotSupportedException::NotSupportedException() {}


NotSupportedException::NotSupportedException(const String& message) :
SystemException(message)
{}


NotSupportedException::~NotSupportedException() {}


   /*=====================================================================*/
SPF_NAMESPACE_END
