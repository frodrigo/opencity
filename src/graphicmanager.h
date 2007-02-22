/***************************************************************************
						graphicmanager.h  -  description
							-------------------
	begin                : september 21th, 2003
	copyright            : (C) 2003-2007 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_GRAPHICMANAGER_H_
#define _OPENCITY_GRAPHICMANAGER_H_ 1

#include "main.h"


class Renderer;
class Structure;

class Model;
class Movement;
class Agent;


//========================================================================
/** It handles the rendering of 3d objects
\sa Renderer
*/
class GraphicManager {
public:
	GraphicManager();
	virtual ~GraphicManager();


//========================================================================
/** Get the ObjectId at the specified window's coordinates
	\return uint == the ObjectId previously displayed
*/
	const uint
	GetSelection(
		const uint & rcuiWinX,
		const uint & rcuiWinY ) const;


//========================================================================
/** Display a terrain square, there should be no structure on it. This
	method does take care about the height of the terrain.
*/
	void
	DisplayTerrain(
		const uint & rcuiW,
		const uint & rcuiL ) const;


//========================================================================
/** Display a the structure thanks to its graphic code.
	\param pcStructure points to the structure to display. ( != NULL )
	\param rcuiW,rcuiL the map (W,L) coordinates of the structure
*/
	void
	DisplayStructure(
		const Structure* pcStructure,
		const uint & rcuiW,
		const uint & rcuiL ) const;


//========================================================================
/** Display a graphic code at the specified map coordinates
	\param enumGC must be a valid graphic code
	\param rcuiW,rcuiL the map (W,L) coordinates of the graphism
*/
	void
	DisplayGC(
		const OPENCITY_GRAPHIC_CODE & enumGC,
		const uint & rcuiW,
		const uint & rcuiL ) const;


	void
	DisplayTerrainHighlight(
		const uint & rcuiW,
		const uint & rcuiL,
		const OPENCITY_TOOL_CODE & enumTool ) const;


	void
	DisplayStructureHighlight(
		const Structure* pcStructure,
		const uint & rcuiW,
		const uint & rcuiL,
		const OPENCITY_TOOL_CODE & enumTool ) const;


	void
	DisplayTerrainSelection(
		const uint & rcuiW,
		const uint & rcuiL,
		const uint & rcuiID ) const;


	void
	DisplayStructureSelection(
		const Structure* pcStructure,
		const uint & rcuiW,
		const uint & rcuiL,
		const uint & rcuiID ) const;


//========================================================================
/** This is a special function used for movement (vehicle) rendering
*/
	void
	Display(
		const OC_FLOAT & rcfW,
		const OC_FLOAT & rcfL,
		const OC_FLOAT & rcfH,
		const Movement* const pm ) const;


	void
	DisplayAgent(float x, float y, const Agent* const pAgent) const;


private:
	Model* tabpModel [OC_GRAPHIC_CODE_MAX];		///< A table of Model pointers

};

#endif










































