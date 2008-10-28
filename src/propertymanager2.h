/***************************************************************************
					propertymanager2.h  -  description
							-------------------
	begin                : january 28th, 2007
	copyright            : (C) 2007-2008 by Duong Khang NGUYEN
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

#ifndef _OPENCITY_PROPERTYMANAGER2_H_
#define _OPENCITY_PROPERTYMANAGER2_H_ 1

#include "main.h"

#include <map>

#include "property.h"


//========================================================================
/** This manager handles the in-game object properties. Each object comes
with metadata in a separated XML file. It uses TinyXML and TinyXPath
to parse the those XML.
*/
class PropertyManager2
{
public:
	PropertyManager2();
	~PropertyManager2();


	const uint GetNumberProperty() const;


//========================================================================
/** Get the Property of the graphism file given as the key. If the
requested Property for the given key is not found then NULL is returned.
	\return The pointer to the requested Property of NULL if not found
	\remarks Use the indexed version instead for better performance
*/
	const Property* const
	Get(const string& key) const;


//========================================================================
/** Get the Property of the graphism file given as the key. If the
requested Property for the given key is not found then NULL is returned.
	\param index The index of the requested Property
	\return The pointer to the requested Property of NULL if not found
	\remarks There is not out of index checking ! Please call
GetNumberProperty() to determine the upper bound of the index
*/
	const Property* const
	Get(const uint index) const;


private:
	uint					_uiNumberProperty;	/// The total number of properties
	Property** 				_aProperty;			/// The property array
	std::map<string, uint>	_mapIndex;			/// The property index map


};

#endif
































