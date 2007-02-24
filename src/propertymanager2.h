/***************************************************************************
					propertymanager2.h  -  description
							-------------------
	begin                : january 28th, 2007
	copyright            : (C) 2007 by Duong-Khang NGUYEN
	email                : neoneurone @ users sourceforge net

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

enum OPENCITY_DIRECTION;
enum OPENCITY_STRUCTURE_TYPE;
struct Property;				// Forward declaration


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


//========================================================================

private:
	Property* _aProperty;		///< The array which contains all object properties


//========================================================================
// Private methods
//========================================================================
	void _LoadProperties(
		uint index,
		string filename
	);


	const OPENCITY_DIRECTION
	_Str2Direction(const string& rcstrDir);


	const OPENCITY_STRUCTURE_TYPE
	_Str2Type(const string& rcstrType);
};

#endif
































