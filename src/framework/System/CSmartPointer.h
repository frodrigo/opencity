/***************************************************************************
                     CSmartPointer.h  -  description
							-------------------
	begin                : January 6th, 2009
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

#ifndef _OPENCITY_FRAMEWORK_SYSTEM_CSMARTPOINTER_H_
#define _OPENCITY_FRAMEWORK_SYSTEM_CSMARTPOINTER_H_ 1

// System::Object class
#include "CObject.h"

// boost::share_ptr template
#include <boost/shared_ptr.hpp>


SPF_NAMESPACE_BEGIN(System)

template<typename T>
class SmartPointer : public Object
{
	public:
		SmartPointer();
		SmartPointer(T* const pointer);
		virtual ~SmartPointer();

		virtual String ToString() const;

	private:
		boost::shared_ptr<T> mpObject;
}; // class System::SmartPointer

SPF_NAMESPACE_END


#ifndef _GLIBCXX_EXPORT_TEMPLATE
	#include "CSmartPointer.tcc"
#endif

#endif
