/***************************************************************************
                        CAssertFailedException.cpp  -  description
							-------------------
	begin                : January 10th, 2010
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
#include "CAssertFailedException.h"			// AssertFailedException class

SPF_NAMESPACE_BEGIN(UnitTesting)


   /*=====================================================================*/
AssertFailedException::AssertFailedException() {}


AssertFailedException::AssertFailedException(const System::String& message) :
UnitTestAssertException(message)
{}


AssertFailedException::~AssertFailedException() {}
   /*=====================================================================*/


SPF_NAMESPACE_END
