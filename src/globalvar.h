/***************************************************************************
						GlobalVar.h    -  description
							-------------------
	begin                : oct 17th, 2006
	copyright            : (C) 2006 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_GLOBALVAR_H_
#define _OPENCITY_GLOBALVAR_ 1

#include "main.h"
#include "audiomanager.h"		// Audio support
#include "graphicmanager.h"
#include "map.h"
#include "propertymanager.h"
#include "renderer.h"
#include "networking.h"			// Networking support
#include "movementmanager.h"
#include "kernel.h"				// MAS kernel
#include "environment.h"		// MAS environment
#include "pathfinder.h"


//========================================================================
/** This structure contain the global settings
	\author Duong-Khang NGUYEN
*/
struct GlobalVar
{
// Config file and command line options
	bool gboolUseAudio;
	bool gboolFullScreen;
	bool gboolServerMode;
	uint guiCityWidth;
	uint guiCityLength;
	uint guiMsPerFrame;
	uint guiScreenWidth;
	uint guiScreenHeight;
	uint guiVideoBpp;

	OC_FLOAT gfMsSimDelayMax;
	string gsZenServer;

// The mutex that all the simulators depend on
	SDL_mutex* gpmutexSim;

// The famous renderer
	Renderer* gpRenderer;

// Datamanagers
	AudioManager* gpAudioMgr;
	GraphicManager* gpGraphicMgr;
	PropertyManager* gpPropertyMgr;
	Map* gpMapMgr;
	Networking* gpNetworking;
	PathFinder* gpPathFinder;
	MovementManager* gpMoveMgr;

// Multi-Agent System
	Kernel* gpKernel;
	Environment* gpEnvironment;

// The SDL video surface
	SDL_Surface* gpVideoSrf;
};


#endif































