/***************************************************************************
                        CArray.tcc  -  description
							-------------------
	begin                : January 10th, 2009
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

#ifndef _OPENCITY_FRAMEWORK_SYSTEM_COLLECTIONS_GENERIC_CARRAY_TCC_
#define _OPENCITY_FRAMEWORK_SYSTEM_COLLECTIONS_GENERIC_CARRAY_TCC_ 1

// Framework headers
#include "../../CString.h"		// System::String class


SPF_NAMESPACE_NESTED_BEGIN(System, Collections)
SPF_NAMESPACE_BEGIN(Generic)


   /*=====================================================================*/
template<typename T, int size>
Array<T, size>::Array() {}


template<typename T, int size>
Array<T, size>::~Array() {}


   /*=====================================================================*/
template<typename T, int size>
int Array<T, size>::GetCount() const
{
	// FIXME, overflow conversion
	return maCollection.size();
}


template<typename T, int size>
const int Array<T, size>::GetLength() const
{
	// FIXME, overflow conversion
	return maCollection.size();
}


   /*=====================================================================*/
template<typename T, int size>
String Array<T, size>::ToString() const
{
	return String("System::Collections::Generic::Array");
}


   /*=====================================================================*/
template<typename T, int size>
T& Array<T, size>::operator[](int index)
{
	return maCollection[index];
}


template<typename T, int size>
const T& Array<T, size>::operator[](int index) const
{
	return maCollection[index];
}


   /*=====================================================================*/


SPF_NAMESPACE_END
SPF_NAMESPACE_NESTED_END

#endif
