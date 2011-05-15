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
#include "System/CType.h"			// System::Type class
#include "CTestResult.h"			// UnitTesting::TestResult enumeration

SPF_NAMESPACE_BEGIN(UnitTesting)

/**
	Represents a unit test method.
*/
class TestMethod : public System::Object
{
	public:
		TestMethod(const System::Delegate& delegate, const TestResult& expectedResult = TestResult::Passed);
		TestMethod(const System::Delegate& delegate, const System::String& description, const TestResult& expectedResult = TestResult::Passed);

/**
	Initializes a new TestMethod instance with the specified delegate and
expected exception type.

	\param	delegate		The test delegate.
	\param	description		The description of the test method.
	\param	expectedType	The expected exception type.
*/
		TestMethod(const System::Delegate& delegate, const System::String& description, const System::Type& expectedType);
		virtual ~TestMethod();

		const TestResult& GetRunResult() const;

		const TestResult& GetExpectedResult() const;
		void SetExpectedResult(const TestResult& expectedResult);

		const System::Type& GetExpectedType() const;
		void SetExpectedType(const System::Type& expectedType);

		const TestResult& GetFinalResult() const;
		const System::String& GetDescription() const;
		const TestResult& Run();

		virtual System::String ToString() const;

	private:
		TestResult meRunResult;
		TestResult meExpectedResult;
		TestResult meFinalResult;
		System::Type moExpectedType;
		System::Delegate moDelegate;
		System::String msDescription;

		TestResult RunWithExpectedResult();
		TestResult RunWithExpectedType();

}; // class UnitTesting::TestMethod

// namespace UnitTesting
SPF_NAMESPACE_END

#endif
