/***************************************************************************
                        IFormatProvider.h  -  description
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

#ifndef _SPF_SYSTEM_IFORMATPROVIDER_H_
#define _SPF_SYSTEM_IFORMATPROVIDER_H_ 1

SPF_NAMESPACE_BEGIN(System)

/**
	Provides a mechanism for retrieving an object to control formatting.
*/
class IFormatProvider
{
	public:

/**
	Returns an object that provides formatting services for the specified type.

	\param	formatType	An object that specifies the type of format object
to return.
	\return	An instance of the object specified by formatType, if the
IFormatProvider implementation can supply that type of object; otherwise,
null reference.
*/
		virtual Object GetFormat(const Type& formatType) const = 0;

	protected:
		IFormatProvider() {}
		virtual ~IFormatProvider() {}

}; // class System::IFormatProvider

SPF_NAMESPACE_END

#endif
