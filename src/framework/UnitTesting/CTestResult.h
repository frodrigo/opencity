/***************************************************************************
                      CTestResult.h  -  description
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

#ifndef _SPF_SYSTEM_CTESTRESULT_H_
#define _SPF_SYSTEM_CTESTRESULT_H_ 1

// Framework headers
#include "System/CEnum.h"			// System::Enum class

SPF_NAMESPACE_BEGIN(UnitTesting)

/**
	Specifies a result for a unit test.
*/
class TestResult : public System::Enum
{
	public:
/**
	Undefined test result.
*/
		static const TestResult Undefined;

/**
	The unit test has passed.
*/
		static const TestResult Passed;

/**
	The unit test has failed.
*/
		static const TestResult Failed;

		TestResult();
		TestResult(const TestResult& original);
		virtual ~TestResult();

		virtual System::String ToString() const;

	private:
		TestResult(int value);
}; // class UnitTesting::TestResult

SPF_NAMESPACE_END

#endif
