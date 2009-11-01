/***************************************************************************
                    CStringSplitOptions.h  -  description
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

#ifndef _SPF_SYSTEM_CSTRINGSPLITOPTIONS_H_
#define _SPF_SYSTEM_CSTRINGSPLITOPTIONS_H_ 1

// Framework headers
#include "CEnum.h"			// System::Enum class

SPF_NAMESPACE_BEGIN(System)

class StringSplitOptions : public Enum
{
	public:
		enum
		{
			None				= 0x0000,
			RemoveEmptyEntries	= 0x0001
		};

		StringSplitOptions();
		virtual ~StringSplitOptions();

		virtual String ToString() const;
}; // class System::StringSplitOptions

SPF_NAMESPACE_END

#endif
