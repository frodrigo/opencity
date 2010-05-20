/***************************************************************************
                        CType.h  -  description
							-------------------
	begin                : January 1st, 2009
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

#ifndef _SPF_SYSTEM_CTYPE_H_
#define _SPF_SYSTEM_CTYPE_H_ 1

// Framework headers
#include "Reflection/CMemberInfo.h"		// System::Reflection::MemberInfo class

SPF_NAMESPACE_BEGIN(System)

/**
	Represents type declarations: class types, interface types, array types,
value types, enumeration types, type parameters, generic type definitions,
and open or closed constructed generic types.
*/
class Type : public Reflection::MemberInfo
{
	public:
		Type();
		Type(const NullValue& null);
		Type(const std::type_info& typeInfo);
		virtual ~Type();

		// Properties
		const String& GetNamespace() const;
		const String& GetFullName() const;

		virtual String ToString() const;

		// Operators
		virtual Type& operator=(const Type& type);
		virtual bool operator==(const Type& type) const;

	private:
		String msNamespace;
		String msFullName;
}; // class System::String

SPF_NAMESPACE_END

#endif
