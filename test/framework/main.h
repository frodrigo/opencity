/***************************************************************************
                        main.h  -  description
							-------------------
	begin                : January 6th, 2009
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

#ifndef _SPF_TEST_MAIN_H_
#define _SPF_TEST_MAIN_H_

// Shar Plus Framework headers
#include "System/CObject.h"
#include "System/CConsole.h"
#include "System/CString.h"

#include "UnitTesting/ATestClass.h"
#include "UnitTesting/CTestResult.h"
#include "UnitTesting/AAssert.h"

namespace Test
{
	class Car
	{
		public:
			Car(const System::String& name) :
				msName(name)
			{ System::Terminal << "Car " << msName << " ctor\n"; }


			virtual ~Car()
			{ System::Terminal << "Car " << msName << " dtor\n"; }


		protected:
			System::String msName;
	};


	class CarObject : public System::Object, public Car
	{
		public:
			CarObject(const System::String& name) : Car(name)
			{}

			virtual ~CarObject()
			{}

			void PrintName()
			{
				System::Terminal << "CarObject name is: " << msName;
			}
	};


	class Calculator : public System::Object
	{

		public:
			Calculator():miRegister(0)
			{}

			const int GetRegister() const { return miRegister; }

			void Add(const int value) { miRegister += value; }

		private:
			int miRegister;
	};


	class CalculatorTestClass : public UnitTesting::TestClass
	{
		public:
			CalculatorTestClass()
			{
				this->Initialize();
			}


			void AddTest()
			{
				//System::Terminal << "AddTest called.\n";

				Calculator calculator;
				int expected = 3;
				int actual = 0;

				calculator.Add(expected);
				actual = calculator.GetRegister();
				UnitTesting::Assert::AreEqual(expected, actual);		// passed

				calculator.Add(expected);
				actual = calculator.GetRegister();
				//UnitTesting::Assert::AreEqual(expected, actual);		// failed
				UnitTesting::Assert::AreNotEqual(expected, actual);		// passed
			}


		private:
			void Initialize()
			{
				System::Delegate addTest(*this, (System::MemberPointer0)&CalculatorTestClass::AddTest);
				UnitTesting::TestMethod addMethodTest(UnitTesting::TestResult::Passed, addTest, "AddTest");
				this->Add(addMethodTest);

				addMethodTest.SetExpectedResult(UnitTesting::TestResult::Failed);
				this->Add(addMethodTest);
			}
	};
}

#endif
