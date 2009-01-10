/***************************************************************************
                          CList.h  -  description
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

#ifndef _OPENCITY_FRAMEWORK_SYSTEM_COLLECTIONS_GENERIC_CLIST_H_
#define _OPENCITY_FRAMEWORK_SYSTEM_COLLECTIONS_GENERIC_CLIST_H_ 1

// Framework headers
#include "../../CObject.h"			// System::Object class

// Standard C++ headers
#include <vector>


SPF_NAMESPACE_NESTED_BEGIN(System, Collections)
SPF_NAMESPACE_BEGIN(Generic)


/**
	Represents a strongly typed list of items that can be accessed by index.
Provides methods to search, sort, and manipulate lists.
	\see Array
*/
template<typename T>
class List : public Object
{
	public:
		List();
		virtual ~List();

		/**
			Adds an object to the end of the List.
			\param T The object to be added to the end of the List.
		*/
		void Add(const T& item);

		int GetCount() const;

		virtual String ToString() const;

		T& operator[](int index);
		const T& operator[](int index) const;

	protected:
		std::vector<T> mvCollection;
}; // class System::String


SPF_NAMESPACE_END
SPF_NAMESPACE_NESTED_END

#ifndef _GLIBCXX_EXPORT_TEMPLATE
	#include "CList.tcc"
#endif

#endif
