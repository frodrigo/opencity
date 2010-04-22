/***************************************************************************
                      CConsoleTestRunner.h  -  description
							-------------------
	begin                : April 22th, 2010
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

#ifndef _SPF_UNITTESTING_CCONSOLETESTRUNNER_H_
#define _SPF_UNITTESTING_CCONSOLETESTRUNNER_H_ 1

// Framework headers
#include "ATestRunner.h"			// UnitTesting::TestRunner abstract class


SPF_NAMESPACE_BEGIN(UnitTesting)

/**
	Represents a console test runner.
*/
class ConsoleTestRunner : public TestRunner
{
	public:
		ConsoleTestRunner();
		virtual ~ConsoleTestRunner();

/**
	Displays the test results to the console.
*/
		void DisplayResults();

		virtual System::String ToString() const;

}; // class UnitTesting::ConsoleTestRunner

// namespace UnitTesting
SPF_NAMESPACE_END

#endif
