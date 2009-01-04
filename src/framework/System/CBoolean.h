/***************************************************************************
                        CBoolean.h  -  description
							-------------------
	begin                : January 4th, 2009
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

#ifndef _OPENCITY_FRAMEWORK_SYSTEM_CBOOLEAN_H_
#define _OPENCITY_FRAMEWORK_SYSTEM_CBOOLEAN_H_ 1

// Framework headers
#include "CObject.h"


SPF_NAMESPACE_BEGIN(System)

class Boolean : public Object
{
	public:
		Boolean();
		explicit Boolean(bool value);
		virtual ~Boolean();

		virtual String ToString() const;

		Boolean& operator=(const bool& value);
		bool operator==(const Boolean& value);
		bool operator!=(const Boolean& value);

		bool operator==(const bool& value);
		bool operator!=(const bool& value);


	private:
		bool mbValue;
}; // class System::Boolean

SPF_NAMESPACE_END

#endif
