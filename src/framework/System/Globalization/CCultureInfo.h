/***************************************************************************
                        CCultureInfo.h  -  description
							-------------------
	begin                : April 28th, 2010
	copyright            : (C) 2010 by Duong Khang NGUYEN
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

#ifndef _SPF_SYSTEM_GLOBALIZATION_CCULTUREINFO_H_
#define _SPF_SYSTEM_GLOBALIZATION_CCULTUREINFO_H_ 1

// Framework headers
#include "System/CObject.h"				// System::Object class
#include "System/ICloneable.h"			// System::ICloneable interface
#include "System/IFormatProvider.h"		// System::IFormatProvider interface

SPF_NAMESPACE_NESTED_BEGIN(System, Globalization)

/**
*/
class CultureInfo : public System::Object, public System::ICloneable, public System::IFormatProvider
{
	public:
		virtual String ToString() const;

	protected:
		CultureInfo();
		virtual ~CultureInfo();

}; // class System::Globalization::CultureInfo

// namespace System::Globalization
SPF_NAMESPACE_NESTED_END

#endif
