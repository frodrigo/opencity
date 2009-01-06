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

#ifndef _OPENCITY_FRAMEWORK_TEST_MAIN_H_
#define _OPENCITY_FRAMEWORK_TEST_MAIN_H_

// Shar Plus Framework headers
#include "System/CString.h"
#include "System/CConsole.h"


namespace Test
{
	class Car
	{
		public:
			Car(const System::String& name) :
				msName(name)
			{ System::Terminal << "Car " << msName << " ctor\n"; }


			~Car()
			{ System::Terminal << "Car " << msName << " dtor\n"; }


		private:
			System::String msName;
	};
}

#endif
