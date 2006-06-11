/***************************************************************************
                          pathfinder.h  -  description
       $Id$
                             -------------------
    begin                : may 17th, 2004
    copyright            : (C) 2004-2006 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_PATHFINDER_H_
#define _OPENCITY_PATHFINDER_H_ 1

#include "main.h"

#include <vector>


class BuildingLayer;
class Map;

class Destination;

   /** This class implements few famous algorithms in pathfinding problems
   */
class PathFinder {
public:
	enum PATH_TYPE {
		OC_DISTANCE,
		OC_TRAFFIC
	};

	PathFinder(
		SDL_mutex* const mutex,
		BuildingLayer* const pblayer,
		Map* const map,
		const uint & rcuiCityWidth,
		const uint & rcuiCityHeight );

	~PathFinder();


	const bool
	findShortestPath(
		const uint & rcuiW1, const uint & rcuiH1,
		const uint & rcuiW2, const uint & rcuiH2,
		std::vector<Destination> & rvdest,
		const PATH_TYPE & enumType,
		uint uiMaxLength = 0xFFFFFFFF );


private:
	SDL_mutex* pmutex;
	BuildingLayer* pbuildlayer;
	Map* pmap;
	uint uiWidth;		// the city's width and height
	uint uiHeight;
};


#endif






























