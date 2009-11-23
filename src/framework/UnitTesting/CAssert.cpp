/***************************************************************************
                        CAssert.cpp  -  description
							-------------------
	begin                : November 15th, 2009
	copyright            : (C) 2009 by Duong Khang NGUYEN
	email                : neoneurone @ gmail com

	$Id: CAssert.cpp 418 2009-05-02 09:59:12Z neoneurone $
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
#include "CAssert.h"				// UnitTesting::Assert class
#include "System/CConsole.h"		// System::Console class
#include "System/CString.h"			// System::String class
#include "System/CException.h"		// System::Exception class


SPF_NAMESPACE_BEGIN(UnitTesting)


   /*=====================================================================*/
Assert::Assert() {}


Assert::~Assert() {}


   /*=====================================================================*/
void Assert::AreEqual(const Object& expected, const Object& actual)
{
	if (expected == actual)
		return;

	// TODO: AssertFailedException
	throw System::Exception("The expected value is not equal to the actual value.");
}


   /*=====================================================================*/
void Assert::IsFalse(const bool condition)
{
	Assert::IsFalse(condition, "Assertion failed\n");
}


void Assert::IsFalse(const bool condition, const System::String& message)
{
	if (!condition)
		return;

	System::Terminal << message;
}


   /*=====================================================================*/
void Assert::IsTrue(const bool condition)
{
	Assert::IsTrue(condition, "Assertion failed\n");
}


void Assert::IsTrue(const bool condition, const System::String& message)
{
	if (condition)
		return;

	System::Terminal << message;
}


   /*=====================================================================*/
System::String Assert::ToString() const
{
	return System::String("UnitTesting::Assert");
}


SPF_NAMESPACE_END
