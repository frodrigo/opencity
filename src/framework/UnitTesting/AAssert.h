/***************************************************************************
                        AAssert.h  -  description
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

#ifndef _SPF_UNITTESTING_AASSERT_H_
#define _SPF_UNITTESTING_AASSERT_H_ 1

// Framework headers
#include "System/CObject.h"

SPF_NAMESPACE_BEGIN(UnitTesting)


/**
	Verifies conditions in unit tests using true/false propositions.
*/
class Assert : public System::Object
{
	public:

/**
	Verifies that two specified integers are equal. The assertion fails if the
integers are not equal.

	\param	expected	The first integer to compare. This is the integer the
unit test expects.
	\param	actual		The second integer to compare. This is the integer the
unit test produced.
*/
		static void AreEqual(const int expected, const int actual);

/**
	Verifies that two specified integers are equal. The assertion fails if the
integers are not equal.

	\param	expected	The first integer to compare. This is the integer the
unit test expects.
	\param	actual		The second integer to compare. This is the integer the
unit test produced.
	\param	message		A message to display if the assertion fails. This
message can be seen in the unit test results.
*/
		static void AreEqual(const int expected, const int actual, const System::String& message);

/**
	Verifies that two specified objects are equal. The assertion fails if the
objects are not equal.

	\param	expected	The first object to compare. This is the object the
unit test expects.
	\param	actual		The second object to compare. This is the object the
unit test produced.
	\see	Object
*/
		static void AreEqual(const Object& expected, const Object& actual);

/**
	Verifies that two specified objects are equal. The assertion fails if the
objects are not equal.

	\param	expected	The first object to compare. This is the object the
unit test expects.
	\param	actual		The second object to compare. This is the object the
unit test produced.
	\param	message		A message to display if the assertion fails. This
message can be seen in the unit test results.
	\see	Object
*/
		static void AreEqual(
			const Object& expected, const Object& actual,
			const System::String& message);

		static void AreNotEqual(const Object& expected, const Object& actual);
		static void AreNotEqual(const Object& expected, const Object& actual, const System::String& message);

		static void AreNotEqual(const int expected, const int actual);
		static void AreNotEqual(const int expected, const int actual, const System::String& message);


/**
	Verifies that two specified object variables refer to the same object. The
assertion fails if they refer to different objects.

	\param	expected	The first object to compare. This is the object the
unit test expects.
	\param	actual		The second object to compare. This is the object the
unit test produced.
*/
		static void AreSame(const Object& expected, const Object& actual);

/**
	Verifies that two specified object variables refer to the same object. The
assertion fails if they refer to different objects. Displays a message if the
assertion fails.

	\param	expected	The first object to compare. This is the object the
unit test expects.
	\param	actual		The second object to compare. This is the object the
unit test produced.
	\param	message		A message to display if the assertion fails. This
message can be seen in the unit test results.
*/
		static void AreSame(
			const Object& expected, const Object& actual,
			const System::String& message);

/**
	Verifies that the specified condition is false. The assertion fails if the
condition is true.
*/
		static void IsFalse(const bool condition);

/**
	Verifies that the specified condition is false. The assertion fails if the
condition is true. Displays a message if the assertion fails.
*/
		static void IsFalse(const bool condition, const System::String& message);

/**
	Verifies that the specified condition is true. The assertion fails if the
condition is false.
*/
		static void IsTrue(const bool condition);

/**
	Verifies that the specified condition is true. The assertion fails if the
condition is false. Displays a message if the assertion fails.
*/
		static void IsTrue(const bool condition, const System::String& message);


}; // class UnitTesting::Assert

// namespace UnitTesting
SPF_NAMESPACE_END

#endif
