/***************************************************************************
                       CNullValue.h  -  description
							-------------------
	begin                : January 18th, 2009
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

#ifndef _SPF_SYSTEM_CNULLVALUE_H_
#define _SPF_SYSTEM_CNULLVALUE_H_ 1

// Framework headers
#include "AValueType.h"


SPF_NAMESPACE_BEGIN(System)

/**
	Represents a NullValue object. In C++, a reference can not be assigned
a null value. In this Sharp Plus Framework, the C# null reference is emulated
by the System::NullValue class.
	To assign a null value to an object, use the System::Null constant.
*/
class NullValue : public ValueType
{
	public:
		NullValue();
		NullValue(const NullValue& value);
		virtual ~NullValue();

		bool IsNull() const;

		virtual String ToString() const;

}; // class System::NullValue

#ifndef _SPF_SYSTEM_CNULLVALUE_CPP_
	extern const NullValue Null;
#endif

SPF_NAMESPACE_END

#endif
