/***************************************************************************
						zen.cpp  -  description
							-------------------
	project              : OpenCity
	codename             : Zen server
	begin                : 26th nov, 2006
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


#include "zen.h"

#include "city.h"				// The heart of the project
#include "conf.h"				// Parser for .conf file
#include "agentpolice.h"		// MAS testing
#include "agentdemonstrator.h"

#include "globalvar.h"			// Global settings variable: gVars

#include "SDL_image.h"
#include "binreloc.h"			// BinReloc routines from AutoPackage
#include "tinyxml/tinyxml.h"

#include <cmath>				// For log10
#include <cstdlib>				// For getenv


   /*=====================================================================*/
   /*                        GLOBAL    VARIABLES                          */
   /*=====================================================================*/
	GlobalVar gVars;


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
	#define OC_WINDOW_POS_X			20
	#define OC_WINDOW_POS_Y			20
	#define OC_WINDOW_WIDTH			750
	#define OC_WINDOW_HEIGHT		560
	#define OC_WINDOW_BPP_DEFAULT	32				// OC uses this by default
	#define OC_WINDOW_BPP_16		16
	#define OC_FULLSCREEN_WIDTH		1024
	#define OC_FULLSCREEN_HEIGHT	768

// Exit code
	#define OC_CONFIG_NOT_FOUND		-1
	#define OC_CONFIG_PARSE_ERROR	-2

// Settings file
	#define OC_CONFIG_FILE_FILENAME	"config/opencity.xml"

// Others macros
	#define OC_WINDOW_NAME PACKAGE VERSION
	#define OC_PROGRAM_NAME			"OpenCity ZeN server application"



   /*=====================================================================*/
   /*                         LOCAL    VARIABLES                          */
   /*=====================================================================*/
/// The current user interface is pointed by this pointer
	static UI * uipCurrentUI		= NULL;

/// Set to true when the user request to quit the program
	static bool boolQuit			= false;

/// Static so that the others can not access this
	static string sHomeDir			= "";
	static string sSaveDir			= "";


   /*=====================================================================*/
void ocActive( const SDL_ActiveEvent & e)
{
	OPENCITY_DEBUG( "Active event received" );

	if (e.state & SDL_APPACTIVE) {
		gVars.gboolActive = (e.gain == 1);
	}
}


   /*=====================================================================*/
void ocQuit( const int quit_code )
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
		case SDL_ACTIVEEVENT:
			ocActive( event.active );
			break;

		case SDL_QUIT:
		// Handle quit requests (like Ctrl-c).
			cout << "Quit requested, stoping " << OC_PROGRAM_NAME << "..." << endl;
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
void formatHomeDir()
{
    string::size_type pos;

	if (sHomeDir.size() > 0) {
    // Delete all quotes "
	    while ( (pos = sHomeDir.find( '\"' )) != sHomeDir.npos ) {
		    sHomeDir.erase( pos );
		}
    // Append the "/" to HOMEDIR    
		if (sHomeDir[ sHomeDir.size()-1 ] != '/')
			sHomeDir += '/';
	}
	else {
		sHomeDir = "/";
	}
}


   /*=====================================================================*/
void parseArg(int argc, char *argv[])
{
	int counter;

	counter = 0;

	while (++counter < argc) {
		if (strcmp( argv[counter], "--homedir" ) == 0) {
			cout << "<OPTION> " << argv[counter] << " detected" << endl;
			if (++counter < argc)
				sHomeDir = argv[counter];
			else
				sHomeDir = "";
			formatHomeDir();
			cout << "<OPTION> HomeDir is: \"" << sHomeDir << "\"" << endl;
		}
		else {
			cout << "Unknown option: [" << argv[counter] << "]" << endl;
			cout << "Usage: " << argv[0]
			     << " [--homedir newHomePath]"
				 << endl << endl;
			cout << "Warning: any command line switch will overwrite the config file settings"
			     << endl;
			exit(-1);
		}
	} // while
}


   /*=====================================================================*/
void displayStatus( const string & str )
{
	cout << str << endl;
}


   /*=====================================================================*/
int serverMode()
{
// Initialize the video system in order to capture Ctrl-C !
	SDL_Init(SDL_INIT_VIDEO);

// Create the mutex first
	gVars.gpmutexSim = SDL_CreateMutex();

// Create the global renderer in order to use its text rendering functions
//	gVars.gpRenderer = new Renderer( gVars.guiCityWidth, gVars.guiCityLength );

// AudioManager's initialization
//	displayStatus( "Looking for GPU freezing system... ");
//	gVars.gpAudioMgr = new AudioManager();

// Create the other required global managers
	displayStatus( "Activating embedded GPS...");
	gVars.gpMapMgr = new Map( gVars.guiCityWidth, gVars.guiCityLength );

//	displayStatus( "Calibrating earthquake subsystem...");
//	gVars.gpGraphicMgr = new GraphicManager();

	displayStatus( "Shaking DNA mixer thread...");
	gVars.gpPropertyMgr = new PropertyManager();

	displayStatus( "Mounting intergalactic hyperlink ...");
	gVars.gpNetworking = new Networking();

	displayStatus( "Initializing the particle handler ...");
	gVars.gpMoveMgr = new MovementManager( gVars.gpGraphicMgr, gVars.gpMapMgr );


// the pointer of our new city
	City* pNewCity = new City( gVars.guiCityWidth, gVars.guiCityLength, 2, false );
	if (pNewCity == NULL) {
		OPENCITY_FATAL( "Error while creating new city" );
		return (-15);
	}
	else {
		boolQuit = (gVars.gpNetworking->StartServer() == OC_NET_OK) ? false : true;
		if (!boolQuit)
			displayStatus( "Online world conquero activated");
		else
			displayStatus( "Online world conquero activation has failed" );

		while (!boolQuit) {
		// Run the city at the LAST_SPEED (default parameter)
			cout << ".";
			(void)gVars.gpNetworking->ProcessServerData();
			ocProcessSDLEvents();
			pNewCity->Run();

			cout.flush();
			SDL_Delay( gVars.guiMsPerFrame );
		}
	}

// Stop the zen server and close the network connection
	(void)gVars.gpNetworking->StopServer();
	gVars.gpNetworking->Close();

// WARNING: the deleting/creating order is very important !
	delete pNewCity;

	delete gVars.gpMoveMgr;
	delete gVars.gpNetworking;
	delete gVars.gpPropertyMgr;
//	delete gVars.gpGraphicMgr;
	delete gVars.gpMapMgr;

// Close the audio device then delete the audio manager
//	gVars.gpAudioMgr->CloseAudio();
//	delete gVars.gpAudioMgr;

//	delete gVars.gpRenderer;

// Delete the simulators' mutex now
	SDL_DestroyMutex( gVars.gpmutexSim );

	gVars.gpVideoSrf = NULL;
	SDL_Quit();					// WARNING: Calls free() on an invalid pointer. Detected by glibc
	return 0;
}


   /*=====================================================================*/
void printCopyright() {
// Output the copyright text
	cout << "Welcome to " << PACKAGE << " version " << VERSION << endl;
	cout << "Copyright (C) by Duong-Khang NGUYEN. All rights reserved." << endl;
	cout << "   web  : http://www.opencity.info" << endl;
	cout << "   email: neoneurone @ users sf net" << endl << endl;

	cout << "This program is released under the terms of" << endl;
	cout << "GNU General Public License (See the COPYING file for more details)" << endl << endl;

	cout << "Starting " << OC_PROGRAM_NAME << "..." << endl << endl;
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
/** Detect and set the homedir and the savedir using BinReloc library
and win32 standard function
*/
void detectProgramPath()
{
	char* pTemp = NULL;
	BrInitError brError;

// IF the homedir is not set THEN try to get it from BinReloc routines
	if (sHomeDir == "") {
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
			sHomeDir = pTemp;
			sHomeDir += "/share/";
			sHomeDir += PACKAGE;
			free(pTemp);
			formatHomeDir();
		}
	}

// IF the save directory is not set the find it
	if (sSaveDir == "") {
		pTemp = findSaveDir();
		sSaveDir = pTemp;
		free(pTemp);
#ifndef WIN32
		sSaveDir += "/.OpenCity/";
		mkdir( sSaveDir.c_str(), 0755 );
#else
	// Win32 uses \ as directory separtor
		sSaveDir += "\\OpenCity\\";
        CreateDirectory( sSaveDir.c_str(), NULL );		
    // Replace \ by /
	    string::size_type pos;
	    while ( (pos = sSaveDir.find( '\\' )) != sSaveDir.npos ) {
		    sSaveDir.replace( pos, 1, "/" );
		}
#endif
	}
}


   /*=====================================================================*/
/** Read the OpenCity's main settings file "opencity.xml"
	\return "" if OK, otherwise the error description
		0: if OK
		OC_CONFIG_NOT_FOUND: the config file has not been found
		OC_CONFIG_PARSE_ERROR: the was a parse error
*/
string readSettings()
{
	string errorString = "";
	TiXmlDocument settings;

// Now try to open the config file then read it
	OPENCITY_INFO(
		"Reading XML config file: \"" << ocHomeDirPrefix(OC_CONFIG_FILE_FILENAME) << "\""
	);

// Load the settings file
	string fn = ocHomeDirPrefix(OC_CONFIG_FILE_FILENAME);
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
	int i = 0;
	while (pElement != NULL)
	{
// Debug
//		cout << i++ << "||" << *pElement << std::endl;
	// "city" element, read the city's size
		if (pElement->ValueStr() == "city") {
			TiXmlElement* pChild = pElement->FirstChildElement();
			while (pChild != NULL) {
				cout << i++ << "||" << *pChild << std::endl;
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
void initGlobalVar()
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
#if defined(WIN32) || defined(_WIN32)
extern "C"
#endif
int main(int argc, char *argv[])
{
// Initialize the global settings variable to the default values
	initGlobalVar();

// Print out the copyright
	printCopyright();

// Parse the command-line options
	parseArg( argc, argv );

// Detect the main path: sHomeDir and sSaveDir
	detectProgramPath();

// Read the application settings from the XML settings file
	string errorDesc = readSettings();
	if (errorDesc != "") {
		OPENCITY_FATAL(
			"The was an error while loading the settings file: \"" << errorDesc << "\"" << endl
			<< "If the main config file \"" << OC_CONFIG_FILE_FILENAME << "\" has not been found then" << endl
			<< "try to specify the home directory with ""--homedir""." << endl
			<< "For example:" << endl
			<< "    " << argv[0] << " --homedir \"/absolute/path/to/opencity/data\"" << endl
			<< "or" << endl
			<< "    " << argv[0] << " --homedir \"../relative/path/to/opencity/data\"" << endl
		);
		exit(OC_CONFIG_NOT_FOUND);
	}

// Initialization of global variables
	uipCurrentUI = NULL;
	gVars.gfMsSimDelayMax = log10((OC_FLOAT)gVars.guiCityWidth*gVars.guiCityLength + 1) * OC_MS_GLOBAL_LOG_FACTOR;

// Initialize the random number generator
	srand( time(NULL) );

// Launch the server
	int returnCode = serverMode();

	return returnCode;
}


   /*=====================================================================*/
   /*                       GLOBAL       FUNCTIONS                        */
   /*=====================================================================*/
string
ocHomeDirPrefix( const string & s )
{
	return sHomeDir + s;
}


   /*=====================================================================*/
string
ocSaveDirPrefix( const string & s )
{
	return sSaveDir + s;
}


   /*=====================================================================*/
string ocStrVersion()
{
	ostringstream oss;

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






















