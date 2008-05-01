/***************************************************************************
						agentposition.h  -  description
							-------------------
	begin                : may 1st, 2008
	copyright            : (C) 2008 by Duong-Khang NGUYEN
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


#ifndef _OPENCITY_AGENTPOSITION_H_
#define _OPENCITY_AGENTPOSITION_H_ 1


//========================================================================
/** Utility class used to handle each agent's position
*/
class AgentPosition
{
public:
	int x, y;				///< 2D coordinates of the agent on the map
	unsigned long step;

	AgentPosition() : x(-1), y(-1), step(0) {}
	AgentPosition(int xx, int yy, unsigned long xstep) : x(xx), y(yy), step(xstep) {}
	AgentPosition(const AgentPosition &p) : x(p.x), y(p.y), step(p.step) {}
};

#endif
