/***************************************************************************
                       CTestRunner.h  -  description
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

#ifndef _SPF_UNITTESTING_CTESTRUNNER_H_
#define _SPF_UNITTESTING_CTESTRUNNER_H_ 1

// Framework headers
#include "System/CObject.h"

SPF_NAMESPACE_BEGIN(UnitTesting)


/**
*/
class TestRunner : public System::Object
{
	public:
		TestRunner();
		virtual ~TestRunner();

		virtual System::String ToString() const;

}; // class UnitTesting::TestRunner

// namespace UnitTesting
SPF_NAMESPACE_END

#endif
