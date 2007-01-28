/***************************************************************************
					propertymanager2.h  -  description
							-------------------
	begin                : january 28th, 2007
	copyright            : (C) 2007 by Duong-Khang NGUYEN
	email                : neoneurone @ users sourceforge net
	
	$Id: propertymanager.h 63 2006-10-17 20:45:12Z neoneurone $
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
	void _loadProperties(
		uint index,
		string filename
	);
};

#endif
































