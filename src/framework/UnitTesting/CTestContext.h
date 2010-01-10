/***************************************************************************
                        CTestContext.h  -  description
							-------------------
	begin                : November 29th, 2009
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

#ifndef _SPF_UNITTESTING_CTESTCONTEXT_H_
#define _SPF_UNITTESTING_CTESTCONTEXT_H_ 1

// Framework headers
#include "System/CObject.h"

SPF_NAMESPACE_BEGIN(UnitTesting)


/**
	Verifies conditions in unit tests using true/false propositions.
*/
class TestContext : public System::Object
{
	public:
		TestContext();
		virtual ~TestContext();

		virtual System::String ToString() const;

}; // class UnitTesting::TestContext

// namespace UnitTesting
SPF_NAMESPACE_END

#endif
