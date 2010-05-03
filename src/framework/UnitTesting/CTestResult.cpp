/***************************************************************************
                      CTestResult.cpp  -  description
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
#include "CTestResult.h"		// UnitTesting::TestResult class
#include "System/CString.h"		// System::String class


SPF_NAMESPACE_BEGIN(UnitTesting)


   /*=====================================================================*/
const TestResult TestResult::Undefined(-1);
const TestResult TestResult::Passed(0x0000);
const TestResult TestResult::Failed(0x0001);


   /*=====================================================================*/
TestResult::TestResult():
	Enum(TestResult::Undefined)
{}


TestResult::TestResult(const TestResult& original):
	Enum(original)
{}


TestResult::TestResult(int value):
	Enum(value)
{}


TestResult::~TestResult() {}


   /*=====================================================================*/
System::String TestResult::ToString() const
{
	return System::String("System::TestResult");
}


SPF_NAMESPACE_END
