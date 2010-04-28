/***************************************************************************
                        ICloneable.h  -  description
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

#ifndef _SPF_SYSTEM_ICLONEABLE_H_
#define _SPF_SYSTEM_ICLONEABLE_H_ 1

SPF_NAMESPACE_BEGIN(System)

/**
	Supports cloning, which creates a new instance of a class with
the same value as an existing instance.
*/
class ICloneable
{
	public:

/**
	Creates a new object that is a copy of the current instance.
*/
		virtual Object Clone() = 0;

	protected:
		ICloneable() {}
		virtual ~ICloneable() {}

}; // class System::ICloneable

SPF_NAMESPACE_END

#endif
