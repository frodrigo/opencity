/***************************************************************************
						wegstructure.h  -  description
							-------------------
	begin                : june 17th, 2003
	copyright            : (C) 2003-2007 by Duong Khang NGUYEN
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

#ifndef _OPENCITY_WEGSTRUCTURE_H_
#define _OPENCITY_WEGSTRUCTURE_H_ 1

#include "structure.h"


//========================================================================
/**This is an specialization of 'Structure'. It regroups Water,
Electricity and Gas structures. All of them are governmental structures.
*/
class WEGStructure : public Structure  {
public:
	WEGStructure();
	WEGStructure( const OPENCITY_STRUCTURE_CODE & enumStructCode );
	WEGStructure(
		const OPENCITY_STRUCTURE_CODE & enumStructCode,
		Structure * pMain );

	virtual ~WEGStructure();  // virtual since Structure's one is
};

#endif































