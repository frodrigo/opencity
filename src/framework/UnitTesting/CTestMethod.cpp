/***************************************************************************
                        CTestMethod.cpp  -  description
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

// Framework headers
#include "CTestMethod.h"			// UnitTesting::TestMethod class
#include "CAssertFailedException.h"	// UnitTesting::AssertFailedException class

// Debug
//#include "System/CConsole.h"		// System::Console class

SPF_NAMESPACE_BEGIN(UnitTesting)


   /*=====================================================================*/
TestMethod::TestMethod(const System::Delegate& delegate, const TestResult& expectedResult):
	meExpectedResult(expectedResult),
	moDelegate(delegate)
{
}


TestMethod::TestMethod(const System::Delegate& delegate, const System::String& description, const TestResult& expectedResult):
	meExpectedResult(expectedResult),
	moDelegate(delegate),
	msDescription(description)
{
//	System::Terminal << "ctor + desc.\n";
}


TestMethod::~TestMethod()
{
//	System::Terminal << "dtor.\n";
}


   /*=====================================================================*/
const TestResult& TestMethod::GetRunResult() const
{
	return meRunResult;
}


const TestResult& TestMethod::GetExpectedResult() const
{
	return meExpectedResult;
}


void TestMethod::SetExpectedResult(const TestResult& expectedResult)
{
	meExpectedResult = expectedResult;
}


const TestResult& TestMethod::GetFinalResult() const
{
	return meFinalResult;
}


const System::String& TestMethod::GetDescription() const
{
	return msDescription;
}


const TestResult& TestMethod::Run()
{
	// Try to execute the delegate.
	try {
		moDelegate.DynamicInvoke();
		meRunResult = TestResult::Passed;
	}
	catch (AssertFailedException exception) {
		meRunResult = TestResult::Failed;
	}

	// Compare the run result to the expected test result.
	if (meRunResult == meExpectedResult) {
//		System::Terminal << "passed.\n";
		meFinalResult = TestResult::Passed;
	}
	else {
//		System::Terminal << "failed.\n";
		meFinalResult = TestResult::Failed;
	}

	return meFinalResult;
}


   /*=====================================================================*/
System::String TestMethod::ToString() const
{
	return System::String("UnitTesting::TestMethod");
}


SPF_NAMESPACE_END
