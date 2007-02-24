/***************************************************************************
				opencity_structure_type.h  -  description
							-------------------
	begin                : february 22th, 2007
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

#ifndef _OPENCITY_STRUCTURE_TYPE_ENUM_H_
#define _OPENCITY_STRUCTURE_TYPE_ENUM_H_ 1


/** Each type of structure has several different structure code.
This is used for un/serialization and metadata loading
*/
enum OPENCITY_STRUCTURE_TYPE {
	OC_TYPE_UNDEFINED = 0,
	OC_TYPE_RESIDENCE,
	OC_TYPE_COMMERCE,
	OC_TYPE_INDUSTRY,
	OC_TYPE_WATER,
	OC_TYPE_ELECTRICITY,
	OC_TYPE_GAS,
	OC_TYPE_GOVERNMENT,
	OC_TYPE_PATH,
	OC_TYPE_TREE,
	OC_TYPE_VEHICLE
};


#endif
