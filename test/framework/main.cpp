/***************************************************************************
                        main.cpp  -  description
							-------------------
	begin                : December 31th, 2008
	copyright            : (C) 2008 by Duong Khang NGUYEN
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

// Test header
#include "main.h"

// Sharp Plus Framework header
#include "System/CBoolean.h"
#include "System/CConsole.h"
#include "System/CException.h"
#include "System/CNullValue.h"
#include "System/CSmartPointer.h"
#include "System/CType.h"

#include "System/ADelegate.h"

#include "System/Diagnostics/CDebug.h"

#include "System/Collections/Generic/CArray.h"
#include "System/Collections/Generic/CList.h"
#include "System/Collections/Generic/CLinkedList.h"

#include "System/Information/Software/CSdl.h"

#include "UnitTesting/CConsoleTestRunner.h"
#include "System/CBooleanTest.h"
#include "System/CStringTest.h"

#include "System/CNotImplementedException.h"

// Test GNU gettext
#include <libintl.h>

int TestGnuGettext()
{
	#define PACKAGE "opencity"
	#define LOCALEDIR "/usr/share/locale"

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	return 0;
}

// System::Object test
int TestObject()
{
	System::Terminal << "System::Object test suite\n";
	System::Terminal << "There should be 2 failed assertions:\n";

	System::Object object, otherObject;
	System::Diagnostics::Debug::Assert(object == object);
	System::Diagnostics::Debug::Assert(object.Equals(object));
	System::Diagnostics::Debug::Assert(!object.Equals(otherObject));
	System::Diagnostics::Debug::Assert(!otherObject.Equals(object));
	System::Diagnostics::Debug::Assert(!(object == object));
	System::Diagnostics::Debug::Assert(!object.Equals(object));

	System::Terminal << "\n";
	return 0;
}


// System::Delegate test
int TestDelegate()
{
	System::Terminal << "System::Delegate test suite\n";

	Test::CarObject car("NumberOne");
	System::Delegate delegate(car, (System::MemberPointer0)&Test::CarObject::PrintName);
	delegate.DynamicInvoke();

	System::Terminal << "\n";
	return 0;
}


// System::SmartPointer test
int TestSmartPointer()
{
	System::Terminal << "System::SmartPointer test suite\n";

	System::SmartPointer<Test::Car> pCarA( new Test::Car(System::String("A")) );

	System::Terminal << "End of System::SmartPointer test suite\n";
	return 0;
}


// System::Exception test
int TestException()
{
	System::Terminal << "System::Exception test suite\n";

	try
	{
		throw System::Exception("System::Exception test\n");
	}
	catch (System::Exception& ex)
	{
		System::Terminal << ex;
		System::String s = ex.GetMessage();
		System::Terminal << s;
		System::Terminal << ex.GetMessage();
		System::Terminal << (System::String)ex.GetMessage();
	}

	System::Terminal << "\n";
	return 0;
}


// System::Type test
int TestType()
{
	System::Terminal << "System::Type test suite\n";

	System::Object object;
	System::Type t = object.GetType();
	System::Terminal << t.GetName() << "\n";
	System::Terminal << t.GetType().GetName() << "\n";

	System::Terminal << System::Terminal.GetType().GetName() << "\n";

	System::Boolean b = System::Boolean(false);
	System::Terminal << b.GetType().GetName() << "\n";

	System::String s = System::String("String object");
	System::Terminal << s.GetType().GetName() << "\n";

	System::Exception ex;
	System::Terminal << ex.GetType().GetName() << "\n";

	System::Object* pObj = new System::NotImplementedException();
	System::Terminal << pObj->GetType().GetName() << "\n";
	delete pObj;

	System::Diagnostics::Debug debug;
	System::Terminal << debug.GetType().GetName() << "\n";

	System::Terminal << "\n";
	return 0;
}


// System::Collections::Generic test
int TestCollectionsGeneric()
{
	System::Terminal << "System::Collections::Generic test suite\n";

	System::Collections::Generic::Array<System::Object, 2> aObject;
	System::Object objA, objB;
	aObject[0] = objA;
	aObject[1] = objB;
	// aObject[2] = objB;	// out of range assertion failed
	System::Terminal << aObject.ToString() << " has " << aObject.GetLength() << " objects\n";

	System::Collections::Generic::LinkedList<System::Object> llObject;
	llObject.AddLast(objA);
	llObject.AddLast(objB);
	System::Terminal << llObject.ToString() << " has " << llObject.GetCount() << " objects\n";

	System::Collections::Generic::List<System::Object> lObject;
	lObject.Add(objA);
	lObject.Add(objB);
	System::Terminal << lObject.ToString() << " has " << lObject.GetCount() << " objects\n";

	System::Terminal << "\n";
	return 0;
}


int TestNullValue()
{
	System::Terminal << "System::NullValue test suite\n";

	System::Object objA;
	System::Terminal << "Before null assignment, is null : " << objA.IsNull() << "\n";

	objA = System::Null;
	System::Terminal << "After null assignment, is null : " << objA.IsNull() << "\n";

	System::Boolean boolA;
	boolA = System::Null;
	System::Diagnostics::Debug::Assert( boolA.IsNull(), "Null bool assertion failed.\n" );

	System::String strA;
	strA = System::Null;
	System::Diagnostics::Debug::Assert( strA.IsNull(), "Null string assertion failed.\n" );

	System::Terminal << "\n";
	return 0;
}


int TestSystemInformationSoftware()
{
	System::Terminal << "System::Information::Software::Sdl test suite\n";

	System::Terminal << "Compile-time version: " << System::Information::Software::Sdl::GetCompiletimeVersion() << "\n";
	System::Terminal << "Run-time version: " << System::Information::Software::Sdl::GetRuntimeVersion() << "\n";

	System::Terminal << "\n";
	return 0;
}


int TestTestRunner()
{
	System::Terminal << "UnitTesting::TestRunner test suite\n";

	const UnitTesting::TestClass& testClass = Test::CalculatorTestClass();
	UnitTesting::ConsoleTestRunner runner;
	runner.Add(testClass);

	Test::BooleanTest booleanTest;
	runner.Add(booleanTest);

	Test::StringTest stringTest;
	runner.Add(stringTest);

	runner.Run();
	runner.DisplayResults();

	System::Terminal << "\n";
	return 0;
}


// Main test procedure
int main()
{
	// Test GNU Gettext
	TestGnuGettext();

	// System::Console test
	System::Terminal << "SharpPlus framework test suite - begin\n";
	System::Terminal << System::String("System::Console System::String output test\n");
	System::Terminal << "\n";

	// System::SmartPointer test
	TestObject();

	// System::Delegate test
	TestDelegate();

	// System::SmartPointer test
	TestSmartPointer();
	System::Terminal << "\n";

	// System::Exception test
	TestException();

	// System::Type test
	TestType();

	// System::NullValue test
	TestNullValue();

	// System::Collections::Generic test
	TestCollectionsGeneric();

	// System::Information::Software test
	TestSystemInformationSoftware();

	// UnitTesting::TestRunner
	TestTestRunner();

	// System::Diagnostics::Debug test
	System::Terminal << "There should be 2 failed assertions preceding the ending message:\n";
	System::Diagnostics::Debug::Assert(true);
	System::Diagnostics::Debug::Assert(false);
	System::Diagnostics::Debug::Assert(0 == 1);

	System::Terminal << "SharpPlus framework test suite - end\n";
	return 0;
}
