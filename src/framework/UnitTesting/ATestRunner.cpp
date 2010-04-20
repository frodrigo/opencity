/***************************************************************************
                      CTestRunner.cpp  -  description
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

// Framework headers
#include "ATestRunner.h"			// UnitTesting::TestRunner abstract class
#include "ATestClass.h"				// UnitTesting::TestClass class
#include "System/CConsole.h"		// System::Console class
#include "System/Collections/Generic/CList.h"	// System::Collections::Generic::List


using namespace System::Collections::Generic;

SPF_NAMESPACE_BEGIN(UnitTesting)


   /*=====================================================================*/
TestRunner::TestRunner() {}


TestRunner::~TestRunner() {}


   /*=====================================================================*/
void TestRunner::Add(const TestClass& testClass)
{
	mcTestClasses.Add(testClass);
}


void TestRunner::Run()
{
	int count = mcTestClasses.GetCount();
	for (int i = 0; i < count; i++) {
		TestClass testClass = mcTestClasses[i];

		// TODO: catch exception here
		List<TestResult> runResults = testClass.Run();

		// Test: display the results
		List<TestMethod> testMethods = testClass.GetTestMethods();
		int numberMethods = testMethods.GetCount();
		for (int j = 0; j < numberMethods; j++) {
			TestResult runResult = runResults[j];
			TestMethod testMethod = testMethods[j];

			if (runResult == TestResult::Passed) {
				System::Terminal << testMethod.GetDescription() << " passed.\n";
			}
			else {
				System::Terminal << testMethod.GetDescription() << " failed.\n";
			}
		}
	} // for
}


   /*=====================================================================*/
System::String TestRunner::ToString() const
{
	return System::String("UnitTesting::TestRunner");
}


SPF_NAMESPACE_END
