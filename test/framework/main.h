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
			}

		private:
			void Initialize()
			{
				/*
				TestMethod(const TestResult& expectedResult, const System::Delegate& delegate);

				Calculator calculator;
				System::Delegate delegate(calculator,  );
				TestMethod method1 = TestMethod();

				Test::CarObject car("NumberOne");
				System::Delegate delegate(car, (System::MemberPointer0)&Test::CarObject::PrintName);
				delegate.DynamicInvoke();
				*/
			}
	};
}

#endif
