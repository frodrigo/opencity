/***************************************************************************
							treestructure.h  -  description
								-------------------
	begin                : july 23th, 2006
	copyright            : (C) 2006 by Duong-Khang NGUYEN
	email                : neoneurone @ users sourceforge net

	$Id: treestructure.h 3 2006-06-11 08:16:14Z neoneurone $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#ifndef _OPENCITY_TREESTRUCTURE_H_
#define _OPENCITY_TREESTRUCTURE_H_ 1

#include "structure.h"

/** This is a specialisation of structure for trees.
*/
class TreeStructure : public Structure  {
public:
	TreeStructure();

	TreeStructure( const OPENCITY_STRUCTURE_CODE & enumStructCode);

/* not used atm
	TreeStructure(
		const OPENCITY_STRUCTURE_CODE & enumStructCode,
		Structure * pMain );
*/


	virtual ~TreeStructure();  // virtual since 'Structure' is an abstract class


//========================================================================
/** Returns the GRAPHIC_CODE of the next level
*/
/* not used atm
	const OPENCITY_GRAPHIC_CODE
	GetNextLevelGraphicCode() const;
*/


//========================================================================
/** Returns the GRAPHIC_CODE of the previous level
*/
/* not used atm
	const OPENCITY_GRAPHIC_CODE
	GetPreviousLevelGraphicCode() const;
*/

/* not used atm
	const bool
	LevelUp();

	const bool
	LevelDown();
*/
};

#endif

































