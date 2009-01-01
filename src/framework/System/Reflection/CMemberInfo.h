/***************************************************************************
                        CMemberInfo.h  -  description
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

#ifndef _OPENCITY_FRAMEWORK_SYSTEM_REFLECTION_CMEMBERINFO_H_
#define _OPENCITY_FRAMEWORK_SYSTEM_REFLECTION_CMEMBERINFO_H_ 1

// Framework headers
#include "../CObject.h"			// System::Object class
#include "../CString.h"			// System::String class

namespace System
{
namespace Reflection
{
	class MemberInfo : public Object
	{
		public:
			MemberInfo();
			MemberInfo(const String& name);
			virtual ~MemberInfo();

			/**
				Gets the name of the current member.
				\return A System::String containing the name of this member.
			*/
			const String& GetName() const;

			virtual String ToString() const;

		protected:
			String msName;
	}; // class System::Reflection::MemberInfo
} // namespace System::Reflection
} // namespace System

#endif
