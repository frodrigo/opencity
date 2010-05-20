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
#include "System/CNullValue.h"		// System::NullValue

// Debug
//#include "System/CConsole.h"		// System::Console class
#include "System/CNotImplementedException.h"

SPF_NAMESPACE_BEGIN(UnitTesting)


   /*=====================================================================*/
TestMethod::TestMethod(const System::Delegate& delegate, const TestResult& expectedResult):
	meExpectedResult(expectedResult),
	moExpectedType(System::Null),
	moDelegate(delegate)
{
}


TestMethod::TestMethod(const System::Delegate& delegate, const System::String& description, const TestResult& expectedResult):
	meExpectedResult(expectedResult),
	moExpectedType(System::Null),
	moDelegate(delegate),
	msDescription(description)
{
//	System::Terminal << "ctor + desc.\n";
}


TestMethod::TestMethod(const System::Delegate& delegate, const System::String& description, const System::Type& expectedType):
	meExpectedResult(TestResult::Failed),
	moExpectedType(expectedType),
	moDelegate(delegate),
	msDescription(description)
{
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


const System::Type& TestMethod::GetExpectedType() const
{
	return moExpectedType;
}


void TestMethod::SetExpectedType(const System::Type& expectedType)
{
	moExpectedType = expectedType;
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
	if (moExpectedType.GetFullName() == System::String::Empty)
		meFinalResult = this->RunWithExpectedResult();
	else
		meFinalResult = this->RunWithExpectedType();

	return meFinalResult;
}


   /*=====================================================================*/
TestResult TestMethod::RunWithExpectedResult()
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
	TestResult result;
	if (meRunResult == meExpectedResult) {
//		System::Terminal << "passed.\n";
		result = TestResult::Passed;
	}
	else {
//		System::Terminal << "failed.\n";
		result = TestResult::Failed;
	}

	return result;
}


TestResult TestMethod::RunWithExpectedType()
{
	System::Type exceptionType;

	// Try to execute the delegate.
	try {
		moDelegate.DynamicInvoke();
		meRunResult = TestResult::Passed;
	}
	catch (System::Exception* pException) {
		exceptionType = pException->GetType();
		delete pException;
		meRunResult = TestResult::Failed;
	}

	// Compare the run result to the expected test result.
	// The expected result for an exception unit test method is always TestResult::Failed
	TestResult result;
	if (meRunResult == meExpectedResult) {
		if (exceptionType == moExpectedType) {
//			System::Terminal << "passed.\n";
			result = TestResult::Passed;
		}
		else {
			result = TestResult::Failed;
		}
	}
	else {
//		System::Terminal << "failed.\n";
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
