/***************************************************************************
                          environment.h  -  description
         $Id: environment.h,v 1.9 2006/06/05 09:58:06 neoneurone Exp $
                             -------------------
    begin                : dec 11th 2005
    copyright            : (C) 2005 by Duong-Khang NGUYEN
    email                : neoneurone @ users sourceforge net
    author               : Victor STINNER
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/


#ifndef _OPENCITY_ENVIRONMENT_H_
#define _OPENCITY_ENVIRONMENT_H_ 1

#include <vector>
#include <map>
#include "agent.h"

#define MAX_PATH_LENGTH 20

class Destination;
class Structure;
class Layer;

   /*=====================================================================*/
unsigned char randomBool();
int randomInt(int min, int max);
direction_t rotate180(direction_t dir);
direction_t rotateLeft(direction_t dir);
direction_t rotateRight(direction_t dir);

class AgentPosition
{
public:    
    int x, y;
    unsigned long step;
    AgentPosition() : x(-1), y(-1), step(0) {}
    AgentPosition(int xx, int yy, unsigned long xstep) : x(xx), y(yy), step(xstep) {}
    AgentPosition(const AgentPosition &p) : x(p.x), y(p.y), step(p.step) {}
};



   /*=====================================================================*/
/** This is the environment of the Multi-Agent System.
	@author Victor STINNER
*/
class Environment
{
public:
//========================================================================
/** Create a new Environment instance with the specified width, and height.
	@note The specified width and height must be equal to the width and
the height of the specified Layer respectively
	@see BuildingLayer
*/
	Environment(unsigned int width, unsigned int height, Layer* pBL, Kernel *kernel);

    unsigned int getHeight() const;
    unsigned int getWidth() const;


//========================================================================
/** Display all the registered agents.
	@see GraphicManager
*/
	void displayAgent();


	void registerAgent(Agent* agent, int x, int y);
	void unregisterAgent(Agent* agent);

	Agent* getAgentAt(int x, int y);
	bool hasAgentAt(const Agent* agent, int x, int y) const;
	bool moveAgent(Agent* agent, int x, int y);


//========================================================================
/** Return the shortest path between 2 points. The length of the path
is limited to MAX_PATH_LENGTH
	@see PathFinder
*/
	const bool
	findShortestPath(
		unsigned int x1, unsigned int y1,
		unsigned int x2, unsigned int y2,
		std::vector<Destination> & rvdest );

	Structure* getBuildingXY(unsigned int x, unsigned int y);


   /*=====================================================================*/
   /*                         STATIC     METHODS                          */
   /*=====================================================================*/
//========================================================================
/** Calculate the square of the euclidian distance between 2 points
	@return The square of the euclidian distance
*/
	static unsigned int
	toSquareDistance(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);


private:
	unsigned int m_width;			///< Width of the map
	unsigned int m_height;			///< Height of the map
    Kernel *m_kernel;
	Layer* m_pBuildingLayer;
	std::map<Agent*, AgentPosition> m_last_pos;
	typedef std::map<Agent*, AgentPosition>::iterator m_last_pos_it;
	std::vector<Agent*> m_vector;
	typedef std::vector<Agent*>::iterator m_vector_it;
	typedef std::vector<Agent*>::const_iterator m_vector_cit;
};


#endif

































