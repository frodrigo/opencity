/***************************************************************************
                       CTestRunner.h  -  description
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

#ifndef _SPF_UNITTESTING_CTESTRUNNER_H_
#define _SPF_UNITTESTING_CTESTRUNNER_H_ 1

// Framework headers
#include "System/Collections/Generic/CList.h"	// System::Collections::Generic::List

SPF_NAMESPACE_BEGIN(UnitTesting)

// Framework UnitTesting::TestClass class forward declaration
class TestClass;

/**
*/
class TestRunner : public System::Object
{
	public:
		TestRunner();
		virtual ~TestRunner();

		virtual void Add(const TestClass& testClass);
		virtual void Run();

		virtual System::String ToString() const;

	private:
		System::Collections::Generic::List<TestClass> mcTestClasses;

}; // class UnitTesting::TestRunner

// namespace UnitTesting
SPF_NAMESPACE_END

#endif
