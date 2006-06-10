/***************************************************************************
                          propertymanager.h  -  description
     $Id: propertymanager.h,v 1.14 2006/06/05 09:58:06 neoneurone Exp $
                             -------------------
    begin                : feb 06th, 2004
    copyright            : (C) 2003-2006 by Duong-Khang NGUYEN
    email                : neoneurone @ users sourceforge net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#ifndef _OPENCITY_PROPERTYMANAGER_H_
	#define _OPENCITY_PROPERTYMANAGER_H 1

#include "main.h"
#include "conf.h"				// For models' configuration files

#include <vector>

class Structure;


//========================================================================
/** This manager handles specially the models' .conf files. When an instance is
	created, it reads the "graphism.conf" file and load all the associated
	models' .conf files for later use. If a .conf file is not found for a specific
	model, the manager just silently ignores the problem. It's the programmer's
	responsability to not query that .conf, otherwise, the manager will crash.
*/
class PropertyManager
{
public:
	PropertyManager();
	~PropertyManager();


//========================================================================
/** Returns the requested property of the structure
	\param pCode The code of the requested property
	\param sCode The structure code
	\param pcStruct The pointer to the structure (optional)
	\return The value of the requested property
	\see enum.h for more details about the parameters
*/
	const int
	Get(
		const OPENCITY_PROPERTY_CODE & pCode,
		const OPENCITY_STRUCTURE_CODE & sCode,
		const Structure* pcStruct = NULL ) const;


//========================================================================
/** Return the _first_ level graphic code for the corresponding 
	structure code
	\param scode The structure code of the structure that you want to
		retrieve the graphic code.
	\return The corresponding graphic code
*/
	const OPENCITY_GRAPHIC_CODE
	GetGC(
		const OPENCITY_STRUCTURE_CODE scode ) const;


//========================================================================
/** Return the structure type from its structure code
*/
	const OPENCITY_STRUCTURE_TYPE
	GetST(
		const OPENCITY_STRUCTURE_CODE scode ) const;


//========================================================================
/** Return the structure's "width, length, and height" parameter
	\param gcode The graphic code of the structure that you want to retrieve
		the properties
	\param w Structure's width (OpenGL Ox)
	\param l Structure's length (OpenGL Oz)
	\param h Structure's height (OpenGL Oy)
	\param defw, defl, defh contain the default values for w, l and h respectively
	\return the value pointed by the given "key"
*/
	void
	GetWLH(
		const OPENCITY_GRAPHIC_CODE & gcode,
		uint & w, const uint defw,
		uint & l, const uint defl,
		uint & h, const uint defh);


private:
	std::vector<Conf*>	vpConf;			///< Models' configuration files




//========================================================================
/** Return the string which represents the value part of the (key-value)
	pair of the structure code given in parameter.
	\param gcode The OpenCity graphic code of the structure that you want to
				retrieve the parameter
	\param key The key of the element that you what to retrieve the value
	\param def Default value
	\return the value pointed by the given "key"
*/
	const string
	propertymanagerGetStr(
		const OPENCITY_GRAPHIC_CODE & gcode,
		const string & key,
		const string & def = "");


	const OPENCITY_ERR_CODE
	propertymanagerGetLint(
		const OPENCITY_GRAPHIC_CODE & gcode,
		const string & key,
		OC_LINT & rlint,
		const OC_LINT & def = 0);
};

#endif
































