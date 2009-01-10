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
#include "System/CSmartPointer.h"
#include "System/CBoolean.h"
#include "System/CType.h"
#include "System/CConsole.h"
#include "System/CException.h"

#include "System/Diagnostics/CDebug.h"

#include "System/Collections/Generic/CArray.h"
#include "System/Collections/Generic/CList.h"
#include "System/Collections/Generic/CLinkedList.h"


// System::SmartPointer test
int TestSmartPointer()
{
	System::Terminal << "System::SmartPointer test suite\n";

	System::SmartPointer<Test::Car> pCarA( new Test::Car(System::String("A")) );

	System::Terminal << "End of System::SmartPointer test suite\n";
	return 0;
}


// System::String test
int TestString()
{
	System::Terminal << "System::String test suite\n";

	System::String s1 = System::String("String assignment test\n");
	System::Terminal << s1;
	System::String sA = "String A ";
	System::String sB = System::String("String B\n");
	System::String sC = sA + sB;
	System::Terminal << sC;

	System::Terminal << "\n";
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


// System::Boolean test
int TestBoolean()
{
	System::Terminal << "System::Boolean test suite\n";

	System::Boolean b = System::Boolean(false);
	System::Terminal << b << "\n";
	b = true;
	System::Terminal << b << "\n";

	// Primitive type bool comparison
	if (b == false)
		System::Terminal << "b is false\n";
	else
		System::Terminal << "b is true\n";

	// Boolean type comparison
	System::Boolean a(false);
	if (a == b)
		System::Terminal << "a == b == " << a << "\n";
	if (a != b)
		System::Terminal << "a != b\n";

	a = true;
	if (a == b)
		System::Terminal << "a == b == " << a << "\n";
	if (a != b)
		System::Terminal << "a != b\n";


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


// Main test procedure
int main()
{
	// System::Console test
	System::Terminal << "OpenCity C++ framework test suite\n";
	System::Terminal << System::String("System::Console System::String output test\n");
	System::Terminal << "\n";

	// System::SmartPointer test
	TestSmartPointer();
	System::Terminal << "\n";

	// System::Boolean test
	TestBoolean();

	// System::String test
	TestString();

	// System::Exception test
	TestException();

	// System::Type test
	TestType();

	// System::Collections::Generic test
	TestCollectionsGeneric();

	// System::Diagnostics::Debug test
	System::Diagnostics::Debug::Assert(true);
	System::Diagnostics::Debug::Assert(false);
	System::Diagnostics::Debug::Assert(0 == 1);

}
