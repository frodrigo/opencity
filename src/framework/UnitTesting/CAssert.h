/***************************************************************************
                        CAssert.h  -  description
							-------------------
	begin                : November 15th, 2009
	copyright            : (C) 2009 by Duong Khang NGUYEN
	email                : neoneurone @ gmail com

	$Id: CAssert.h 424 2009-11-01 15:59:02Z neoneurone $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#ifndef _SPF_UNITTESTING_CASSERT_H_
#define _SPF_UNITTESTING_CASSERT_H_ 1

// Framework headers
#include "System/CObject.h"

SPF_NAMESPACE_BEGIN(UnitTesting)


/**
	Verifies conditions in unit tests using true/false propositions.
*/
class Assert : public System::Object
{
	public:
		Assert();
		virtual ~Assert();

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


		virtual System::String ToString() const;

}; // class UnitTesting::Assert

// namespace UnitTesting
SPF_NAMESPACE_END

#endif
