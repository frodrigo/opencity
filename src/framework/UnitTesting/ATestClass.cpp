/***************************************************************************
                        CTestClass.cpp  -  description
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
#include "ATestClass.h"				// UnitTesting::TestClass abstract class
#include "CAssertFailedException.h"	// UnitTesting::AssertFailedException class
#include "System/CConsole.h"			// System::Console class


SPF_NAMESPACE_BEGIN(UnitTesting)


   /*=====================================================================*/
TestClass::TestClass() {}


TestClass::~TestClass() {}


   /*=====================================================================*/
void TestClass::Add(const TestMethod& method)
{
	mcTestMethods.Add(method);
}


void TestClass::Run()
{
	int count = mcTestMethods.GetCount();
	for (int i = 0; i < count; i++) {
		TestResult result;
		TestMethod method = mcTestMethods[i];

		try {
			method.Run();
			result = TestResult::Passed;
			System::Terminal << method.GetDescription() + " passed. \n";
		}
		catch (AssertFailedException exception) {
			result = TestResult::Failed;
			System::Terminal << method.GetDescription() + " failed. \n";
		}
	} // for
}


   /*=====================================================================*/
System::String TestClass::ToString() const
{
	return System::String("UnitTesting::TestClass");
}


SPF_NAMESPACE_END
