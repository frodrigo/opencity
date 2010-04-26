/***************************************************************************
                      CConsoleTestRunner.cpp  -  description
							-------------------
	begin                : November 29th, 2009
	copyright            : (C) 2009 by Duong Khang NGUYEN
	email                : neoneurone @ gmail com

	$Id: AConsoleTestRunner.cpp 440 2010-04-20 15:30:32Z neoneurone $
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
#include "CConsoleTestRunner.h"		// UnitTesting::ConsoleTestRunner abstract class
#include "ATestClass.h"				// UnitTesting::TestClass class
#include "System/CConsole.h"		// System::Console class
#include "System/Collections/Generic/CList.h"	// System::Collections::Generic::List


using namespace System::Collections::Generic;

SPF_NAMESPACE_BEGIN(UnitTesting)


   /*=====================================================================*/
ConsoleTestRunner::ConsoleTestRunner() {}


ConsoleTestRunner::~ConsoleTestRunner() {}


   /*=====================================================================*/
void ConsoleTestRunner::DisplayResults()
{
	int count = mcTestClasses.GetCount();
	for (int i = 0; i < count; i++) {
		// TODO: display test class description
		const TestClass& testClass = mcTestClasses[i];
		System::String description = testClass.GetDescription();
		if (description.GetLength() > 0) {
			System::Terminal << description;
		}

		// Test: display the results
		const List<TestMethod>& testMethods = testClass.GetTestMethods();
		int numberMethods = testMethods.GetCount();
		for (int j = 0; j < numberMethods; j++) {
			const TestMethod& testMethod = testMethods[j];

			System::String message = "\t";
			message += testMethod.GetDescription();
			const TestResult& finalResult = testMethod.GetFinalResult();
			if (finalResult == TestResult::Passed) {
				message += " passed.\n";
			} else
			if (finalResult == TestResult::Failed) {
				message += " failed.\n";
			}
			else {
				message += " undefined.\n";
			}

			System::Terminal << message;
		} // for method
	} // for class
}


   /*=====================================================================*/
System::String ConsoleTestRunner::ToString() const
{
	return System::String("UnitTesting::ConsoleTestRunner");
}


SPF_NAMESPACE_END
