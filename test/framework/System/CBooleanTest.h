/***************************************************************************
                        CBooleanTest.h  -  description
							-------------------
	begin                : April 26th, 2010
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

#ifndef _SPF_TEST_CBOOLEANTEST_H_
#define _SPF_TEST_CBOOLEANTEST_H_ 1

// Framework headers
#include "UnitTesting/ATestClass.h"

SPF_NAMESPACE_BEGIN(Test)


/**
	Contains the System::Boolean unit tests.
*/
class BooleanTest : public UnitTesting::TestClass
{
	public:
		BooleanTest();
		virtual ~BooleanTest();

		void AssignmentOperatorTest() const;
		void EqualOperatorTest() const;
		void InequalOperatorTest() const;

		virtual System::String ToString() const;

}; // class Test::BooleanTest

// namespace Test
SPF_NAMESPACE_END

#endif
