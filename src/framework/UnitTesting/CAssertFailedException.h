/***************************************************************************
                        CAssertFailedException.h  -  description
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

#ifndef _SPF_UNITTEST_ASSERTFAILEDEXCEPTION_H_
#define _SPF_UNITTEST_ASSERTFAILEDEXCEPTION_H_ 1

// Framework headers
#include "CUnitTestAssertException.h"	// UnitTest::UnitTestAssertException base class

SPF_NAMESPACE_BEGIN(UnitTesting)

/**
	Represents errors that occur during application execution
*/
class AssertFailedException : public UnitTestAssertException
{
	public:
		AssertFailedException();
		AssertFailedException(const System::String& message);
		virtual ~AssertFailedException();

}; // class UnitTest::AssertFailedException

SPF_NAMESPACE_END

#endif
