/***************************************************************************
                          CLinkedList.h  -  description
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

#ifndef _OPENCITY_FRAMEWORK_SYSTEM_COLLECTIONS_GENERIC_CLINKEDLIST_H_
#define _OPENCITY_FRAMEWORK_SYSTEM_COLLECTIONS_GENERIC_CLINKEDLIST_H_ 1

// Framework headers
#include "../../CObject.h"			// System::Object class

// Standard C++ headers
#include <list>


SPF_NAMESPACE_NESTED_BEGIN(System, Collections)
SPF_NAMESPACE_BEGIN(Generic)


template<typename T>
class LinkedList : public Object
{
	public:
		LinkedList();
		virtual ~LinkedList();

		/**
			Adds an object to the end of the LinkedList.
			\param T The object to be added to the end of the LinkedList.
		*/
		void AddLast(const T& item);

		int GetCount() const;

		virtual String ToString() const;

	protected:
		std::list<T> mlCollection;
}; // class System::String


SPF_NAMESPACE_END
SPF_NAMESPACE_NESTED_END

#ifndef _GLIBCXX_EXPORT_TEMPLATE
	#include "CLinkedList.tcc"
#endif

#endif
