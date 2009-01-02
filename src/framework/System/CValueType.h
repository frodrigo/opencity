/***************************************************************************
                        CValueType.h  -  description
							-------------------
	begin                : January 2nd, 2009
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

#ifndef _OPENCITY_FRAMEWORK_SYSTEM_CVALUETYPE_H_
#define _OPENCITY_FRAMEWORK_SYSTEM_CVALUETYPE_H_ 1

// Framework headers
#include "CObject.h"			// System::Object class

SPF_NAMESPACE_BEGIN(System)

class ValueType : public Object
{
	public:
		ValueType();
		virtual ~ValueType();

		virtual String ToString() const;
}; // class System::ValueType

SPF_NAMESPACE_END

#endif
