/***************************************************************************
                        CStringTest.h  -  description
							-------------------
	begin                : April 23th, 2010
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

#ifndef _SPF_TEST_CSTRINGTEST_H_
#define _SPF_TEST_CSTRINGTEST_H_ 1

// Framework headers
#include "UnitTesting/ATestClass.h"

SPF_NAMESPACE_BEGIN(Test)


/**
	Contains the System::String unit tests.
*/
class StringTest : public UnitTesting::TestClass
{
	public:
		StringTest();
		virtual ~StringTest();

		void StringEmptyTest() const;

		void GetLengthTest() const;

		void EqualOperatorTest() const;
		void AssignmentOperatorTest() const;
		void PlusOperatorTest() const;
		void PlusEqualOperatorTest() const;

		void LeftShiftOperatorTest() const;

		virtual System::String ToString() const;

}; // class UnitTesting::StringTest

// namespace UnitTesting
SPF_NAMESPACE_END

#endif
