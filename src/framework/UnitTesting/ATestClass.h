/***************************************************************************
                        CTestClass.h  -  description
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

#ifndef _SPF_UNITTESTING_ATESTCLASS_H_
#define _SPF_UNITTESTING_ATESTCLASS_H_ 1

// Framework headers
#include "CTestMethod.h"						// UnitTesting::TestMethod
#include "System/Collections/Generic/CList.h"	// System::Collections::Generic::List

SPF_NAMESPACE_BEGIN(UnitTesting)

/**
	Represents a unit test class.
*/
class TestClass : public System::Object
{
	public:
		TestClass(const System::String& description = System::String::Empty);
		virtual ~TestClass();

		const System::String& GetDescription() const;
		virtual const System::Collections::Generic::List<TestMethod>& GetTestMethods() const;

/**
	Adds the specified test method to this test class.

	\param	testMethod	The test method to add.
	\see	TestMethod
*/
		virtual void Add(const TestMethod& testMethod);

/**
	Runs this test class.

	\return	A collection containing the results of the tests.
	\see	TestResult
*/
		virtual const System::Collections::Generic::List<TestResult> Run();

		virtual System::String ToString() const;

	private:
		System::String msDescription;
		System::Collections::Generic::List<TestMethod> mcTestMethods;

}; // class UnitTesting::TestClass

// namespace UnitTesting
SPF_NAMESPACE_END

#endif
