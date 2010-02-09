/***************************************************************************
                        IDisposable.h  -  description
							-------------------
	begin                : February 9th, 2010
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

#ifndef _SPF_SYSTEM_IDISPOSABLE_H_
#define _SPF_SYSTEM_IDISPOSABLE_H_ 1


SPF_NAMESPACE_BEGIN(System)


/**
	Defines a method to release allocated resources.
*/
class IDisposable
{
	public:

/**
	Performs application-defined tasks associated with freeing, releasing,
or resetting unmanaged resources.
*/
		virtual void Dispose() = 0;

	protected:
		IDisposable() {}
		virtual ~IDisposable() {}

}; // class System::IDisposable

SPF_NAMESPACE_END

#endif
