/***************************************************************************
						globalvar.h    -  description
							-------------------
	begin                : october 17th, 2006
	copyright            : (C) 2006-2008 by Duong Khang NGUYEN
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

#ifndef _OPENCITY_GLOBALVAR_H_
#define _OPENCITY_GLOBALVAR_H_ 1

#ifdef OPENCITY_SDL_MIXER
	#include "audiomanagersdl.h"	// SDL_mixer audio support
#else
	#include "audiomanager.h"		// Dummy audio support
#endif

#include "main.h"					// Main defines
#include "extensionmanager.h"		// OpenGL extension manager
#include "graphicmanager.h"			// Graphic object manager
#include "map.h"					// Height map handling
#include "propertymanager.h"		// Structure properties manager
#include "renderer.h"				// Video renderer
#include "networking.h"				// Networking support
#include "movementmanager.h"		// Moving object manager
#include "kernel.h"					// MAS kernel
#include "environment.h"			// MAS environment
#include "pathfinder.h"				// Path finding algorithm
#include "mapgen/mapmaker.h"		// Map generator

// The current OpenCity version
#define OC_VERSION			0
#define OC_PATCHLEVEL		0
#define OC_SUBLEVEL			7


//========================================================================
/** This structure contain the global settings
	\author Duong Khang NGUYEN
*/
struct GlobalVar
{
// Config file and command line options
	bool gboolUseAudio;				///< Is audio enabled ?
	bool gboolAcceleratedVisual;	///< Is GL accelerated visual enabled ?
	bool gboolFullScreen;			///< Are we in fullscreen mode ?
	bool gboolServerMode;			///< Are we running in the server mode ?
	uint guiCityWidth;				///< The current city's width
	uint guiCityLength;				///< The current city's length
	uint guiMsPerFrame;				///< The number of milliseconds to wait between each frame.
	uint guiScreenWidth;			///< The current screen's width
	uint guiScreenHeight;			///< The current screen's height
	uint guiVideoBpp;				///< The current video bits-per-plane
	string gsOpenGLDriver;			///< The name of the OpenGL driver

	uint guiGeneratorSeed;
	string gsGeneratorHeightMap;
	MapGen::MapMaker::MAP_TYPE guiGeneratorMapType;
	MapGen::MapMaker::WATER_TYPE guiGeneratorWaterType;
	MapGen::MapMaker::MAP_SHAPE_TYPE guiGeneratorMapShapeType;
	MapGen::MapMaker::TREE_DENSITY_TYPE guiGeneratorTreeDensityType;

	OC_FLOAT gfMsSimDelayMax;
	string gsZenServer;

// Application status
	bool gboolActive;				///< Is the application active ?

// The mutex that all the simulators depend on
	SDL_mutex* gpmutexSim;

// The famous renderer
	Renderer* gpRenderer;

// Data managers
	AudioManager* gpAudioMgr;
	GraphicManager* gpGraphicMgr;
	ExtensionManager* gpExtensionMgr;
	PropertyManager* gpPropertyMgr;
	MapGen::MapMaker* gpMapMaker;	///< The MapMaker is used to create the height map
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
































