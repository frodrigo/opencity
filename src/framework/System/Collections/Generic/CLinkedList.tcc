/***************************************************************************
                      CLinkedList.tcc  -  description
							-------------------
	begin                : January 4th, 2009
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

#ifndef _OPENCITY_FRAMEWORK_SYSTEM_COLLECTIONS_GENERIC_CLINKEDLIST_TCC_
#define _OPENCITY_FRAMEWORK_SYSTEM_COLLECTIONS_GENERIC_CLINKEDLIST_TCC_ 1

// Framework headers
#include "../../CString.h"			// System::String class


SPF_NAMESPACE_NESTED_BEGIN(System, Collections)
SPF_NAMESPACE_BEGIN(Generic)


   /*=====================================================================*/
template<typename T>
LinkedList<T>::LinkedList() {}


template<typename T>
LinkedList<T>::~LinkedList() {}


   /*=====================================================================*/
template<typename T>
void LinkedList<T>::AddLast(const T& item)
{
	mlCollection.push_back(item);
}


template<typename T>
int LinkedList<T>::GetCount() const
{
	// FIXME, overflow conversion
	return mlCollection.size();
}


   /*=====================================================================*/
template<typename T>
String LinkedList<T>::ToString() const
{
	return String("System::Collections::Generic::LinkedList");
}


SPF_NAMESPACE_END
SPF_NAMESPACE_NESTED_END

#endif
