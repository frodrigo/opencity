/***************************************************************************
						main.cpp  -  description
							-------------------
	project              : OpenCity
	codename             : Delphine
	begin                : may 28th, 2003
	copyright            : (C) 2003-2008 by Duong Khang NGUYEN
	email                : neoneurone @ gmail com

	details              :
			This is a game project which targets the ultimate city
		simulation just like like xxxCity or something like that :)

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

//========================================================================
/**
	\mainpage OpenCity source code documentation
	\section intro Introduction
		OpenCity is another 3D city simulator. It's certainly not the best
	city simulation nor the best eye candy game out there but we love it
	because it's our city simulation and we can do anything we want with
	it.

	\section reuse Code reuse
		If you are reading this page, we believe that you are interested
	in the tricks behind the scene. And we think that you are happy to know
	that our city simulation is made with portability and object oriented
	programming technics in mind. It means that you can reuse few parts of
	OpenCity source code. At the moment, you can reuse the classes
	AudioManager, Conf, ModelLoader, and MapGen::MapMaker without major
	modifications.

	\author Duong Khang NGUYEN &lt;neoneurone @ gmail com&gt;
	\author Frédéric RODRIGO
*/

// Useful enumerations
#include "opencity_direction.h"
#include "opencity_structure_type.h"

// OpenCity headers
#include "main.h"
#include "city.h"				// The project core
#include "conf.h"				// Parser for .conf file
#include "agentpolice.h"		// MAS testing
#include "agentdemonstrator.h"

// Global settings
#include "globalvar.h"
extern GlobalVar gVars;

// Libraries headers
#include "SDL_image.h"
#include "binreloc.h"			// BinReloc routines from AutoPackage
#include "tinyxml.h"
#include "SimpleOpt.h"			// Simple command line argument parser

// Standard headers
#include <cmath>				// For log10
#include <cstdlib>				// For getenv
#include <sstream>
#include <ctime>				// For time

// Test XPath
#include "property.h"
#include "propertymanager2.h"


   /*=====================================================================*/
   /*                           LOCAL     MACROS                          */
   /*=====================================================================*/
#ifndef __WIN32__
	#include <sys/stat.h>		// mkdir
#else
// Win32 specifics
	#include <shlobj.h>			// Windows shell technologies
	#define DATADIR "C:/Program Files"
	#define SYSCONFDIR DATADIR
#endif

// Window's settings
	#define OC_WINDOW_POS_X			20
	#define OC_WINDOW_POS_Y			20
	#define OC_WINDOW_WIDTH			750
	#define OC_WINDOW_HEIGHT		560
	#define OC_WINDOW_BPP_DEFAULT	0			///< Use the current video bpp
	#define OC_FULLSCREEN_WIDTH		1024
	#define OC_FULLSCREEN_HEIGHT	768

// Exit code. This is not an exhaustive list.
	#define OC_ERROR_NOT_FOUND			-1		///< Config files not found
	#define OC_ERROR_PARSE_CONFIG		-2		///< Config parsing error
	#define OC_ERROR_ARGUMENT			-3		///< Command line argument error

	#define OC_ERROR_MEMORY				-10		///< Out of memory

	#define OC_ERROR_SDL_INIT			-20		///< SDL initialization failed
	#define OC_ERROR_SDL_DOUBLEBUFFER	-21		///< Video double buffer unsupported
	#define OC_ERROR_SDL_ACCELERATED	-22		///< Accelerated display unsupported
	#define OC_ERROR_SDL_VIDEORESIZE	-23		///< Unable to resize the window
	#define OC_ERROR_SDL_FULLSCREEN		-24		///< Unable to go fullscreen
	#define OC_ERROR_SDL_OPENGL			-25		///< Unable to load OpenGL library dynamically

// Settings file
	#define OC_CONFIG_FILE_FILENAME		"config/opencity.xml"

// Others macros
	#define OC_WINDOW_NAME PACKAGE VERSION
	#define OC_PROGRAM_NAME				"OpenCity standalone/client application"



   /*=====================================================================*/
   /*                         LOCAL    VARIABLES                          */
   /*=====================================================================*/
/// The current user interface is pointed by this pointer
	static UI* uipCurrentUI		= NULL;

/// Set to true when the user request to quit the program
	static bool bQuit			= false;
	static bool bRestart		= false;

/// Flags we will pass into SDL_SetVideoMode.
	static int iVideoFlag		= SDL_OPENGL;

/// The paths are static so that the others can not access this
	static string sDataDir		= "";
	static string sSaveDir		= "";
	static string sConfigDir	= "";

/// Set to true if OpenGL version string is requested from command line
	static bool bGLVersion		= false;


   /*=====================================================================*/
void ocQuit( const int quit_code )
{
	cout << endl
		 << "Quit requested, quit code is : " << quit_code
	     << endl
	     << "Bye bye !"
	     << endl;
	bQuit = true;
}


   /*=====================================================================*/
void ocRestart()
{
	cout << "Restart with a new city from scratch. " << endl;
	bRestart = true;
}


   /*=====================================================================*/
void ocSetNewUI( UI * pcNewUI)
{
	uipCurrentUI = pcNewUI;
}


   /*=====================================================================*/
void ocKeyboard( const SDL_KeyboardEvent& rcEvent )
{
	if (uipCurrentUI != NULL) {
		uipCurrentUI->Keyboard( rcEvent );
	}
}


   /*=====================================================================*/
void ocMouseButton( const SDL_MouseButtonEvent& rcEvent )
{
	if (uipCurrentUI != NULL) {
		uipCurrentUI->MouseButton( rcEvent );
	}
}


   /*=====================================================================*/
void ocMouseMotion( const SDL_MouseMotionEvent& motionEvent )
{
	if (uipCurrentUI != NULL) {
		uipCurrentUI->MouseMotion( motionEvent );
	}
}


   /*=====================================================================*/
void ocResize( const SDL_ResizeEvent& rcsResizeEvent)
{
#ifndef __WIN32__
// Linux needs this whereas Win32 does not
// Set the new window's size
	if( SDL_SetVideoMode(
		rcsResizeEvent.w, rcsResizeEvent.h,
		gVars.guiVideoBpp, iVideoFlag ) == 0 ) {
		OPENCITY_FATAL( "Video mode reset failed: " << SDL_GetError( ) );
		exit( OC_ERROR_SDL_VIDEORESIZE );
	}
	gVars.gpVideoSrf = SDL_GetVideoSurface();
#endif

// Save the new screen size
	gVars.guiScreenWidth = rcsResizeEvent.w;
	gVars.guiScreenHeight = rcsResizeEvent.h;

	if (uipCurrentUI != NULL) {
		uipCurrentUI->Resize( rcsResizeEvent );
	}
}


   /*=====================================================================*/
void ocActive( const SDL_ActiveEvent & e)
{
//	OPENCITY_DEBUG( "Active event received" );

	if (e.state & SDL_APPACTIVE) {
		gVars.gboolActive = (e.gain == 1);
	}
}


   /*=====================================================================*/
void ocExpose( const SDL_ExposeEvent& rcsExposeEvent )
{
	if (uipCurrentUI != NULL) {
		uipCurrentUI->Expose( rcsExposeEvent );
	}
}


   /*=====================================================================*/
void ocProcessSDLEvents( void )
{
	static SDL_Event event;

// Grab all the events off the queue.
	while( SDL_PollEvent( &event ) ) {

		switch( event.type ) {
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			ocKeyboard( event.key );
			break;

		case SDL_MOUSEMOTION:
			ocMouseMotion( event.motion );
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			ocMouseButton( event.button );
			break;

		case SDL_VIDEORESIZE:
			ocResize( event.resize );
			break;

		case SDL_ACTIVEEVENT:
			ocActive( event.active );
			break;

		case SDL_VIDEOEXPOSE:
			ocExpose( event.expose );
			break;

		case SDL_QUIT:
		// Handle quit requests (like Ctrl-c).
			cout << endl << "Quit requested, stoping " << OC_PROGRAM_NAME << "..." << endl;
			bQuit = true;
			break;
		}
	}
}


   /*=====================================================================*/
static int initSDL()
{
// Initialization of the SDL library
	OPENCITY_DEBUG( "SDL Initialization" );

// First, initialize SDL's video subsystem.
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		// Failed, exit.
		OPENCITY_FATAL( "SDL video initialization failed: " << SDL_GetError() );
		return OC_ERROR_SDL_INIT;
	}

// Set the SDL_GL_DOUBLEBUFFER ON for smoother rendering
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );

// Get the OpenGL driver name
	const char* glDriver;
	if (gVars.gsOpenGLDriver != "") {
		OPENCITY_INFO( "Loading OpenGL driver: " << gVars.gsOpenGLDriver );
		glDriver = gVars.gsOpenGLDriver.c_str();
	}
	else {
		OPENCITY_INFO( "Loading default OpenGL driver..." );
		glDriver = NULL;
	}

// Dynamically load the default OpenGL implementation library
	if (SDL_GL_LoadLibrary(glDriver) < 0) {
		OPENCITY_FATAL( "Failed to load OpenGL library: " << SDL_GetError() );
		return OC_ERROR_SDL_OPENGL;
	}

// Will we go for fullscreen ?
	if (gVars.gboolFullScreen == true) {
		iVideoFlag |= SDL_FULLSCREEN;

	// Use the current desktop video resolution
		gVars.guiScreenWidth	= 0;
		gVars.guiScreenHeight	= 0;
	}
	else {
		iVideoFlag |= SDL_RESIZABLE;
	}

// OK, go for the video settings now
	gVars.gpVideoSrf = SDL_SetVideoMode( gVars.guiScreenWidth, gVars.guiScreenHeight, gVars.guiVideoBpp, iVideoFlag );
	if ( gVars.gpVideoSrf == NULL ) {
	// This could happen for a variety of reasons, including DISPLAY
	// not being set, the specified resolution not being available, etc.
		OPENCITY_ERROR(
			"Initialization of " << gVars.guiVideoBpp <<
			" bpp video mode failed: " << SDL_GetError()
		);
		return OC_ERROR_SDL_INIT;
	}
	else {
		OPENCITY_INFO(
			"Using " << (uint)gVars.gpVideoSrf->w <<
			"x" << (uint)gVars.gpVideoSrf->h <<
			" at " << (uint)gVars.gpVideoSrf->format->BitsPerPixel << " bpp"
		);

	// Store the fullscreen resolution for later use
		if (gVars.gboolFullScreen == true) {
			gVars.guiScreenWidth	= (uint)gVars.gpVideoSrf->w;
			gVars.guiScreenHeight	= (uint)gVars.gpVideoSrf->h;
		}
	}

// Retrieve the video driver name
	#define BUFFER_SIZE 256
	char myBuffer[BUFFER_SIZE];
	if (SDL_VideoDriverName(myBuffer, BUFFER_SIZE) != NULL) {
		OPENCITY_INFO( "Current video driver: " << myBuffer );
	}
	else {
		OPENCITY_ERROR( "Failed to retrieve the video driver name" );
	}

// Display the OpenGL version string
	if (bGLVersion == true) {
		OPENCITY_INFO( "GL vendor: "	 << glGetString( GL_VENDOR ) );
		OPENCITY_INFO( "GL renderer: "	 << glGetString( GL_RENDERER ) );
		OPENCITY_INFO( "GL version: "	 << glGetString( GL_VERSION ) );
		OPENCITY_INFO( "GL extensions: " << glGetString( GL_EXTENSIONS ) );
	}

// Double buffer available ?
	int iValue = 0;
	SDL_GL_GetAttribute( SDL_GL_DOUBLEBUFFER, &iValue );
	if ( iValue == 0 ) {
		OPENCITY_FATAL( "Video double buffer unsupported" );
		return OC_ERROR_SDL_DOUBLEBUFFER;
	}
	else {
		OPENCITY_INFO( "Video double buffer supported" );
	}

// Accelerated display available ?
	SDL_GL_GetAttribute( SDL_GL_ACCELERATED_VISUAL, &iValue );
	if ( iValue == 0 ) {
		OPENCITY_FATAL( "Accelerated visual unsupported" );
		return OC_ERROR_SDL_ACCELERATED;
	}
	else {
		OPENCITY_INFO( "Accelerated visual supported" );
	}

	return 0;
}


   /*=====================================================================*/
static string formatPath(const string& rcsPath)
{
	string result = rcsPath;

	if (result.size() > 0) {
	// Delete all quotes "
		string::size_type pos;
		while ( (pos = result.find( '\"' )) != result.npos ) {
		    result.erase( pos );
		}
	// Append the "/" to path
		if (result[ result.size()-1 ] != '/')
			result += '/';
	}
	else {
		result = "/";
	}

	return result;
}


   /*=====================================================================*/
static int parseArg(int argc, char *argv[])
{
// Command-line options definition
	enum {
		OPT_GL_VERSION,
		OPT_GL_DRIVER,
		OPT_FULL_SCREEN,
		OPT_NO_AUDIO,
		OPT_DATA_DIR,
		OPT_CONF_DIR,
		OPT_GENERATOR_HEIGHT_MAP,
		OPT_GENERATOR_SEED,
		OPT_GENERATOR_MAP,
		OPT_GENERATOR_WATER,
		OPT_GENERATOR_MAP_SHAPE,
		OPT_GENERATOR_TREE_DENSITY,
		OPT_HELP
	};

	CSimpleOpt::SOption g_rgOptions[] = {
		{ OPT_GL_VERSION,				(char*)"--gl-version",				SO_NONE		},
		{ OPT_GL_VERSION,				(char*)"-glv",						SO_NONE		},
		{ OPT_GL_DRIVER,				(char*)"--gl-driver",				SO_REQ_SEP	},
		{ OPT_GL_DRIVER,				(char*)"-gld",						SO_REQ_SEP	},
		{ OPT_FULL_SCREEN,				(char*)"--full-screen",				SO_NONE		},
		{ OPT_FULL_SCREEN,				(char*)"-fs",						SO_NONE		},
		{ OPT_NO_AUDIO,					(char*)"--no-audio",				SO_NONE		},
		{ OPT_NO_AUDIO,					(char*)"-na",						SO_NONE		},
		{ OPT_DATA_DIR,					(char*)"--data-dir",				SO_REQ_SEP	},
		{ OPT_DATA_DIR,					(char*)"-dd",						SO_REQ_SEP	},
		{ OPT_CONF_DIR,					(char*)"--conf-dir",				SO_REQ_SEP	},
		{ OPT_CONF_DIR,					(char*)"-cd",						SO_REQ_SEP	},
		{ OPT_GENERATOR_HEIGHT_MAP,		(char*)"--generator-height-map",	SO_REQ_SEP	},
		{ OPT_GENERATOR_SEED,			(char*)"--generator-seed",			SO_REQ_SEP	},
		{ OPT_GENERATOR_MAP,			(char*)"--generator-map",			SO_REQ_SEP	},
		{ OPT_GENERATOR_WATER,			(char*)"--generator-water",			SO_REQ_SEP	},
		{ OPT_GENERATOR_MAP_SHAPE,		(char*)"--generator-map-shape",		SO_REQ_SEP	},
		{ OPT_GENERATOR_TREE_DENSITY,	(char*)"--generator-tree-density",	SO_REQ_SEP	},
		{ OPT_HELP,						(char*)"--help",					SO_NONE		},
		{ OPT_HELP,						(char*)"-h",						SO_NONE		},
		SO_END_OF_OPTIONS // END
	};

// SimpleOpt parser initialization
	CSimpleOpt args(argc, argv, g_rgOptions, SO_O_EXACT | SO_O_NOSLASH | SO_O_SHORTARG | SO_O_CLUMP );
	int i = 0;
	while (args.Next()) {
		switch (args.LastError()) {
		case SO_OPT_INVALID:
			OPENCITY_OPTION( "" << args.OptionText() << " unrecognized" );
			break;
		case SO_OPT_MULTIPLE:
			OPENCITY_OPTION( "" << args.OptionText() << " matched multiple options" );
			break;
		case SO_ARG_INVALID:
			OPENCITY_OPTION( "" << args.OptionText() << " does not accept any argument" );
			break;
		case SO_ARG_INVALID_TYPE:
			OPENCITY_OPTION( "" << args.OptionText() << " has an invalid argument format" );
			break;
		case SO_ARG_MISSING:
			OPENCITY_OPTION( "" << args.OptionText() << " requires an argument" );
			break;
		case SO_ARG_INVALID_DATA:
			OPENCITY_OPTION( "" << args.OptionText() << " has an invalid argument data" );
			break;
		case SO_SUCCESS:
			OPENCITY_OPTION( "" << args.OptionText() << " detected" );
			break;
		} // switch (args.LastError())

		// Exit the program on error
		if (args.LastError() != SO_SUCCESS) {
			OPENCITY_OPTION( "Try " << argv[0] << " --help for more usage information" );
			exit( OC_ERROR_ARGUMENT );
		}

		switch (args.OptionId()) {
		case OPT_GL_VERSION:
			bGLVersion = true;
			break;

		case OPT_GL_DRIVER:
			gVars.gsOpenGLDriver = args.OptionArg();
			break;

		case OPT_FULL_SCREEN:
			gVars.gboolFullScreen = true;
			break;

		case OPT_NO_AUDIO:
			gVars.gboolUseAudio = false;
			break;

		case OPT_DATA_DIR:
			sDataDir = formatPath(args.OptionArg());
			OPENCITY_OPTION( "The data directory is: \"" << sDataDir << "\"" );
			break;

		case OPT_CONF_DIR:
			sConfigDir = formatPath(args.OptionArg());
			OPENCITY_OPTION( "The configuration directory is: \"" << sConfigDir << "\"" );
			break;

		case OPT_GENERATOR_HEIGHT_MAP:
			gVars.gsGeneratorHeightMap = args.OptionArg();
			break;

		case OPT_GENERATOR_SEED:
			i = atoi( args.OptionArg() ); // return 0 as error value
			if (i != 0 || ( i == 0 && strcmp(args.OptionArg(), "0") == 0 )) {
				gVars.guiGeneratorSeed = i;
			}
			else {
				OPENCITY_INFO(
					"Argument provided to --generator-seed must be an integer. The default value is used instead."
				);
			}
			break;

		case OPT_GENERATOR_MAP:
			i = atoi( args.OptionArg() ); // return 0 as error value
			if (i != 0 || ( i == 0 && strcmp(args.OptionArg(), "0") == 0 )) {
				gVars.guiGeneratorMapType = MapGen::MapMaker::MAP_TYPE(i);
			}
			else {
				OPENCITY_INFO(
					"Argument provided to --generator-map must be an integer. The default value is used instead."
				);
			}
			break;

		case OPT_GENERATOR_WATER:
			i = atoi( args.OptionArg() ); // return 0 as error value
			if (i != 0 || ( i == 0 && strcmp(args.OptionArg(), "0") == 0 )) {
				gVars.guiGeneratorWaterType = MapGen::MapMaker::WATER_TYPE(i);
			}
			else {
				OPENCITY_INFO(
					"Argument provided to --generator-water must be an integer. The default value is used instead." );
			}
			break;

		case OPT_GENERATOR_MAP_SHAPE:
			i = atoi( args.OptionArg() ); // return 0 as error value
			if (i != 0 || ( i == 0 && strcmp(args.OptionArg(), "0") == 0 )) {
				gVars.guiGeneratorMapShapeType = MapGen::MapMaker::MAP_SHAPE_TYPE(i);
			}
			else {
				OPENCITY_INFO(
					"Argument provided to --generator-map-shape must be an integer. The default value is used instead."
				);
			}
			break;

		case OPT_GENERATOR_TREE_DENSITY:
			i = atoi( args.OptionArg() ); // return 0 as error value
			if (i != 0 || ( i == 0 && strcmp(args.OptionArg(), "0") == 0 )) {
				gVars.guiGeneratorTreeDensityType = MapGen::MapMaker::TREE_DENSITY_TYPE(i);
			}
			else {
				OPENCITY_INFO(
					"Argument provided to --generator-tree-density must be an integer. The default value is used instead."
				);
			}
			break;

		case OPT_HELP:
			cout << "Usage: " << argv[0]
				<< " [-fs|--full-screen] [-glv|--gl-version] [-gld|--gl-driver openGLDriverName]"
				<< " [-dd|--data-dir newDataPath] [-cd|--conf-dir newConfigPath]"
				<< " [-na|--no-audio] [--generator-height-map heightMapPicture |"
				<< " (--generator-seed seed [--generator-map MAP-TYPE] [--generator-water WATER-TYPE]"
				<< " [--generator-map-shape MAP-SHAPE-TYPE] [--generator-tree-density TREE-DENSITY-TYPE])]"
				<< endl << endl
				<< "Where the map generator constants are: " << endl
				<< "   MAP-TYPE         : 0=plain (default), 1=hill, 2=mountain" << endl
				<< "   TREE-DENSITY-TYPE: 0=sparse (default), 1=normal, 2=dense" << endl
				<< "   MAP-SHAPE-TYPE   : 0=none (default), 1=island, 2=volcano, 3=crater" << endl
				<< "   WATER-TYPE       : 0=dry, 1=lake (default), 2=coast" << endl
				<< endl;
			cout << "Warning: the command option overwrite the config file settings."
				<< endl;
			exit( OC_ERROR_ARGUMENT );
			break;
		}	// switch (args.OptionId())
	} // while (args.Next())

	return 0;
}


   /*=====================================================================*/
static void displaySplash()
{
	#define OC_SPLASH_CLEAR_COLOR		.15, .15, .3, 1.0

	glClearColor( OC_SPLASH_CLEAR_COLOR );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	gVars.gpRenderer->DisplaySplash();
}


   /*=====================================================================*/
static void displayStatus( const string & str )
{
	assert( gVars.gpRenderer != NULL );

	uint x, y;

	displaySplash();

// Center the text on the screen
	x = (gVars.gpVideoSrf->w - str.size()*10)/2;
	y = (gVars.gpVideoSrf->h - 140) / 2;
	gVars.gpRenderer->DisplayText( x, y, OC_BLUE_COLOR, str );
	SDL_GL_SwapBuffers();
}


   /*=====================================================================*/
static int clientMode()
{
// Initialize SDL
	if (gVars.gpVideoSrf == NULL) {
		int errorCode = initSDL();
		if (errorCode != 0) {
			return errorCode;
		}
	}


// Set the window's caption
	SDL_WM_SetCaption( PACKAGE " " VERSION, NULL );
	SDL_WM_SetIcon( IMG_Load(ocDataDirPrefix("graphism/icon/OpenCity32.png").c_str()), NULL );


// Create the mutex first
	gVars.gpmutexSim = SDL_CreateMutex();


// Load OpenGL extensions
	gVars.gpExtensionMgr = new ExtensionManager();
	if (gVars.gpExtensionMgr == NULL or !gVars.gpExtensionMgr->Load()) {
		OPENCITY_FATAL( "Failed to load OpenGL extensions" );
		return OC_ERROR_SDL_OPENGL;
	}


// Create the global renderer in order to use its text rendering functions
	gVars.gpRenderer = new Renderer( gVars.guiCityWidth, gVars.guiCityLength );


// AudioManager's initialization
	displayStatus( "Looking for GPU freezing system... ");
	gVars.gpAudioMgr = new AudioManager();

	if ( gVars.gpAudioMgr == NULL ) {
		OPENCITY_FATAL( "Error while creating the audio manager" );
		return OC_ERROR_MEMORY;
	} else
	if ( (gVars.gboolUseAudio == true)
	  and (gVars.gpAudioMgr->OpenAudio() != OC_ERR_FREE))
	{
	// try to open the audio device IF we have successfully created an audio object
		OPENCITY_INFO( "Audio open error ! OpenCity continues happily." );
	}


// Load musics and sounds
	displayStatus( "Warming up central processing unit...");
	gVars.gpAudioMgr->LoadMusicList( ocDataDirPrefix(OC_MUSIC_LIST_FILENAME), ocDataDirPrefix("") );
	OPENCITY_INFO( "Loaded " << gVars.gpAudioMgr->GetNumberMusic() << " musics." );
	gVars.gpAudioMgr->LoadSoundList( ocDataDirPrefix(OC_SOUND_LIST_FILENAME), ocDataDirPrefix("") );
	OPENCITY_INFO( "Loaded " << gVars.gpAudioMgr->GetNumberSound() << " sounds." );


// Create the other required global managers
	displayStatus( "Initializing the vibration detector..." );
	gVars.gpMapMaker = new MapGen::MapMaker(
		gVars.guiCityWidth, gVars.guiCityLength,
		gVars.gsGeneratorHeightMap,
		gVars.guiGeneratorMapType,
		gVars.guiGeneratorWaterType,
		gVars.guiGeneratorMapShapeType,
		gVars.guiGeneratorTreeDensityType,
		gVars.guiGeneratorSeed
	);

	displayStatus( "Activating embedded GPS...");
	gVars.gpMapMgr = new Map( gVars.guiCityWidth, gVars.guiCityLength );

	displayStatus( "Calibrating earthquake subsystem...");
	gVars.gpGraphicMgr = new GraphicManager();

	displayStatus( "Shaking DNA mixer thread...");
	gVars.gpPropertyMgr = new PropertyManager();

	displayStatus( "Mounting intergalactic hyperlink ...");
	gVars.gpNetworking = new Networking();

	displayStatus( "Initializing the particle handler ...");
	gVars.gpMoveMgr = new MovementManager( gVars.gpGraphicMgr, gVars.gpMapMgr );


// The pointer of our new city
	City* pNewCity = new City( gVars.guiCityWidth, gVars.guiCityLength );
	if (pNewCity == NULL) {
		OPENCITY_FATAL( "Error while creating new city" );
		return OC_ERROR_MEMORY;
	}
	displayStatus( "Almost done...");


// FIXME: buggy MAS
/*
// Create the necessary classes for the Multi-Agent System
	gVars.gpKernel = new Kernel();
	gVars.gpEnvironment = new Environment(
		gVars.guiCityWidth, gVars.guiCityLength, pNewCity->GetLayer( OC_LAYER_BUILDING ), gVars.gpKernel );

	new AgentPolice(*gVars.gpKernel, *gVars.gpEnvironment, 1, 2);
	new AgentPolice(*gVars.gpKernel, *gVars.gpEnvironment, 3, 4);
	new AgentDemonstrator(*gVars.gpKernel, *gVars.gpEnvironment, 4, 2);
*/

	while (!bQuit) {
	// Process input events
		ocProcessSDLEvents();

	// Restart with a new city from scratch
	// WARNING: code duplication
		if (bRestart) {
		// Lock the simulator
			SDL_LockMutex( gVars.gpmutexSim );

		// Remove all moving objects
			gVars.gpMoveMgr->Remove();

			delete pNewCity;
			delete gVars.gpMapMgr;
			delete gVars.gpMapMaker;

			gVars.guiGeneratorSeed = time(NULL);
			gVars.gpMapMaker = new MapGen::MapMaker(
				gVars.guiCityWidth, gVars.guiCityLength,
				gVars.gsGeneratorHeightMap,
				gVars.guiGeneratorMapType,
				gVars.guiGeneratorWaterType,
				gVars.guiGeneratorMapShapeType,
				gVars.guiGeneratorTreeDensityType,
				gVars.guiGeneratorSeed
			);
			gVars.gpMapMgr = new Map( gVars.guiCityWidth, gVars.guiCityLength );
			pNewCity = new City( gVars.guiCityWidth, gVars.guiCityLength );
			if (pNewCity == NULL) {
				OPENCITY_FATAL( "Error while creating new city" );
				return OC_ERROR_MEMORY;
			}
			gVars.gpRenderer->bHeightChange = true;
			gVars.gpRenderer->bMinimapChange = true;

		// Unlock the simulator
			SDL_UnlockMutex( gVars.gpmutexSim );

			bRestart = false;
		}

	// Process city's task
		pNewCity->Run();
		//gVars.gpKernel->live();

	// IF the application is not iconified THEN update the display
		if (gVars.gboolActive)
			pNewCity->Display();

#undef OC_PRINT_FPS
#ifndef OC_PRINT_FPS
		SDL_Delay( gVars.guiMsPerFrame );
#else
		static Uint32 uiNumberTick = SDL_GetTicks();
		static uint uiNumberFrame = 0;

		uiNumberFrame++;
		if ((SDL_GetTicks() - uiNumberTick) > 5000) {
			cout << uiNumberFrame << " frames per 5 seconds = "
					<< uiNumberFrame / 5 << " FPS" << endl;
			uiNumberTick = SDL_GetTicks();
			uiNumberFrame = 0;
		}
		SDL_Delay( gVars.guiMsPerFrame );
#endif
	} // while (!bQuit)

	//delete gVars.gpEnvironment;
	//delete gVars.gpKernel;


// Close the network connection
	gVars.gpNetworking->Close();

// WARNING: the deleting/creating order is very important !
	delete pNewCity;

	delete gVars.gpMoveMgr;
	delete gVars.gpNetworking;
	delete gVars.gpPropertyMgr;
	delete gVars.gpGraphicMgr;
	delete gVars.gpMapMgr;
	delete gVars.gpMapMaker;

// close the audio device then delete the audio manager
	gVars.gpAudioMgr->CloseAudio();
	delete gVars.gpAudioMgr;

	delete gVars.gpRenderer;
	delete gVars.gpExtensionMgr;

// delete the simulators' mutex now
	SDL_DestroyMutex( gVars.gpmutexSim );

//	SDL_FreeSurface( gVars.gpVideoSrf ); // This is not recommended by SDL documentation
	gVars.gpVideoSrf = NULL;

	SDL_Quit();					// WARNING: Calls free() on an invalid pointer. Detected by glibc

	return 0;
}


   /*=====================================================================*/
static void printCopyright() {
// Output the copyright text
	cout << "Welcome to " << PACKAGE << " version " << VERSION << endl;
	cout << "Copyright (C) by Duong Khang NGUYEN. All rights reserved." << endl;
	cout << "   web  : http://www.opencity.info" << endl;
	cout << "   email: neoneurone @ gmail com" << endl << endl;

	cout << "This program is released under the terms of" << endl;
	cout << "GNU General Public License (See the COPYING file for more details)" << endl << endl;

	cout << "Starting " << OC_PROGRAM_NAME << "..." << endl << endl;
}


   /*=====================================================================*/
/** Return the save directory.
	\return The pointer to the absolute directory. The caller must free
the pointer if it's not used anymore.
*/
static char* findSaveDir()
{
	char* ret = NULL;

#ifndef __WIN32__
// Get the home directory from the environment variable
	char* env = getenv("HOME");
	if (env != NULL) {
		ret = (char*)malloc( strlen(env) + 1 );
		strcpy( ret, env );
	}
#else
// Find the directory: "C:\Documents and Settings\username\Application Data"
// Required shell DLL version: 5.0 or later
// header: shlobj.h
// lib: shell32.lib ?
// dll: shell32.dll

	TCHAR szPath[MAX_PATH];

	if(SUCCEEDED(
		SHGetFolderPath(NULL, CSIDL_APPDATA|CSIDL_FLAG_CREATE, NULL, 0, szPath)
		)) {
		ret = (char*)malloc( strlen(szPath) + 1 );
		strcpy( ret, szPath );
	}
#endif

// The required save directory does not exist, use the current directory
	if (ret == NULL) {
		ret = (char*)malloc( 2 );
		strcpy( ret, "." );
	}

	return ret;
}


   /*=====================================================================*/
/** Try to detect and set the datadir, the confdir and the savedir using
BinReloc library and win32 standard function
*/
static void detectProgramPath()
{
	char* pTemp = NULL;
	BrInitError brError;

// IF the datadir is not set THEN try to get it from BinReloc routines
	if (sDataDir == "") {
	// Default system directory settings
		sDataDir = DATADIR;

	// Init the BinReloc routines
		if (br_init(&brError) != 1) {
			OPENCITY_INFO(
				"Failed to initialized BinReloc routines to search for the datadir. " <<
				"The error was: " << brError
			);
		}

	// Construct the datadir from the prefix
		pTemp = br_find_data_dir( sDataDir.c_str() );
		sDataDir = pTemp;
		sDataDir += "/";
		sDataDir += PACKAGE;
		sDataDir = formatPath( sDataDir );
		free(pTemp);
	}

// IF the configdir is not set THEN try to get it from BinReloc routines
	if (sConfigDir == "") {
	// Default system directory settings
		sConfigDir = SYSCONFDIR;

	// Init the BinReloc routines
		if (br_init(&brError) != 1) {
			OPENCITY_INFO(
				"Failed to initialized BinReloc routines to search for the confdir. " <<
				"The error was: " << brError
			);
		}

	// Construct the pkgsysconfdir from the prefix
		pTemp = br_find_etc_dir( sConfigDir.c_str() );
		sConfigDir = pTemp;
		sConfigDir += "/";
		sConfigDir += PACKAGE;
		sConfigDir = formatPath( sConfigDir );
		free(pTemp);
	}

// IF the save directory is not set the find it
	if (sSaveDir == "") {
		pTemp = findSaveDir();
		sSaveDir = pTemp;
		free(pTemp);
#ifndef __WIN32__
		sSaveDir += "/.opencity/";
		mkdir( sSaveDir.c_str(), 0755 );
#else
	// Win32 uses \ as directory separtor
		sSaveDir += "\\opencity\\";
        CreateDirectory( sSaveDir.c_str(), NULL );
    // Replace \ by /
	    string::size_type pos;
	    while ( (pos = sSaveDir.find( '\\' )) != sSaveDir.npos ) {
		    sSaveDir.replace( pos, 1, "/" );
		}
#endif
	}

// Print out some information
	OPENCITY_INFO( "Detected data directory   : " << sDataDir );
	OPENCITY_INFO( "Detected config directory : " << sConfigDir );
	OPENCITY_INFO( "Detected save directory   : " << sSaveDir );
}


   /*=====================================================================*/
/** Read the OpenCity's main settings file "opencity.xml"
	\return "" if OK, otherwise the error description
		0: if OK
		OC_ERROR_NOT_FOUND: the config file has not been found
		OC_ERROR_PARSE_CONFIG: the was a parse error
*/
static string readSettings()
{
	string errorString = "";
	TiXmlDocument settings;

// Now try to open the config file then read it
	OPENCITY_INFO(
		"Reading XML config file: \"" << ocConfigDirPrefix(OC_CONFIG_FILE_FILENAME) << "\""
	);

// Load the settings file
	string fn = ocConfigDirPrefix(OC_CONFIG_FILE_FILENAME);
	if (!settings.LoadFile(fn)) {
		errorString = settings.ErrorDesc();
		return errorString;
	}

// Error testing
	if (settings.Error()) {
		errorString = settings.ErrorDesc();
		return errorString;
	}

// Get the root element
	TiXmlNode* pRoot = settings.RootElement();
	if (pRoot == NULL) {
		errorString = settings.ErrorDesc();
		return errorString;
	}

// Parse the settings
	TiXmlElement* pElement = pRoot->FirstChildElement();
	const char* str = NULL;
	while (pElement != NULL)
	{
// Debug
//		cout << i++ << "||" << *pElement << std::endl;
	// "fullscreen" element
		if (pElement->ValueStr() == "fullscreen") {
			str = pElement->Attribute("enable");
			if (str != NULL && strcmp(str, "true") == 0) {
				gVars.gboolFullScreen |= true;
			}
			else {
				gVars.gboolFullScreen |= false;
			}

			// IF fullscreen mode enabled THEN read size
			if (gVars.gboolFullScreen) {
				TiXmlElement* pChild = pElement->FirstChildElement();
				while (pChild != NULL) {
					if (pChild->ValueStr() == "width") {
						pChild->QueryIntAttribute("value", (int*)&gVars.guiScreenWidth);
					}
					else if (pChild->ValueStr() == "height") {
						pChild->QueryIntAttribute("value", (int*)&gVars.guiScreenHeight);
					}
					pChild = pChild->NextSiblingElement();
				}
			}
		}
	// "audio" element
		else if (pElement->ValueStr() == "audio") {
			str = pElement->Attribute("enable");
			if (str != NULL && strcmp(str, "true") == 0) {
				gVars.gboolUseAudio &= true;
			} else {
				gVars.gboolUseAudio &= false;
			}
		}
	// "city" element, read the city's size
		else if (pElement->ValueStr() == "city") {
			TiXmlElement* pChild = pElement->FirstChildElement();
			while (pChild != NULL) {
// Debug
//				cout << i++ << "||" << *pChild << std::endl;
				if (pChild->ValueStr() == "width") {
					pChild->QueryIntAttribute("value", (int*)&gVars.guiCityWidth);
				}
				else if (pChild->ValueStr() == "length") {
					pChild->QueryIntAttribute("value", (int*)&gVars.guiCityLength);
				}
				pChild = pChild->NextSiblingElement();
			}
		}
	// "msPerFrame" element
		else if (pElement->ValueStr() == "msPerFrame") {
			pElement->QueryIntAttribute("value", (int*)&gVars.guiMsPerFrame);
		}
	// "zenServer" element
		else if (pElement->ValueStr() == "zenServer") {
			if (pElement->GetText() != NULL)
				gVars.gsZenServer = pElement->GetText();
		}

		pElement = pElement->NextSiblingElement();
	}

	return errorString;
}


   /*=====================================================================*/
static void initGlobalVar()
{
// Config file and command line options
	gVars.gboolUseAudio				= true;
	gVars.gboolFullScreen			= false;
	gVars.gboolServerMode			= false;
	gVars.guiCityWidth				= OC_CITY_W;
	gVars.guiCityLength				= OC_CITY_L;
	gVars.guiMsPerFrame				= OC_MS_PER_FRAME;
	gVars.guiScreenWidth			= OC_WINDOW_WIDTH;
	gVars.guiScreenHeight			= OC_WINDOW_HEIGHT;
	gVars.guiVideoBpp				= OC_WINDOW_BPP_DEFAULT;
	gVars.gsOpenGLDriver			= "";

	gVars.gsGeneratorHeightMap			= "";
	gVars.guiGeneratorSeed				= time(NULL);
	gVars.guiGeneratorMapType			= MapGen::MapMaker::PLAIN;
	gVars.guiGeneratorWaterType			= MapGen::MapMaker::LAKE;
	gVars.guiGeneratorMapShapeType		= MapGen::MapMaker::NONE;
	gVars.guiGeneratorTreeDensityType	= MapGen::MapMaker::SPARSE;

	gVars.gfMsSimDelayMax			= 0;
	gVars.gsZenServer				= "localhost";

// Application status
	gVars.gboolActive				= true;		// the application is active at start

// The mutex that all the simulators depend on
	gVars.gpmutexSim				= NULL;

// The famous renderer
	gVars.gpRenderer				= NULL;

// Datamanagers
	gVars.gpAudioMgr				= NULL;		// global Audio Manager
	gVars.gpGraphicMgr				= NULL;		// global Graphic Manager
	gVars.gpPropertyMgr				= NULL;		// global Property Manager
	gVars.gpMapMaker				= NULL;		// global map maker
	gVars.gpMapMgr					= NULL;		// global height Map Manager
	gVars.gpNetworking				= NULL;		// global networking support class
	gVars.gpPathFinder				= NULL;		// global pathfinder class
	gVars.gpMoveMgr					= NULL;		// global movement manager

// Multi-Agent System
	gVars.gpKernel					= NULL;		// global MAS Kernel
	gVars.gpEnvironment				= NULL;		// global Environement class

// The SDL video surface
	gVars.gpVideoSrf				= NULL;		// global video screen surface
}


   /*=====================================================================*/
//#ifdef __WIN32__
//extern "C"
//#endif
int main(int argc, char *argv[])
{
// Initialize the global settings variable to the default values
	initGlobalVar();

// Print out the copyright
	printCopyright();

// Parse the command-line options
	int returnCode = parseArg( argc, argv );
	if (returnCode != 0) {
		return returnCode;
	}

// Detect the main path: sDataDir and sSaveDir
	detectProgramPath();

// Read the application settings from the XML settings file
	string errorDesc = readSettings();
	if (errorDesc != "") {
		OPENCITY_FATAL(
			"The was an error while loading the settings file: \"" << errorDesc << "\"" << endl
			<< "If the main config file \"" << OC_CONFIG_FILE_FILENAME << "\" has not been found then" << endl
			<< "try to specify the data directory with ""--data-dir"" "
			<< "and the configuration directory with ""--conf-dir""." << endl
			<< "For example:" << endl
			<< "    " << argv[0] << " --data-dir \"/absolute/path/to/opencity/data\" "
			<< "--conf-dir \"/absolute/path/to/opencity/conf\"" << endl
			<< "or" << endl
			<< "    " << argv[0] << " --data-dir \"../relative/path/to/opencity/data\" "
			<< "--conf-dir \"../relative/path/to/opencity/conf\"" << endl
		);
		exit(OC_ERROR_NOT_FOUND);
	}

// Test XPath
// 	PropertyManager2* pPropertyMgr = new PropertyManager2();
// 	const Property* myProperty = pPropertyMgr->Get( 1 );
// 	OPENCITY_DEBUG( "Build cost 1: " << myProperty->uiDestroyCost );
// 	const Property* myProperty2 = pPropertyMgr->Get( "graphism/residential/little_house/little_house.ac" );
// 	OPENCITY_DEBUG( "Build cost 2: " << myProperty2->uiIncome );
// 	delete pPropertyMgr;
// 	abort();

// Initialization of global variables
	uipCurrentUI = NULL;
	gVars.gfMsSimDelayMax = log10((OC_FLOAT)gVars.guiCityWidth*gVars.guiCityLength + 1) * OC_MS_GLOBAL_LOG_FACTOR;

// Initialize the random number generator
	srand( time(NULL) );

// Launch the game
	returnCode = clientMode();

	return returnCode;
}


   /*=====================================================================*/
   /*                       GLOBAL       FUNCTIONS                        */
   /*=====================================================================*/
string
ocDataDirPrefix( const string& s )
{
	return sDataDir + s;
}


   /*=====================================================================*/
string
ocConfigDirPrefix( const string& s )
{
	return sConfigDir + s;
}


   /*=====================================================================*/
string
ocSaveDirPrefix( const string& s )
{
	return sSaveDir + s;
}


   /*=====================================================================*/
string ocStrVersion()
{
	std::ostringstream oss;

	oss << OC_VERSION << "." << OC_PATCHLEVEL << "." << OC_SUBLEVEL;
	return oss.str();
}


   /*=====================================================================*/
long ocLongVersion()
{
	long lVersion = 0;

	lVersion = OC_VERSION*65536 + OC_PATCHLEVEL*256 + OC_SUBLEVEL;
	return lVersion;
}
























