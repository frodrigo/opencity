/***************************************************************************
                          CArray.h  -  description
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

#ifndef _SPF_SYSTEM_COLLECTIONS_GENERIC_CARRAY_H_
#define _SPF_SYSTEM_COLLECTIONS_GENERIC_CARRAY_H_ 1

// Framework headers
#include "../../CObject.h"			// System::Object class

// boost::array template
#include <boost/array.hpp>


SPF_NAMESPACE_NESTED_BEGIN(System, Collections)
SPF_NAMESPACE_BEGIN(Generic)


/**
	Represents a strongly typed array of fixed length.
	\see List
*/
template<typename T, int size>
class Array : public Object
{
	public:
		Array();
		virtual ~Array();

		/**
			Gets the number of elements contained in the Array. It's always equal
		to the value returned by the GetLength() method.
			\see GetLength()
		*/
		int GetCount() const;

		/**
			Gets a 32-bit integer that represents the total number of elements in
		all the dimensions of the Array. It's always equal to the value returned
		by the GetCount() method.
			\see GetCount()
		*/
		const int GetLength() const;

		virtual String ToString() const;

		T& operator[](int index);
		const T& operator[](int index) const;

	protected:
		boost::array<T, size> maCollection;
}; // class System::Collections::Generic::Array


SPF_NAMESPACE_END
SPF_NAMESPACE_NESTED_END

#ifndef _GLIBCXX_EXPORT_TEMPLATE
	#include "CArray.tcc"
#endif

#endif
