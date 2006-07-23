/***************************************************************************
                          rcistructure.h  -  description
      $Id: rcistructure.h 3 2006-06-11 08:16:14Z neoneurone $
                             -------------------
    begin                : june 17th, 2003
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

#ifndef _OPENCITY_RCISTRUCTURE_H_
#define _OPENCITY_RCISTRUCTURE_H_ 1


#include "structure.h"

/** This is a specialisation of structure. It regroups Residential,
Commercial and Industrial
*/
class RCIStructure : public Structure  {
public:
	RCIStructure();

	RCIStructure( const OPENCITY_STRUCTURE_CODE & enumStructCode);
	RCIStructure(
		const OPENCITY_STRUCTURE_CODE & enumStructCode,
		Structure * pMain );


	virtual ~RCIStructure();  // virtual since 'Structure' is an abstract class


//========================================================================
/** Returns the GRAPHIC_CODE of the next level
*/
	const OPENCITY_GRAPHIC_CODE
	GetNextLevelGraphicCode() const;


//========================================================================
/** Returns the GRAPHIC_CODE of the previous level
*/
	const OPENCITY_GRAPHIC_CODE
	GetPreviousLevelGraphicCode() const;


	const bool
	LevelUp();

	const bool
	LevelDown();
	
};

#endif

































