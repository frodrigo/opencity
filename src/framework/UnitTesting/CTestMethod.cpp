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


SPF_NAMESPACE_BEGIN(UnitTesting)


   /*=====================================================================*/
TestMethod::TestMethod(const TestResult& expectedResult, const System::Delegate& delegate):
meExpectedResult(expectedResult),
moDelegate(delegate)
{
}


TestMethod::TestMethod(const TestResult& expectedResult, const System::Delegate& delegate, const System::String& description):
meExpectedResult(expectedResult),
moDelegate(delegate),
msDescription(description)
{
}


TestMethod::~TestMethod()
{
}


   /*=====================================================================*/
const TestResult TestMethod::GetExpectedTestResult() const
{
	return meExpectedResult;
}


const System::String TestMethod::GetDescription() const
{
	return msDescription;
}


const TestResult TestMethod::Run() const
{
	TestResult result;

	try {
		moDelegate.DynamicInvoke();
		result = TestResult::Passed;
	}
	catch (AssertFailedException exception) {
		result = TestResult::Failed;
	}

	return result;
}


   /*=====================================================================*/
System::String TestMethod::ToString() const
{
	return System::String("UnitTesting::TestMethod");
}


SPF_NAMESPACE_END
