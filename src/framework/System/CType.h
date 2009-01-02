/***************************************************************************
                        CType.h  -  description
							-------------------
	begin                : January 1st, 2000
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

#ifndef _OPENCITY_FRAMEWORK_SYSTEM_CTYPE_H_
#define _OPENCITY_FRAMEWORK_SYSTEM_CTYPE_H_ 1

// Framework headers
#include "Reflection/CMemberInfo.h"		// System::Reflection::MemberInfo class

SPF_NAMESPACE_BEGIN(System)

class Type : public Reflection::MemberInfo
{
	public:
		Type();
		Type(const String& name);
		Type(const String& name, const String& space);
		virtual ~Type();

		const String& GetNamespace() const;
		const String& GetFullName() const;

		virtual String ToString() const;

	private:
		String msNamespace;
		String msFullName;
}; // class System::String

SPF_NAMESPACE_END

#endif