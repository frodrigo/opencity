/***************************************************************************
                        CAssert.cpp  -  description
							-------------------
	begin                : November 15th, 2009
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

// Framework headers
#include "CAssert.h"				// UnitTesting::Assert class
#include "CAssertFailedException.h"	// UnitTesting::AssertFailedException class
#include "System/CConsole.h"		// System::Console class
#include "System/CString.h"			// System::String class


SPF_NAMESPACE_BEGIN(UnitTesting)


   /*=====================================================================*/
Assert::Assert() {}


Assert::~Assert() {}


   /*=====================================================================*/
void Assert::AreEqual(const Object& expected, const Object& actual)
{
	Assert::AreEqual(expected, actual, "The expected value is not equal to the actual value.");
}


void Assert::AreEqual(const Object& expected, const Object& actual, const System::String& message)
{
	if (expected == actual)
		return;

	throw AssertFailedException(message);
}


   /*=====================================================================*/
void Assert::AreSame(const Object& expected, const Object& actual)
{
	Assert::AreSame(expected, actual, "The expected object does not refer to the same object as the actual object.");
}


void Assert::AreSame(const Object& expected, const Object& actual, const System::String& message)
{
	if (&expected == &actual)
		return;

	throw AssertFailedException(message);
}


   /*=====================================================================*/
void Assert::IsFalse(const bool condition)
{
	Assert::IsFalse(condition, "The expected false condition is evaluated to true.");
}


void Assert::IsFalse(const bool condition, const System::String& message)
{
	if (!condition)
		return;

	throw AssertFailedException(message);
}


   /*=====================================================================*/
void Assert::IsTrue(const bool condition)
{
	Assert::IsTrue(condition, "The expected true condition is evaluated to false.");
}


void Assert::IsTrue(const bool condition, const System::String& message)
{
	if (condition)
		return;

	throw AssertFailedException(message);
}


   /*=====================================================================*/
System::String Assert::ToString() const
{
	return System::String("UnitTesting::Assert");
}


SPF_NAMESPACE_END
