/***************************************************************************
                       ATestRunner.h  -  description
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

#ifndef _SPF_UNITTESTING_ATESTRUNNER_H_
#define _SPF_UNITTESTING_ATESTRUNNER_H_ 1

// Framework headers
#include "System/Collections/Generic/CList.h"	// System::Collections::Generic::List
#include "CTestResult.h"						// UnitTesting::TestResult


SPF_NAMESPACE_BEGIN(UnitTesting)

// Framework UnitTesting::TestClass class forward declaration
class TestClass;

/**
	Represents an abstract test runner class.

	\see	ConsoleTestRunner
*/
class TestRunner : public System::Object
{
	public:
		TestRunner();
		virtual ~TestRunner();

		virtual const System::Collections::Generic::List<TestClass>& GetTestClasses() const;

/**
	Adds the specified test class to this test runner.

	\param	testClass	The test class to add.
	\see	TestClass
*/
		virtual void Add(const TestClass& testClass);

/**
	Runs this all the tests contained in this runner.
*/
		virtual void Run();

/**
	Displays the test results to the output.
*/
		virtual void DisplayResults() = 0;

		virtual System::String ToString() const;

	protected:
		System::Collections::Generic::List<TestClass> mcTestClasses;

}; // class UnitTesting::TestRunner

// namespace UnitTesting
SPF_NAMESPACE_END

#endif
