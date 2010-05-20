/***************************************************************************
                        CBooleanTest.cpp  -  description
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

// Framework headers
#include "CBooleanTest.h"			// Test::BooleanTest class
#include "UnitTesting/AAssert.h"	// UnitTesting::Assert class
#include "System/CBoolean.h"		// System::Boolean class
#include "System/CNotImplementedException.h"	// System::NotImplementedException class


SPF_NAMESPACE_BEGIN(Test)


   /*=====================================================================*/
BooleanTest::BooleanTest():
TestClass("System::Boolean test suite.\n")
{
	// Standard operators tests
	System::Delegate assignmentOperatorTest(*this, (System::MemberPointer0)&BooleanTest::AssignmentOperatorTest);
	UnitTesting::TestMethod assignmentOperatorTestMethod(assignmentOperatorTest, "AssignmentOperatorTest");
	this->Add(assignmentOperatorTestMethod);

	System::Delegate equalOperatorTest(*this, (System::MemberPointer0)&BooleanTest::EqualOperatorTest);
	UnitTesting::TestMethod equalOperatorTestMethod(equalOperatorTest, "EqualOperatorTest");
	this->Add(equalOperatorTestMethod);

	System::Delegate inequalOperatorTest(*this, (System::MemberPointer0)&BooleanTest::InequalOperatorTest);
	UnitTesting::TestMethod inequalOperatorTestMethod(equalOperatorTest, "InequalOperatorTest");
	this->Add(inequalOperatorTestMethod);

	System::Delegate notImplementedExceptionTest(*this, (System::MemberPointer0)&BooleanTest::NotImplementedExceptionTest);
	UnitTesting::TestMethod notImplementedExceptionTestMethod(notImplementedExceptionTest, "NotImplementedExceptionTest", typeid(System::NotImplementedException));
	this->Add(notImplementedExceptionTestMethod);
}


BooleanTest::~BooleanTest() {}


   /*=====================================================================*/
void BooleanTest::AssignmentOperatorTest() const
{
	System::Boolean b1;
	UnitTesting::Assert::IsFalse(b1);

	b1 = true;
	UnitTesting::Assert::IsTrue(b1);

	System::Boolean b2(false);
	UnitTesting::Assert::AreNotEqual(b1, b2);

	b2 = true;
	UnitTesting::Assert::AreEqual(b1, b2);

	System::Object* op1 = new System::Boolean();
	System::Object* op2 = new System::Boolean();
	UnitTesting::Assert::AreEqual(*op1, *op2);
	delete op1;
	delete op2;

	op1 = new System::Boolean(true);
	op2 = new System::Boolean(true);
	UnitTesting::Assert::AreEqual(*op1, *op2);
	delete op1;
	delete op2;
}


void BooleanTest::EqualOperatorTest() const
{
	System::Boolean b1;
	System::Boolean b2;

	UnitTesting::Assert::IsTrue(b1 == false);
	UnitTesting::Assert::IsTrue(false == b1);
	UnitTesting::Assert::IsTrue(b1 == b2);

	b1 = b2 = true;
	UnitTesting::Assert::IsTrue(b1 == true);
	UnitTesting::Assert::IsTrue(true == b1);
	UnitTesting::Assert::IsTrue(b1 == b2);
}


void BooleanTest::InequalOperatorTest() const
{
	System::Boolean b1;
	System::Boolean b2(true);

	UnitTesting::Assert::IsTrue(b1 != true);
	UnitTesting::Assert::IsTrue(true == b1);
	UnitTesting::Assert::IsTrue(b1 != b2);

	b1 = true;
	UnitTesting::Assert::IsFalse(b1 != true);
	UnitTesting::Assert::IsFalse(true != b1);
	UnitTesting::Assert::IsFalse(b1 != b2);
}


void BooleanTest::NotImplementedExceptionTest() const
{
	throw new System::NotImplementedException();
}


   /*=====================================================================*/
System::String BooleanTest::ToString() const
{
	return System::String("Test::BooleanTest");
}


SPF_NAMESPACE_END
