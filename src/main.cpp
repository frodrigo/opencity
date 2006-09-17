/***************************************************************************
						main.cpp  -  description
							-------------------
	project              : OpenCity
	codename             : Delphine
	begin                : mer mai 28 2003
	copyright            : (C) 2003-2006 by Duong-Khang NGUYEN
	email                : neoneurone @ users sourceforge net

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

#include "main.h"
#include "city.h"				// The heart of the project
#include "audiomanager.h"		// Audio support
#include "graphicmanager.h"
#include "map.h"
#include "propertymanager.h"
#include "renderer.h"
#include "networking.h"			// Networking support
#include "movementmanager.h"
#include "conf.h"				// Parser for .conf file

#include "kernel.h"				// MAS kernel
#include "environment.h"		// MAS environment
#include "agentpolice.h"		// MAS testing
#include "agentdemonstrator.h"


#include "SDL_image.h"
#include "binreloc.h"			// BinReloc routines from AutoPackage

#include <cmath>				// For log10
#include <cstdlib>				// For getenv



   /*=====================================================================*/
   /*                           LOCAL     MACROS                          */
   /*=====================================================================*/
#ifndef WIN32
	#include <sys/stat.h>		// mkdir
#else
// Win32 specifics
	#include <shlobj.h>			// Windows shell technologies
	#define PREFIX "C:/Program Files"
#endif

// Window's settings
	#define OC_WINDOW_POS_X  20
	#define OC_WINDOW_POS_Y  20
	#define OC_WINDOW_WIDTH  750
	#define OC_WINDOW_HEIGHT 560
	#define OC_WINDOW_BPP_DEFAULT 32				// OC uses this by default
	#define OC_WINDOW_BPP_16 16
	#define OC_FULLSCREEN_WIDTH 1024
	#define OC_FULLSCREEN_HEIGHT 768

// Exit code
	#define OC_OPENCITY_CONF_NOT_FOUND	-1

// Others macros
	#define OC_WINDOW_NAME PACKAGE VERSION


   /*=====================================================================*/
   /*                        GLOBAL    VARIABLES                          */
   /*=====================================================================*/
// Config file and command line options
	bool gboolUseDisplayList	= true;
	bool gboolUseAudio			= true;
	bool gboolFullScreen		= false;
	bool gboolServerMode		= false;
	uint uiCityWidth			= OC_CITY_W;
	uint uiCityHeight			= OC_CITY_H;
	uint guiMsPerFrame			= OC_MS_PER_FRAME;
	uint guiVideoBpp			= OC_WINDOW_BPP_DEFAULT;
	OC_FLOAT gfMsSimDelayMax;
	string gsZenServer;

// The mutex that all the simulators depend on
	SDL_mutex* gpmutexSim			= NULL;

// The famous renderer
	Renderer* gpRenderer			= NULL;

// Datamanagers
	AudioManager* gpAudioMgr		= NULL;		// global Audio Manager
	GraphicManager* gpGraphicMgr	= NULL;		// global Graphic Manager
	PropertyManager* gpPropertyMgr	= NULL;		// global Property Manager
	Map* gpMapMgr					= NULL;		// global height Map Manager
	Networking* gpNetworking		= NULL;		// global networking support class
	PathFinder* gpPathFinder		= NULL;		// global pathfinder class
	MovementManager* gpMoveMgr		= NULL;		// global movement manager

// Multi-Agent System
	Kernel* gpKernel				= NULL;		// global MAS Kernel
	Environment* gpEnvironment		= NULL;		// global Environement class

// The SDL video surface
	SDL_Surface* gpVideoSrf			= NULL;		// global video screen surface


   /*=====================================================================*/
   /*                         LOCAL    VARIABLES                          */
   /*=====================================================================*/
/// The current user interface is pointed by this pointer
	static UI * uipCurrentUI = NULL;

/// Set to true when the user request to quit the program
	static bool boolQuit = false;

/// Flags we will pass into SDL_SetVideoMode.
	static int flags = 0;

/// Static so that the others can not access this
	static string gsHomeDir		= "";
	static string gsSaveDir		= "";



   /*=====================================================================*/
void ocPerror( const OPENCITY_ERR_CODE & err_code )
{
	cout << "Something went wrong. Error code : " << err_code << endl;
}


   /*=====================================================================*/
void ocKeyboard( const SDL_KeyboardEvent & rcsKeyboardEvent )
{
	if (uipCurrentUI != NULL) {
		uipCurrentUI->uiKeyboard( rcsKeyboardEvent );
	}
}


   /*=====================================================================*/
void ocMouseButton( const SDL_MouseButtonEvent & rcsMouseButtonEvent )
{
	if (uipCurrentUI != NULL) {
		uipCurrentUI->uiMouseButton( rcsMouseButtonEvent );
	}
}


   /*=====================================================================*/
void ocMouseMotion( const SDL_MouseMotionEvent & motionEvent )
{
	if (uipCurrentUI != NULL) {
		uipCurrentUI->uiMouseMotion( motionEvent );
	}
}


   /*=====================================================================*/
void ocResize( const SDL_ResizeEvent & rcsResizeEvent)
{
#ifndef WIN32
// Linux needs this where as Win32 does not
// Set the new window's size
	if( SDL_SetVideoMode(
		rcsResizeEvent.w, rcsResizeEvent.h,
		guiVideoBpp, flags ) == 0 ) {
		OPENCITY_FATAL( "Video mode reset failed: " << SDL_GetError( ) );
		exit (-4);
	}
	gpVideoSrf = SDL_GetVideoSurface();
#endif

	if (uipCurrentUI != NULL) {
		uipCurrentUI->uiResize( rcsResizeEvent );
	}
}


   /*=====================================================================*/
void ocExpose( const SDL_ExposeEvent & rcsExposeEvent )
{
	if (uipCurrentUI != NULL) {
		uipCurrentUI->uiExpose( rcsExposeEvent );
	}
}


   /*=====================================================================*/
void ocQuit( const int & quit_code )
{
	cout << "Quit requested, quit code is : " << quit_code
	     << endl
	     << "Bye bye !"
	     << endl;
	boolQuit = true;
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

		case SDL_VIDEOEXPOSE:
			ocExpose( event.expose );
			break;

		case SDL_QUIT:
		// Handle quit requests (like Ctrl-c).
			cout << "Quit requested, stoping OpenCity..." << endl;
			boolQuit = true;
			break;
		}
	}
}


   /*=====================================================================*/
void ocSetNewUI( UI * pcNewUI)
{
	uipCurrentUI = pcNewUI;
}


   /*=====================================================================*/
int initSDL()
{
	uint w = OC_WINDOW_WIDTH;
	uint h = OC_WINDOW_HEIGHT;

// Initialization of the SDL library
	OPENCITY_DEBUG( "SDL Initialization" );

// First, initialize SDL's video subsystem.
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		// Failed, exit.
		OPENCITY_FATAL( "SDL video initialization failed: " << SDL_GetError() );
		return -2;
	}

// Set the SDL_GL_DoubleBuffer ON for smoother rendering
// in addition, this is required for the implemented selection method
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

// Set 8 bits for each color component
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
 	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	flags = SDL_OPENGL | SDL_RESIZABLE | SDL_HWSURFACE;

// Will we go for fullscreen ?
	if (gboolFullScreen == true) {
		flags |= SDL_FULLSCREEN;
		w = OC_FULLSCREEN_WIDTH;
		h = OC_FULLSCREEN_HEIGHT;
	}

// OK, go for the video settings now
	gpVideoSrf = SDL_SetVideoMode( w, h, guiVideoBpp, flags );
	if ( gpVideoSrf == NULL ) {
	// This could happen for a variety of reasons,
	// including DISPLAY not being set, the specified
	// resolution not being available, etc.
		OPENCITY_ERROR( 
			"Initialization of 32 bpp video mode failed: " << SDL_GetError() 
		);
		OPENCITY_INFO( "Trying 16 bpp... " );
		guiVideoBpp = OC_WINDOW_BPP_16;

	// Set 5 bits for each color component
		SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 6 );
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
		SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 0 );
		gpVideoSrf = SDL_SetVideoMode( w, h, guiVideoBpp, flags );

		if (gpVideoSrf == NULL) {
			OPENCITY_FATAL( "16 bpp mode has failed: " << SDL_GetError() );
			return -4;
		}
		else {
			OPENCITY_INFO( "16 bpp works." );
		}
	}

//debug cout << "W: " << gpVideoSrf->w << " /H: " << gpVideoSrf->h << endl;


// Test for DoubleBuffer
	int iDblBuff = 0;
	SDL_GL_GetAttribute( SDL_GL_DOUBLEBUFFER,  &iDblBuff );
	if ( iDblBuff == 0 ) {
		OPENCITY_INFO( "Checking doublebuffer: failed !" );
		OPENCITY_FATAL( "We need doublebuffer" );
		return -6;
	}
	else {
		OPENCITY_INFO( "Checking doublebuffer: OK !" );
	}

/* TOKILL, not useful, commented out on Nov 29th, 05
// testing the videomode we've got
	if ( SDL_GetVideoInfo()->vfmt->BitsPerPixel != guiVideoBpp ) {
		cerr << "OpenCity needs '" << guiVideoBpp << "' bits per pixel."
		     << endl;
		return -5;
	}
*/

	return 0;
}


   /*=====================================================================*/
void formatHomeDir()
{
    string::size_type pos;

	if (gsHomeDir.size() > 0) {
    // Delete all quotes "
	    while ( (pos = gsHomeDir.find( '\"' )) != gsHomeDir.npos ) {
		    gsHomeDir.erase( pos );
		}
    // Append the "/" to HOMEDIR    
		if (gsHomeDir[ gsHomeDir.size()-1 ] != '/')
			gsHomeDir += '/';
	}
	else {
		gsHomeDir = "/";
	}
}


   /*=====================================================================*/
void parseArg(int argc, char *argv[])
{
	int counter;

	counter = 0;

	while (++counter < argc) {
		if (strcmp( argv[counter], "--no-dl" ) == 0) {
			cout << "<OPTION> " << argv[counter] << " detected" << endl;
			cout << "Disabled: display list" << endl;
			gboolUseDisplayList = false;
		} else
		if (strcmp( argv[counter], "--gl-version" ) == 0) {
			cout << "<OPTION> " << argv[counter] << " detected" << endl;
			if (gpVideoSrf == NULL) {
				(void)initSDL();
			}
			printf("GL Vendor: '%s' \n", glGetString( GL_VENDOR ));
			printf("GL Renderer: '%s' \n", glGetString( GL_RENDERER ));
			printf("GL Version: '%s' \n", glGetString( GL_VERSION ));
			printf("GL Extensions: '%s' \n", glGetString( GL_EXTENSIONS ));
		} else
		if (strcmp( argv[counter], "--server" ) == 0) {
			cout << "<OPTION> " << argv[counter] << " detected" << endl;
			gboolServerMode = true;
		} else
		if (strcmp( argv[counter], "--homedir" ) == 0) {
			cout << "<OPTION> " << argv[counter] << " detected" << endl;
			if (++counter < argc)
				gsHomeDir = argv[counter];
			else
				gsHomeDir = "";
			formatHomeDir();
			cout << "<OPTION> HomeDir is: \"" << gsHomeDir << "\"" << endl;
		}
		else {
			cout << "Unknown option: [" << argv[counter] << "]" << endl;
			cout << "Usage: " << argv[0]
			     << " [--no-dl] [--gl-version] [--homedir newHomePath] [--server]"
				 << endl << endl;
			cout << "Warning: any command line switch will overwrite the config file settings"
			     << endl;
			exit(-1);
		}
	} // while
}


   /*=====================================================================*/
void displaySplash()
{
	#define OC_SPLASH_CLEAR_COLOR		.15, .15, .3, 1.0

	glClearColor( OC_SPLASH_CLEAR_COLOR );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	gpRenderer->DisplaySplash();
}


   /*=====================================================================*/
void displayStatus( const string & str )
{
	uint x, y;

	displaySplash();

// Center the text on the screen
//	x = (gpVideoSrf->w - 512)/2 + (512 - str.size()*10)/2;
	x = (gpVideoSrf->w - str.size()*10)/2;
	y = (gpVideoSrf->h - 140) / 2;
	gpRenderer->DisplayText( x, y, OC_BLUE_COLOR, str );
	SDL_GL_SwapBuffers();
}


   /*=====================================================================*/
int serverMode()
{
	static SDL_Event event;

// Initialize the video system in order to capture Ctrl-C !
	SDL_Init(SDL_INIT_VIDEO);

// Start server and listen to the standard port
	gpNetworking = new Networking();
	boolQuit = (gpNetworking->StartServer() == OC_NET_OK) ? false : true;

	while (!boolQuit) {
		cout << ".";
		(void)gpNetworking->ProcessServerData();
		SDL_Delay( 50 );
		cout.flush();

		while( SDL_PollEvent( &event ) ) {
			switch( event.type ) {
			case SDL_QUIT:
			// Handle quit requests (like Ctrl-c).
				cout << endl << "Quit requested, stoping OpenCity ZeN server..." << endl;
				boolQuit = true;
				break;
			}
		}
	}

	(void)gpNetworking->StopServer();
	delete gpNetworking;

	SDL_Quit();					// WARNING: Calls free() on an invalid pointer. Detected by glibc

	return 0;
}


   /*=====================================================================*/
int clientMode()
{
	int errCode;

// Initialize SDL
	errCode = initSDL();
	if (errCode != 0) {
		return errCode;
	}

	
// set the window's caption
	SDL_WM_SetCaption( PACKAGE " " VERSION, NULL );
	SDL_WM_SetIcon( IMG_Load(ocHomeDirPrefix("graphism/icon/OpenCity32.png").c_str()), NULL );


// Create the mutex first
	gpmutexSim = SDL_CreateMutex();


// Create the global renderer in order to use its text rendering functions
	gpRenderer = new Renderer( uiCityWidth, uiCityHeight, gboolUseDisplayList );

/* debug
// Test font
	glClearColor( OC_CLEAR_COLOR );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	gpRenderer->DisplayText( 10, 50, OC_WHITE_COLOR, "Hello world" );
	SDL_GL_SwapBuffers();
	SDL_Delay( 3000 );
*/


// AudioManager's initialization
	displayStatus( "Looking for GPU freezing system... ");
	gpAudioMgr = new AudioManager();

	if ( gpAudioMgr == NULL ) {
		OPENCITY_FATAL( "Error while creating the audio manager" );
		return -16;
	} else
	if ( (gboolUseAudio == true)
	  && (gpAudioMgr->OpenAudio() != OC_ERR_FREE))
	{
	// try to open the audio device IF we have successfully created an audio object
		OPENCITY_INFO( "Audio open error ! OpenCity continues happily." );
	}


// Load musics and sounds
	displayStatus( "Warming up central processing unit...");
	gpAudioMgr->LoadMusicList( ocHomeDirPrefix(OC_MUSIC_LIST_FILENAME) );
	OPENCITY_INFO( "Loaded " << gpAudioMgr->GetNumberMusic() << " musics." );
	gpAudioMgr->LoadSoundList( ocHomeDirPrefix(OC_SOUND_LIST_FILENAME), ocHomeDirPrefix("") );
	OPENCITY_INFO( "Loaded " << gpAudioMgr->GetNumberSound() << " sounds." );


// Create the other required global managers
	displayStatus( "Activating embedded GPS...");
	gpMapMgr = new Map( uiCityWidth, uiCityHeight );

	displayStatus( "Calibrating earthquake subsystem...");
	gpGraphicMgr = new GraphicManager();

	displayStatus( "Shaking DNA mixer thread...");
	gpPropertyMgr = new PropertyManager();

	displayStatus( "Mounting intergalactic hyperlink ...");
	gpNetworking = new Networking();

	displayStatus( "Initializing the particle handler ...");
	gpMoveMgr = new MovementManager( gpGraphicMgr, gpMapMgr );


// the pointer of our new city
	City* pNewCity = new City( gboolUseDisplayList, uiCityWidth, uiCityHeight );
	if (pNewCity == NULL) {
		OPENCITY_FATAL( "Error while creating new city" );
		return (-15);
	}
	else {
		displayStatus( "Almost done...");
// debug SDL_Delay( 5000 );

	// FIXME: buggy MAS
	/*
	// Create the necessary classes for the Multi-Agent System
		gpKernel = new Kernel();
		gpEnvironment = new Environment(
			uiCityWidth, uiCityHeight, pNewCity->GetLayer( BUILDING_LAYER ), gpKernel );

		new AgentPolice(*gpKernel, *gpEnvironment, 1, 2);
    	new AgentPolice(*gpKernel, *gpEnvironment, 3, 4);
		new AgentDemonstrator(*gpKernel, *gpEnvironment, 4, 2);
	*/

		while (!boolQuit) {
		// running the city at the LAST_SPEED (default parameter)
			ocProcessSDLEvents();
			pNewCity->Run();
			//gpKernel->live();

#undef OC_PRINT_FPS
#ifndef OC_PRINT_FPS
			SDL_Delay( guiMsPerFrame );
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
			SDL_Delay( guiMsPerFrame );
#endif
		}

		//delete gpEnvironment;
		//delete gpKernel;
	}


// Close the network connection
	gpNetworking->Close();

// WARNING: the deleting/creating order is very important !
	delete pNewCity;

	delete gpMoveMgr;
	delete gpNetworking;
	delete gpPropertyMgr;
	delete gpGraphicMgr;
	delete gpMapMgr;

// close the audio device then delete the audio manager
	gpAudioMgr->CloseAudio();
	delete gpAudioMgr;

	delete gpRenderer;

// delete the simulators' mutex now
	SDL_DestroyMutex( gpmutexSim );

//	SDL_FreeSurface( gpVideoSrf ); // This is not recommended by SDL documentation
	gpVideoSrf = NULL;

	SDL_Quit();					// WARNING: Calls free() on an invalid pointer. Detected by glibc
	return errCode;
}


   /*=====================================================================*/
void printCopyright() {
// Output the copyright text
	cout << "Welcome to " << PACKAGE << " version " << VERSION << endl;
	cout << "Copyright (C) by Duong-Khang NGUYEN. All rights reserved." << endl;
	cout << "   email: neoneurone @ users sf net" << endl << endl;

	cout << "This program is released under the terms of" << endl;
	cout << "GNU General Public License (See the COPYING file for more details)" << endl << endl;

	cout << "Starting ..." << endl << endl;
}


   /*=====================================================================*/
/** Return the save directory.
	\return The pointer to the absolute directory. The caller must free
the pointer if it's not used anymore.
*/
char* findSaveDir()
{
	char* ret = NULL;

#ifndef WIN32
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
/** Read the OpenCity's main config file "opencity.conf"
	\return 0 if OK, -1 otherwise
*/
int readConfig()
{
	int retVal = 0;
	BrInitError brError;
	char* pTemp = NULL;
	Conf* pConf = new Conf();


// IF the homedir is not set THEN try to get it from BinReloc routines
	if (gsHomeDir == "") {
	// Init the BinReloc routines
		if (br_init(&brError) != 1) {
			OPENCITY_INFO(
				"The initialization of BinReloc routines has failed." << endl
				 << "The error was: " << brError
			);
		}
		else {
		// Construct the datadir from the prefix
			pTemp = br_find_prefix( PREFIX );
			gsHomeDir = pTemp;
			gsHomeDir += "/share/";
			gsHomeDir += PACKAGE;
			free(pTemp);
			formatHomeDir();
		}
	}

// IF the save directory is not set the find it
	if (gsSaveDir == "") {
		pTemp = findSaveDir();
		gsSaveDir = pTemp;
		free(pTemp);
#ifndef WIN32
		gsSaveDir += "/.OpenCity/";
		mkdir( gsSaveDir.c_str(), 0755 );
#else
	// Win32 uses \ as directory separtor
		gsSaveDir += "\\OpenCity\\";
        CreateDirectory( gsSaveDir.c_str(), NULL );		
    // Replace \ by /
	    string::size_type pos;
	    while ( (pos = gsSaveDir.find( '\\' )) != gsSaveDir.npos ) {
		    gsSaveDir.replace( pos, 1, "/" );
		}
#endif
	}


// Now try to open the config file then read it
	OPENCITY_INFO( 
		"Reading config file: \"" << ocHomeDirPrefix(OC_CONFIG_FILE_FILENAME) << "\""
	);
	if (pConf->Open(ocHomeDirPrefix(OC_CONFIG_FILE_FILENAME)) == OC_ERR_FREE) {
		bool bValue;
		OC_LINT liValue;
		if (pConf->GetBool("UseDisplayList", bValue) == OC_ERR_FREE)
			gboolUseDisplayList = bValue;
		if (pConf->GetBool("UseAudio", bValue) == OC_ERR_FREE)
			gboolUseAudio = bValue;
		if (pConf->GetBool("FullScreen", bValue) == OC_ERR_FREE)
			gboolFullScreen = bValue;
		if (pConf->GetLint("CityWidth", liValue) == OC_ERR_FREE)
			uiCityWidth = liValue;
		if (pConf->GetLint("CityHeight", liValue) == OC_ERR_FREE)
			uiCityHeight = liValue;
		if (pConf->GetLint("MsPerFrame", liValue) == OC_ERR_FREE)
			guiMsPerFrame = liValue;

		gsZenServer = pConf->GetValue("ZenServer", "localhost");

		pConf->Close();
		retVal = 0;
	}
	else {
		retVal = -1;
	}

	delete pConf;
	return retVal;
}


   /*=====================================================================*/
#if defined(WIN32) || defined(_WIN32)
extern "C"
#endif
int main(int argc, char *argv[])
{
	int returnCode;

// Print out the copyright
	printCopyright();

// Parse the command-line options
	parseArg( argc, argv );

// Read the main config file
	if (readConfig() != 0) {
		OPENCITY_FATAL(
			"The main config file \"opencity.conf\" has not been found." << endl
			<< "Try to specify the home directory with ""--homedir""." << endl
			<< "For example:" << endl
			<< "    opencity --homedir \"/absolute/path/to/opencity/data\"" << endl
			<< "or" << endl
			<< "    opencity --homedir \"../relative/path/to/opencity/data\"" << endl
		);
		exit(OC_OPENCITY_CONF_NOT_FOUND);
	}

// Initialization of global variables
	uipCurrentUI = NULL;
	gfMsSimDelayMax = log10((OC_FLOAT)uiCityWidth*uiCityHeight + 1) * OC_MS_GLOBAL_LOG_FACTOR;

// Initialize the random number generator
	srand( time(NULL) );

// Launch either client or server mode
	if (gboolServerMode == true)
		returnCode = serverMode();
	else
		returnCode = clientMode();

	return returnCode;
}


   /*=====================================================================*/
   /*                       GLOBAL       FUNCTIONS                        */
   /*=====================================================================*/
string
ocHomeDirPrefix( const string & s )
{
	return gsHomeDir + s;
}


   /*=====================================================================*/
string
ocSaveDirPrefix( const string & s )
{
	return gsSaveDir + s;
}
























