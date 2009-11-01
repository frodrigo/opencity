/***************************************************************************
                     CSmartPointer.tcc  -  description
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

#ifndef _SPF_SYSTEM_CSMARTPOINTER_TCC_
#define _SPF_SYSTEM_CSMARTPOINTER_TCC_ 1

// Framework headers
#include "CString.h"				// System::String class


SPF_NAMESPACE_BEGIN(System)


   /*=====================================================================*/
template<typename T>
SmartPointer<T>::SmartPointer() : mpObject( new T() ) {}


template<typename T>
SmartPointer<T>::SmartPointer(T* const p) : mpObject(p) {}


template<typename T>
SmartPointer<T>::~SmartPointer() {}


   /*=====================================================================*/
template<typename T>
String SmartPointer<T>::ToString() const
{
	return String("System::SmartPointer");
}


   /*=====================================================================*/


SPF_NAMESPACE_END

#endif
