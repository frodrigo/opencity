/***************************************************************************
                        CTestMethod.h  -  description
							-------------------
	begin                : April 19th, 2010
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

#ifndef _SPF_UNITTESTING_CTESTMETHOD_H_
#define _SPF_UNITTESTING_CTESTMETHOD_H_ 1

// Framework headers
#include "System/CString.h"			// System::String class
#include "System/ADelegate.h"		// System::Delegate abstract class
#include "CTestResult.h"			// UnitTesting::TestResult enumeration

SPF_NAMESPACE_BEGIN(UnitTesting)

/**
	Represents a unit test method.
*/
class TestMethod : public System::Object
{
	public:
		TestMethod(const TestResult& expectedResult, const System::Delegate& delegate);
		TestMethod(const TestResult& expectedResult, const System::Delegate& delegate, const System::String& description);
		virtual ~TestMethod();

		const TestResult GetExpectedTestResult() const;
		const System::String GetDescription() const;
		void Run() const;

		virtual System::String ToString() const;

	private:
		TestResult meExpectedResult;
		System::Delegate moDelegate;
		System::String msDescription;


}; // class UnitTesting::TestMethod

// namespace UnitTesting
SPF_NAMESPACE_END

#endif
