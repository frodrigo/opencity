/***************************************************************************
						environment.h  -  description
							-------------------
	begin                : dec 11th 2005
	copyright            : (C) 2005 by Duong-Khang NGUYEN
	email                : neoneurone @ users sourceforge net
	author               : Victor STINNER

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


#include "environment.h" 
#include "structure.h"
#include "layer.h"
#include "kernel.h"
#include "graphicmanager.h"
#include "pathfinder.h"


extern GraphicManager* gpGraphicMgr;	// global graphic manager
extern PathFinder* gpPathFinder;		// global PathFinder class


/*=====================================================================*/
unsigned char randomBool()
{
	return rand() <= (RAND_MAX / 2);
}


/*=====================================================================*/
int randomInt(int min, int max)
{
	unsigned int width = (max - min + 1);
	return static_cast<int> (min + (double)rand() * width / RAND_MAX);
}


	/*=====================================================================*/
direction_t rotate180(direction_t dir)
{
	switch (dir)
	{
		case NORTH: return SOUTH; 
		case EAST:  return WEST;
		case SOUTH: return NORTH;
		case WEST:  return EAST;
	}

// We should never reach here
	assert(0);
	return NORTH;
}


	/*=====================================================================*/
direction_t rotateLeft(direction_t dir)
{
	switch (dir)
	{
		case NORTH: return WEST; 
		case EAST:  return NORTH;
		case SOUTH: return EAST;
		case WEST:  return SOUTH;
	}

// We should never reach here
	assert(0);
	return NORTH;
}


	/*=====================================================================*/
direction_t rotateRight(direction_t dir)
{
	switch (dir)
	{
		case NORTH: return EAST; 
		case EAST:  return SOUTH;
		case SOUTH: return WEST;
		case WEST:  return NORTH;
	}

// We should never reach here
	assert(0);
	return NORTH;
}

	/*=====================================================================*/
Environment::Environment(
	unsigned int width,
	unsigned int height,
	Layer* pBL,
	Kernel *kernel):
m_width(width),
m_height(height),
m_kernel(kernel),
m_pBuildingLayer(pBL)
{
	m_vector.assign(m_width * m_height, NULL);
}


/*=====================================================================*/
void Environment::displayAgent()
{
	assert( gpGraphicMgr != NULL );

	m_vector_cit it=m_vector.begin(), end=m_vector.end();
	for (; it != end; ++it) {
		if (*it) {
			Agent *agent = *it;
			float x = agent->getX();
			float y = agent->getY();
			m_last_pos_it last = m_last_pos.find(agent);
			if (last != m_last_pos.end()) {
				unsigned int dt = m_kernel->getStep() - last->second.step;
				if (dt  < 3) {
					int lastx = last->second.x;
					int lasty = last->second.y;
					x = x + (x - lastx) * dt / 3;
					y = y + (y - lasty) * dt / 3;
				}
				else {
					m_last_pos.erase(last);
				}
			}
			gpGraphicMgr->DisplayAgent(x, y, agent);
// debug
//			cout << "Registered agent with GC : " << (*it)->GetGraphicCode() << endl;
		}
	}
}


/*=====================================================================*/
unsigned int Environment::getHeight() const
{
	return m_width;
}


/*=====================================================================*/
unsigned int Environment::getWidth() const
{
	return m_height;
}


/*=====================================================================*/
void Environment::registerAgent(Agent* agent, int x, int y)
{
	unsigned int pos = agent->getY()*m_width + agent->getX();
	if (m_vector[pos] != NULL) {
		cerr << "There's already one agent at x: " << x << " / y: " << y << endl;
		return;
	}
	m_vector[pos] = agent;
}


/*=====================================================================*/
void Environment::unregisterAgent(Agent* agent)
{
	unsigned int pos = agent->getY()*m_width + agent->getX();
	assert(m_vector[pos] != NULL);
	m_vector[pos] = NULL;
}


/*=====================================================================*/
bool Environment::hasAgentAt(const Agent *agent, int x, int y) const
{
	return (m_vector.at(y*m_width + x) != NULL);
}


/*=====================================================================*/
Agent* Environment::getAgentAt(int x, int y)
{
	if (x < 0 || (int)m_width <= x) return NULL;
	if (y < 0 || (int)m_height <= y) return NULL;
	return m_vector.at(y*m_width + x);
}


/*=====================================================================*/
bool Environment::moveAgent(Agent* agent, int x, int y)
{
	unsigned int pos = y*m_width + x;
	unsigned int oldpos = agent->getY()*m_width + agent->getX();
	if (m_vector[pos] != NULL) return false;
	m_last_pos[agent] = AgentPosition(agent->getX(),agent->getY(),m_kernel->getStep());
	m_vector[pos] = agent;
	m_vector[oldpos] = NULL;
	return true;
}


/*=====================================================================*/
const bool
Environment::findShortestPath(
	unsigned int x1, unsigned int y1,
	unsigned int x2, unsigned int y2,
	std::vector<Destination> & rvdest )
{
	assert(gpPathFinder != NULL);

	return gpPathFinder->findShortestPath(
		x1, y1, x2, y2, rvdest,
		PathFinder::OC_DISTANCE, MAX_PATH_LENGTH  );
}


/*=====================================================================*/
Structure*
Environment::getBuildingXY(
	unsigned int x,
	unsigned int y)
{
	return m_pBuildingLayer->GetStructure( x, y );
}


/*=====================================================================*/
/*                         STATIC     METHODS                          */
/*=====================================================================*/
unsigned int
Environment::toSquareDistance(
	unsigned int x1, unsigned int y1,
	unsigned int x2, unsigned int y2)
{
	return ((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}



























