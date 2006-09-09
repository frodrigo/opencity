/***************************************************************************
                          city.cpp  -  description
          $Id$
                             -------------------
    begin                : mer mai 28 2003
    copyright            : (C) 2003-2005 by Duong-Khang NGUYEN
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

#include "city.h"

#include "map.h"

#include "pathfinder.h"				// Vehicles management
#include "vehicle.h"
#include "movementmanager.h"

#include "mainsim.h"				// simulator

#include "graphicmanager.h"			// Building management
#include "propertymanager.h"
#include "structure.h"
#include "buildinglayer.h"
#include "renderer.h"

#include "guibutton.h"				// GUI management
#include "guicontainer.h"

#include "audiomanager.h"			// Audio management
#include "networking.h"				// Networking support

#include "environment.h"			// MAS's environement class
#include "agentpolice.h"
#include "agentdemonstrator.h"
#include "agentrobber.h"

#include <sstream>					// For text output with data conversion

#define OC_ACTION_FACTOR 10


   /*=====================================================================*/
   /*                        GLOBAL    VARIABLES                          */
   /*=====================================================================*/
extern SDL_mutex* gpmutexSim;			// The mutex that all the simulators depend on

extern AudioManager* gpAudioMgr;		// global audiomanger, see main.cpp
extern Map* gpMapMgr;					// global map height manager
extern GraphicManager* gpGraphicMgr;	// global graphic manager
extern PropertyManager* gpPropertyMgr;	// global property manager
extern Renderer* gpRenderer;			// global renderer
extern Networking* gpNetworking;		// global networking support class
extern PathFinder* gpPathFinder;		// global pathfinder class
extern MovementManager* gpMoveMgr;		// global movement manager

extern Kernel* gpKernel;				// global MAS Kernel
extern Environment* gpEnvironment;		// global MAS's environement class

extern uint guiMsPerFrame;			// Global milliseconds per frame
extern string gsZenServer;				// The server name, ex: "localhost"


   /*=====================================================================*/
City::City(
	const bool & rcUseDL,
	const uint & width,
	const uint & height,
	const OC_DATE & foundedDate,
	const int & difficulty ):
strCityName("OpenCity"),
iDifficulty( difficulty ),
strFileName(""),
cityFoundedDate( foundedDate ),
_liCityFund( OC_FUND_START ),
boolModified( false ),

_uiDay( 1 ),
_uiMonth( 1 ),
_uiYear( 0 ),

_cTool('N'),
_uiWidth( width ),
_uiHeight( height ),

boolUseDisplayList( rcUseDL ),
boolLMBPressed( false ),
enumCurrentLayer( BUILDING_LAYER ),
enumCurrentSpeed( NORMAL_SPEED ),
enumCurrentTool( OC_NONE )
{
	OPENCITY_DEBUG( "City ctor - default parameters" );

// set the windows's WH
	iWinWidth = SDL_GetVideoSurface()->w;
	iWinHeight = SDL_GetVideoSurface()->h;

// registering our call-back interface for SDL events' treatment
	ocSetNewUI( this );

// keyboard bool table's initialization
	for (int i = 0; i < KEY_NUMBER; i++)
		this->booltabKeyPressed[i] = false;

// Layers' initialization
	ptabLayer[ BUILDING_LAYER ] = new BuildingLayer( *this );
	gpMapMgr->SetLayer( ptabLayer[BUILDING_LAYER] );

// Pathfinder initialization
	gpPathFinder = new PathFinder(
		gpmutexSim,
		(BuildingLayer*)ptabLayer[ BUILDING_LAYER ],
		gpMapMgr,
		_uiWidth, _uiHeight );

// Debug toolcircle
	boolPathGo = false;
	uiPathStartW = 0; uiPathStartH = 0;
	uiPathStopW = 0; uiPathStopH = 0;
	pctrPath = new GUIContainer( 100, 100, 30, 30 );
	pbtnPathStart = new GUIButton( 20,  20,  30, 30, ocHomeDirPrefix( "graphism/gui/raise" ));
	pbtnPathStop1 = new GUIButton( 60,  0,   30, 30, ocHomeDirPrefix( "graphism/gui/lower" ));
	pbtnPathStop2 = new GUIButton( 100, 20,  30, 30, ocHomeDirPrefix( "graphism/gui/lower" ));
	pbtnTestBuilding = new GUIButton(  20,  80, 30, 30, ocHomeDirPrefix( "graphism/gui/residential" ));
	pctrPath->Add( pbtnPathStart );
	pctrPath->Add( pbtnPathStop1 );
	pctrPath->Add( pbtnPathStop2 );
	pctrPath->Add( pbtnTestBuilding );
	pvehicle = NULL;
//	pMoveMgr = new MovementManager( gpGraphicMgr, gpMapMgr );

// Simulators' initialization
	_CreateSimulator();

// create the GUI
	_CreateGUI();
}


   /*=====================================================================*/
City::City( const int & rciFileHandle ){}



   /*=====================================================================*/
City::~City(  ){
	OPENCITY_DEBUG( "City dtor" );

// delete the GUI
	_DeleteGUI();

// delete all the simulators
	_DeleteSimulator();

// testing, delete the pathfinder
	delete pctrPath;
	delete pbtnTestBuilding;
	delete pbtnPathStart;
	delete pbtnPathStop1;
	delete pbtnPathStop2;
	delete gpPathFinder;
	gpPathFinder = NULL;
//	delete pvehicle; // not needed, it is done automatically by deleting pMoveMgr
//	delete pMoveMgr;

// delete only the BUILDING_LAYER instead of delete [] ptabLayer
	delete ptabLayer[ BUILDING_LAYER ];
// this must be done AFTER deleting the layers
//	delete gpMapMgr;
}


   /*=====================================================================*/
void
City::SaveTo( std::fstream& rfs )
{
	OPENCITY_DEBUG( __PRETTY_FUNCTION__ << "saving" );

	rfs << _liCityFund << std::ends;
	rfs << _uiDay << std::ends;
	rfs << _uiMonth << std::ends;
	rfs << _uiYear << std::ends;
	rfs << _uiWidth << std::ends;
	rfs << _uiHeight << std::ends;
}


   /*=====================================================================*/
void
City::LoadFrom( std::fstream& rfs )
{
	OPENCITY_DEBUG( __PRETTY_FUNCTION__ << "loading" );

	rfs >> _liCityFund; rfs.ignore();
	rfs >> _uiDay; rfs.ignore();
	rfs >> _uiMonth; rfs.ignore();
	rfs >> _uiYear; rfs.ignore();
	rfs >> _uiWidth; rfs.ignore();
	rfs >> _uiHeight; rfs.ignore();
}


   /*=====================================================================*/
void City::SetCurrentLayer( OPENCITY_CITY_LAYER enumNewLayer )
{}


   /*=====================================================================*/
void City::Run( OPENCITY_CITY_SPEED enumSpeed )
{
	static uint uiNumberFrame = 0;
	static ostringstream ossStatus;
	static bool boolKeyDown;
	static int iMouseX, iMouseY;

// if another speed is requested, we switch to it
	if (enumSpeed != LAST_SPEED) {
		enumCurrentSpeed = enumSpeed;
	}

// now treat keys such as: up, down, left, right etc..
	boolKeyDown = _HandleKeyPressed();

// if the mouse reach the border of the screen, translate the map
	_HandleMouseXY();

// NOTE:We can move the following part to City::uiMouseMotion
// however, in this case City::uiMouseMotion is called each time
// when the mouse moves, and this is no good
// user is dragging
	if ((this->boolLMBPressed == true)
	&& (enumCurrentTool != OC_NONE )) {
	// is the user dragging with the left mouse button ?
		if ( SDL_GetMouseState( &iMouseX, &iMouseY ) & SDL_BUTTON(1) ) {
			gpRenderer->GetSelectedWHFrom(
				iMouseX, iMouseY,
				this->uiMapW2, this->uiMapL2,
				gpMapMgr, this->ptabLayer[ enumCurrentLayer ] );

		// draw the map with the highlighted area
			gpRenderer->DisplayHighlight(
				gpMapMgr, ptabLayer[ enumCurrentLayer ],
				this->uiMapW1, this->uiMapL1,
				this->uiMapW2, this->uiMapL2,
				enumCurrentTool );

			goto cityrun_swap;
		}
	}


//cityrun_display:
// Display the screen as usual
	gpRenderer->Display( gpMapMgr, ptabLayer[ enumCurrentLayer ] );


cityrun_swap:
// Display build preview
	_BuildPreview();

// Display the city's funds
	ossStatus.str("");
	ossStatus << _liCityFund << " @";
	gpRenderer->DisplayText( 10, this->iWinHeight-15, OC_WHITE_COLOR, ossStatus.str() );
// Display the R value
	ossStatus.str("");
	ossStatus << _pMSim->GetValue(MainSim::OC_MICROSIM_RES);
	gpRenderer->DisplayText( 120, this->iWinHeight-15, OC_GREEN_COLOR, ossStatus.str() );
// display the C value
	ossStatus.str("");
	ossStatus << _pMSim->GetValue(MainSim::OC_MICROSIM_COM);
	gpRenderer->DisplayText( 180, this->iWinHeight-15, OC_BLUE_COLOR, ossStatus.str() );
// display the I value
	ossStatus.str("");
	ossStatus << _pMSim->GetValue(MainSim::OC_MICROSIM_IND);
	gpRenderer->DisplayText( 240, this->iWinHeight-15, OC_YELLOW_COLOR, ossStatus.str() );
// display the E value
	ossStatus.str("");
	ossStatus << _pMSim->GetValue(MainSim::OC_MICROSIM_ELE);
	gpRenderer->DisplayText( 300, this->iWinHeight-15, OC_PINK_COLOR, ossStatus.str() );

// display the tool
	ossStatus.str("");
	ossStatus << "Tool: " << _cTool;
	gpRenderer->DisplayText( 550, this->iWinHeight-15, OC_WHITE_COLOR, ossStatus.str() );

// display the date
	ossStatus.str("");
	ossStatus << _uiDay << "/" << _uiMonth << "/" << _uiYear;
	gpRenderer->DisplayText( 650, this->iWinHeight-15, OC_WHITE_COLOR, ossStatus.str() );

// testing pathfinding, displaying vehicle
	gpMoveMgr->Move();
	gpMoveMgr->Display();

// FIXME: buggy MAS environment
//	gpEnvironment->displayAgent();

// display the current container
	pctr->Display();

// display everything now
	SDL_GL_SwapBuffers();

//cityrun_return:
	if ( ++uiNumberFrame*guiMsPerFrame > OC_MS_PER_DAY ) {
	// next day
		if ( ++_uiDay > 30 ) {
		// next month
			_uiDay = 1;
			if ( ++_uiMonth > 12 ) {
			// next year
				_uiMonth = 1;
				_uiYear++;
				_DoBill( OC_INCOME );
			}
			else {
				_DoBill( OC_MAINTENANCE_COST );
			}
		}
		uiNumberFrame = 0;

	// auto play background music
		if (gpAudioMgr->PlayingMusic() == false) {
			gpAudioMgr->PlayNextMusic();
		}
	}

}


   /*=====================================================================*/
Layer*
City::GetLayer( OPENCITY_CITY_LAYER enumLayer ) const
{
	return this->ptabLayer[ enumLayer ];
}


   /*=====================================================================*/
const uint &
City::cityGetWidth() const
{
	return _uiWidth;
}


   /*=====================================================================*/
const uint &
City::cityGetHeight() const
{
	return _uiHeight;
}



   /*=====================================================================*/
   /*                    BASE CLASS 'UI' IMPLEMENTATION                   */
   /*=====================================================================*/



   /*=====================================================================*/
void City::uiKeyboard(
	const SDL_KeyboardEvent & rcsSDLKeyboardEvent )
{
//	OPENCITY_DEBUG( "Keydown event received" );

// SDL_KEYDOWN or SDL_PRESSED
	if (rcsSDLKeyboardEvent.type == SDL_KEYDOWN) {
	// test if ALT is pressed
		if (rcsSDLKeyboardEvent.keysym.mod & KMOD_ALT) {
			this->booltabKeyPressed[KEY_ALT] = true;
		}

	// key symbols treatment
		switch (rcsSDLKeyboardEvent.keysym.sym) {
		case SDLK_PAGEUP:
			this->booltabKeyPressed[KEY_PAGEUP] = true;
			break;
		case SDLK_PAGEDOWN:
			this->booltabKeyPressed[KEY_PAGEDOWN] = true;
			break;

		case SDLK_UP:
			this->booltabKeyPressed[KEY_UP] = true;
			break;
		case SDLK_DOWN:
			this->booltabKeyPressed[KEY_DOWN] = true;
			break;
		case SDLK_RIGHT:
			this->booltabKeyPressed[KEY_RIGHT] = true;
			break;
		case SDLK_LEFT:
			this->booltabKeyPressed[KEY_LEFT] = true;
			break;

	// test networking support, connect to the localhost
		case SDLK_t:
			OPENCITY_NET_CODE netCode;
			netCode = gpNetworking->Open( gsZenServer );
			switch (netCode) {
				case OC_NET_CLIENT_CONNECTED:
					OPENCITY_DEBUG( "I'm already connected to a server" );
					break;
				case OC_NET_CLIENT_ACCEPTED:
					OPENCITY_DEBUG( "Connection request accepted" );
					break;
				case OC_NET_CLIENT_REJECTED:
					OPENCITY_DEBUG( "Connection request rejected. Server full ?" );
					break;
				default:
					OPENCITY_DEBUG( "I couldn't connect to the server " );
			}
			break;

		case SDLK_n:  // set the tool to "None"
			enumCurrentTool = OC_NONE;
			_cTool = 'N';
			break;
		case SDLK_r:  // set tool for "zone residential"
			enumCurrentTool = OC_ZONE_RES;
			_cTool = 'R';
			break;
		case SDLK_c:  // set tool for "zone commercial"
			enumCurrentTool = OC_ZONE_COM;
			_cTool = 'C';
			break;
		case SDLK_i:  // set tool for "zone industrial"
			enumCurrentTool = OC_ZONE_IND;
			_cTool = 'I';
			break;

		case SDLK_p:  // set tool for "building road"
			enumCurrentTool = OC_BUILD_ROAD;
			_cTool = 'P';
			break;
		case SDLK_l:  // set tool for building electric lines
			enumCurrentTool = OC_BUILD_ELINE;
			_cTool = 'L';
			break;
		case SDLK_e:  // set tool for building electric plants
			enumCurrentTool = OC_BUILD_EPLANT;
			_cTool = 'E';
			break;

		case SDLK_u:  // height up
			enumCurrentTool = OC_HEIGHT_UP;
			_cTool = 'U';
			break;
		case SDLK_d:  // height down
			enumCurrentTool = OC_HEIGHT_DOWN;
			_cTool = 'D';
			break;
		case SDLK_q:		//query tool
			enumCurrentTool = OC_QUERY;
			_cTool = 'Q';
			break;


		case SDLK_g:  // toggle grid on/off
			gpRenderer->ToggleGrid();
			break;
		case SDLK_k:  // toggle compass on/off
			gpRenderer->ToggleCompass();
			break;
		case SDLK_f:  // toggle wireframe on/off
			gpRenderer->ToggleWireFrame();
			break;
		case SDLK_o:  // toggle projection mode
			gpRenderer->ToggleProjection();
			break;


		case SDLK_INSERT: // zoom in
			this->booltabKeyPressed[KEY_INSERT] = true;
			break;
		case SDLK_DELETE: // zoom out
			this->booltabKeyPressed[KEY_DELETE] = true;
			break;

		case SDLK_x: // destroy
			enumCurrentTool = OC_DESTROY;
			_cTool = 'X';
			break;


	// manipulating the music player
		case SDLK_b:
			gpAudioMgr->PlayNextMusic();
			break;

		case SDLK_z:
			gpAudioMgr->PlayPreviousMusic();
			break;

		case SDLK_s:
			gpAudioMgr->ToggleSound();
			break;

		case SDLK_m:
			gpAudioMgr->ToggleMusic();
			break;


	// Save and load
		case SDLK_F2:
			_Save( ocHomeDirPrefix( "opencity.save" ) );
			break;

		case SDLK_F6:
			_Load( ocHomeDirPrefix( "opencity.save" ) );
			break;


		case SDLK_h:
			gpRenderer->Home();
			break;
		case SDLK_ESCAPE:
		// quit the program when ESCAPE is pressed
			ocQuit();
			break;

#ifndef NDEBUG
	// Testing PathFinder
		case SDLK_a:
			pctr->ResetAttribute(
				OC_GUIMAIN_CLICKED | OC_GUIMAIN_MOUSEOVER );
		// Avoid possible memory leak
			if ( pctr == pctrQ ) {
				pctr = NULL;
				delete pctrQ;
				pctrQ = NULL;
			}

		// Toggle Main <-> Pathfinding toolcircle
			if ( pctr == pctrPath ) {
				pctr = pctrMain;
			}
			else {
				pctr = pctrPath;
			}
			if ( pctr->IsSet( OC_GUIMAIN_VISIBLE ) == false ) {
				pctr->Set( OC_GUIMAIN_VISIBLE );
			}
			break;

	// MAS test toolcircle
		case SDLK_v:
			pctr->ResetAttribute(
				OC_GUIMAIN_CLICKED | OC_GUIMAIN_MOUSEOVER );
		// Avoid possible memory leak
			if ( pctr == pctrQ ) {
				pctr = NULL;
				delete pctrQ;
				pctrQ = NULL;
			}

		// Toggle Main <-> MAS toolcircle
			if ( pctr == pctrMAS ) {
				pctr = pctrMain;
			}
			else {
				pctr = pctrMAS;
			}
			if ( pctr->IsSet( OC_GUIMAIN_VISIBLE ) == false ) {
				pctr->Set( OC_GUIMAIN_VISIBLE );
			}
			break;
#endif

		default:
			break;
		}
	}
// SDL_KEYUP or SDL_RELEASED
	else {
	// test if ALT is released
		if (!(rcsSDLKeyboardEvent.keysym.mod & KMOD_ALT)) {
			this->booltabKeyPressed[KEY_ALT] = false;
		}

	// other key symbols treatment
		switch (rcsSDLKeyboardEvent.keysym.sym) {

		case SDLK_PAGEUP:
			this->booltabKeyPressed[KEY_PAGEUP] = false;
			break;
		case SDLK_PAGEDOWN:
			this->booltabKeyPressed[KEY_PAGEDOWN] = false;
			break;

		case SDLK_UP:
			this->booltabKeyPressed[KEY_UP] = false;
			break;
		case SDLK_DOWN:
			this->booltabKeyPressed[KEY_DOWN] = false;
			break;
		case SDLK_RIGHT:
			this->booltabKeyPressed[KEY_RIGHT] = false;
			break;
		case SDLK_LEFT:
			this->booltabKeyPressed[KEY_LEFT] = false;
			break;

		case SDLK_INSERT: // zoom in
			this->booltabKeyPressed[KEY_INSERT] = false;
			break;
		case SDLK_DELETE: // zoom out
			this->booltabKeyPressed[KEY_DELETE] = false;
			break;

		default:
			break;
		} // switch
	} // key released

}


   /*=====================================================================*/
void
City::uiMouseMotion(
	const SDL_MouseMotionEvent & rcsSDLMouseMotionEvent )
{
//	OPENCITY_DEBUG("Mouse moved");
	pctr->uiMouseMotion( rcsSDLMouseMotionEvent );
}


   /*=====================================================================*/
void
City::uiMouseButton(
	const SDL_MouseButtonEvent & rcsMBE )
{
//	OPENCITY_DEBUG("Mouse button event received" );

// Process the click concerning the GUI first
	pctr->uiMouseButton( rcsMBE );
	if ( pctr->GetClick() != 0 ) {
//debug
//cout << "clicked: " << pctr->GetClick() << endl;
		_HandleGUIClick();
		this->boolLMBPressed = false;
		return;
	}

// The user didn't click on a GUI object so we look for clicks on the map
	switch (rcsMBE.state) {
		case SDL_PRESSED: {
			this->boolLMBPressed = false;
			if ((rcsMBE.button == SDL_BUTTON_LEFT)
				&& (gpRenderer->GetSelectedWHFrom(
					rcsMBE.x, rcsMBE.y,
					this->uiMapW1, this->uiMapL1,
					gpMapMgr,
					this->ptabLayer[ enumCurrentLayer ] ) == true ))
			{
				this->boolLMBPressed = true;
//debug begin
//cout << "step one, coco " << endl;
//cout << "W: " << uiMapW1 << "/" << "H: " << uiMapL1 << endl;
//debug end
			} //if
//debug begin
//SDL_GL_SwapBuffers(); // uncomment this if you want to know how it works
//SDL_Delay( 500 );
//debug end

		// RMB (right mouse button) close/open the toolcircle
			if (rcsMBE.button == SDL_BUTTON_RIGHT) {
			// if the user has invoked "Query"
			// then we destroy it first
				if (pctr == pctrQ) {
					pctr = pctrMain;
				// enable the visible bit
				// since, it is disabled later, the main circletool
				// won't be displayed
					pctr->Set( OC_GUIMAIN_VISIBLE );
					delete pctrQ;
					pctrQ = NULL;		// YES, we need it
				}
	
				if (pctr->IsSet( OC_GUIMAIN_VISIBLE ) == true) {
					pctr->Unset( OC_GUIMAIN_VISIBLE );
				}
				else {
					pctr->SetLocation(
						rcsMBE.x - 70,
						iWinHeight - rcsMBE.y - 70 );
					pctr->Set( OC_GUIMAIN_VISIBLE );
				}
			}
	
		// Wheel button forward
			if (rcsMBE.button == 4) {
			// move right if CTRL pressed
				if (SDL_GetModState() & KMOD_CTRL)
					gpRenderer->MoveRight();
	
			// move up if SHIFT pressed
				if (SDL_GetModState() & KMOD_SHIFT)
					gpRenderer->MoveUp();
	
			// zoom in if nothing pressed
				if (!(SDL_GetModState() & (KMOD_SHIFT | KMOD_CTRL)))
					gpRenderer->ZoomIn();
			}
	
		// Wheel button backward
			if (rcsMBE.button == 5) {
			// move right if CTRL pressed
				if (SDL_GetModState() & KMOD_CTRL)
					gpRenderer->MoveLeft();
	
			// move up if SHIFT pressed
				if (SDL_GetModState() & KMOD_SHIFT)
					gpRenderer->MoveDown();
	
			// zoom in if nothing pressed
				if (!(SDL_GetModState() & (KMOD_SHIFT | KMOD_CTRL)))
					gpRenderer->ZoomOut();
			}

		break;
		} //case SDL_PRESSED

	   //-------------------------------------------------------
		case SDL_RELEASED: {
			// If Ctrl not pressed, 
			// AND dragging enabled 
			// AND mouse button was correctly released in the map
			// THEN do tool
			if (!(SDL_GetModState() & KMOD_CTRL)
				&& (this->boolLMBPressed == true)
				&& gpRenderer->GetSelectedWHFrom(
					rcsMBE.x, rcsMBE.y,
					this->uiMapW2, this->uiMapL2,
					gpMapMgr,
					this->ptabLayer[ enumCurrentLayer ] ))
			{
//debug
//cout << "W2: " << uiMapW2 << "/" << "H2: "
//     << uiMapL2 << endl;
//test pathfinding
				_TestPathfinding();
				_DoTool( rcsMBE );
			} //if
			boolLMBPressed = false;
			break;

		} //case SDL_RELEASED

	} // switch
}


   /*=====================================================================*/
void
City::uiExpose(
	const SDL_ExposeEvent & rcsSDLExposeEvent )
{
	OPENCITY_DEBUG( "Expose event received" );

	gpRenderer->Display( gpMapMgr, ptabLayer[ enumCurrentLayer ] );
	pctr->uiExpose( rcsSDLExposeEvent );

	SDL_GL_SwapBuffers();
}


   /*=====================================================================*/
void City::uiResize( const SDL_ResizeEvent & rcsSDLResizeEvent )
{
	OPENCITY_DEBUG( "Resize event received" );

	iWinWidth = rcsSDLResizeEvent.w;
	iWinHeight = rcsSDLResizeEvent.h;

//---- set the new window's size ----
	gpRenderer->SetWinSize( iWinWidth, iWinHeight );

	gpRenderer->Display( gpMapMgr, ptabLayer[ enumCurrentLayer ] );

//----- updates display -----
	SDL_GL_SwapBuffers();

// tell the containers about the event
	pctrMain->uiResize( rcsSDLResizeEvent );
	pctrL->uiResize( rcsSDLResizeEvent );
	pctrT->uiResize( rcsSDLResizeEvent );
	pctrZ->uiResize( rcsSDLResizeEvent );
	pctrG->uiResize( rcsSDLResizeEvent );
	pctrN->uiResize( rcsSDLResizeEvent );
	pctrS->uiResize( rcsSDLResizeEvent );
	pctrPath->uiResize( rcsSDLResizeEvent );
	pctrMAS->uiResize( rcsSDLResizeEvent );
}














   /*=====================================================================*/
   /*                        PRIVATE     METHODS                          */
   /*=====================================================================*/









void City::_CreateSimulator()
{
// Simulators' initialization
	_pMSim = new MainSim( gpmutexSim, (BuildingLayer*)ptabLayer[ BUILDING_LAYER ], gpMapMgr );

// now initialize simulators threads
	_pthreadMSim = SDL_CreateThread( Simulator::ThreadWrapper, _pMSim );

//testing: how can I put funcTSim into the TrafficSim class ?
//	int (*fn)(void*);
//	fn = reinterpret_cast<int (*)(void*)>(&TrafficSim::Run);
//	pthreadTSim = SDL_CreateThread( TrafficSim::Run, pTSim );

// put all the simulators' threads into RUN state
	_pMSim->Run();
}


   /*=====================================================================*/
void
City::_DeleteSimulator()
{
	int iStatus;

// put all the simulators' threads into RETURN state
	_pMSim->Return();

// wait for simulator threads to end
	SDL_WaitThread( _pthreadMSim, &iStatus );

// delete simulators at the end
	delete _pMSim;
}


   /*=====================================================================*/
void
City::_CreateGUI()
{
// GUI main toolcircle
	pbtnZ = new GUIButton(  20,  80, 30, 30, ocHomeDirPrefix( "graphism/gui/residential" ));
	pbtnS = new GUIButton( 100,  80, 30, 30, ocHomeDirPrefix( "graphism/gui/save" ));
	pbtnL = new GUIButton(  20,  20, 30, 30, ocHomeDirPrefix( "graphism/gui/power" ));
	pbtnP = new GUIButton(  60,   0, 30, 30, ocHomeDirPrefix( "graphism/gui/road" ));
	pbtnX = new GUIButton(  100, 20, 30, 30, ocHomeDirPrefix( "graphism/gui/bulldozer" ));
	pbtnG = new GUIButton(  60, 100, 30, 30, ocHomeDirPrefix( "graphism/gui/government" ));

	pctrMain = new GUIContainer( 100, 100, 30, 30 );
	pctrMain->Add( pbtnZ );
	pctrMain->Add( pbtnS );
	pctrMain->Add( pbtnL );
	pctrMain->Add( pbtnP );
	pctrMain->Add( pbtnX );
	pctrMain->Add( pbtnG );


// GUI Z toolcircle for RCI buttons
	pbtnZB = new GUIButton( 20,  80,  30, 30, ocHomeDirPrefix( "graphism/gui/back" ));
	pbtnZR = new GUIButton( 20,  20,  30, 30, ocHomeDirPrefix( "graphism/gui/residential" ));
	pbtnZC = new GUIButton( 60,  0,   30, 30, ocHomeDirPrefix( "graphism/gui/commercial" ));
	pbtnZI = new GUIButton( 100, 20,  30, 30, ocHomeDirPrefix( "graphism/gui/industrial" ));

	pctrZ = new GUIContainer( 100, 100, 30, 30 );
	pctrZ->Add( pbtnZB );
	pctrZ->Add( pbtnZR );
	pctrZ->Add( pbtnZC );
	pctrZ->Add( pbtnZI );


// GUI L toolcircle ( electric lines, electric plants )
	pbtnLB = new GUIButton( 20,  20,  30, 30, ocHomeDirPrefix( "graphism/gui/back" ));
	pbtnLL = new GUIButton( 60,  0,   30, 30, ocHomeDirPrefix( "graphism/gui/power_line" ));
	pbtnLE = new GUIButton( 100, 20,  30, 30, ocHomeDirPrefix( "graphism/gui/power_plant" ));

	pctrL = new GUIContainer( 100, 100, 30, 30 );
	pctrL->Add( pbtnLB );
	pctrL->Add( pbtnLL );
	pctrL->Add( pbtnLE );


// GUI T toolcircle ( raise, lower terrain )
	pbtnTU = new GUIButton( 20,  20,  30, 30, ocHomeDirPrefix( "graphism/gui/raise" ));
	pbtnTD = new GUIButton( 60,  0,   30, 30, ocHomeDirPrefix( "graphism/gui/lower" ));
	pbtnTB = new GUIButton( 100, 20,  30, 30, ocHomeDirPrefix( "graphism/gui/back" ));
	pbtnTX = new GUIButton( 20,  80,  30, 30, ocHomeDirPrefix( "graphism/gui/destroy" ));
	pbtnTQ = new GUIButton( 100,  80, 30, 30, ocHomeDirPrefix( "graphism/gui/query" ));

	pctrT = new GUIContainer( 100, 100, 30, 30 );
	pctrT->Add( pbtnTB );
	pctrT->Add( pbtnTU );
	pctrT->Add( pbtnTD );
	pctrT->Add( pbtnTX );
	pctrT->Add( pbtnTQ );


// GUI Gouvernement toolcircle ( park, education, hospital, police and fire )
	pbtnGB = new GUIButton(  60, 100, 30, 30, ocHomeDirPrefix( "graphism/gui/back" ));
	pbtnGP = new GUIButton(  20,  80, 30, 30, ocHomeDirPrefix( "graphism/gui/park" ));
	pbtnGE = new GUIButton( 100,  80, 30, 30, ocHomeDirPrefix( "graphism/gui/education" ));
	pbtnGH = new GUIButton(  20,  20, 30, 30, ocHomeDirPrefix( "graphism/gui/hospital" ));
	pbtnGL = new GUIButton(  60,   0, 30, 30, ocHomeDirPrefix( "graphism/gui/police" ));
	pbtnGF = new GUIButton(  100, 20, 30, 30, ocHomeDirPrefix( "graphism/gui/fire" ));

	pctrG = new GUIContainer( 100, 100, 30, 30 );
	pctrG->Add( pbtnGB );
	pctrG->Add( pbtnGP );
	pctrG->Add( pbtnGE );
	pctrG->Add( pbtnGH );
	pctrG->Add( pbtnGL );
	pctrG->Add( pbtnGF );


// Create the nature container
	pbtnNB = new GUIButton(  20,  80, 30, 30, ocHomeDirPrefix( "graphism/gui/back" ));
	pbtnNP = new GUIButton(  60, 100, 30, 30, ocHomeDirPrefix( "graphism/gui/park" ));
	pbtnNT = new GUIButton( 100,  80, 30, 30, ocHomeDirPrefix( "graphism/gui/tree" ));

	pctrN = new GUIContainer( 100, 100, 30, 30 );
	pctrN->Add( pbtnNB );
	pctrN->Add( pbtnNP );
	pctrN->Add( pbtnNT );


// Create save/load buttons and the container
	pbtnSL = new GUIButton(  20,  80, 30, 30, ocHomeDirPrefix( "graphism/gui/save_load" ));
	pbtnSS = new GUIButton(  60, 100, 30, 30, ocHomeDirPrefix( "graphism/gui/save_save" ));
	pbtnSB = new GUIButton( 100,  80, 30, 30, ocHomeDirPrefix( "graphism/gui/back" ));

	pctrS = new GUIContainer( 100, 100, 30, 30 );
	pctrS->Add( pbtnSB );
	pctrS->Add( pbtnSS );
	pctrS->Add( pbtnSL );


// MAS toolcircle
	pctrMAS = new GUIContainer( 100, 100, 30, 30 );
	pbtnMASPolice = new GUIButton( 20,  20,  30, 30, ocHomeDirPrefix( "graphism/gui/police" ));
	pbtnMASDemonstrator = new GUIButton( 60,  0,   30, 30, ocHomeDirPrefix( "graphism/gui/demonstrator" ));
	pbtnMASRobber = new GUIButton( 100, 20,  30, 30, ocHomeDirPrefix( "graphism/gui/robber" ));
	pctrMAS->Add( pbtnMASPolice );
	pctrMAS->Add( pbtnMASDemonstrator );
	pctrMAS->Add( pbtnMASRobber );

	pbtnMASRobber->Unset( OC_GUIMAIN_VISIBLE );

// the current pctr points to the MAIN one
	pctr = pctrMain;

// there isn't a query container
	pctrQ = NULL;
}


   /*=====================================================================*/
void
City::_DeleteGUI()
{
// delete the Query container if needed
	if (pctrQ != NULL)
		delete pctrQ;

// MAS toolcircle
	delete pctrMAS;
	delete pbtnMASRobber;
	delete pbtnMASDemonstrator;
	delete pbtnMASPolice;

// Load/save toolcircle
	delete pctrS;
	delete pbtnSB;
	delete pbtnSS;
	delete pbtnSL;

// Nature toolcircle
	delete pctrN;
	delete pbtnNB;
	delete pbtnNP;
	delete pbtnNT;

// GUI G toolcircle ( park, education, hospital, police and fire )
	delete pctrG;
	delete pbtnGB;
	delete pbtnGP;
	delete pbtnGE;
	delete pbtnGH;
	delete pbtnGL;
	delete pbtnGF;

// GUI T toolcircle ( raise, lower terrain )
	delete pctrT;
	delete pbtnTB;
	delete pbtnTU;
	delete pbtnTD;
	delete pbtnTX;
	delete pbtnTQ;

// GUI L toolcircle ( electric lines, electric plants )
	delete pctrL;
	delete pbtnLB;
	delete pbtnLL;
	delete pbtnLE;

// GUI Z toolcircle
	delete pctrZ;
	delete pbtnZB;
	delete pbtnZR;
	delete pbtnZC;
	delete pbtnZI;

// GUI main toolcircle
	delete pctrMain;
	delete pbtnZ;
	delete pbtnS;
	delete pbtnL;
	delete pbtnP;
	delete pbtnX;
	delete pbtnG;
}


   /*=====================================================================*/
void
City::_DoTool(
	const SDL_MouseButtonEvent & sdlMBEvent )
{
	if ( enumCurrentTool == OC_NONE )
		return;

	static uint cost;
	static OPENCITY_ERR_CODE enumErrCode;
	static int w1, h1, w2, h2;

	Structure* pstruct = NULL;		// for MAS

	cost = 0;
	w1 = uiMapW1;
	h1 = uiMapL1;
	w2 = uiMapW2;
	h2 = uiMapL2;
	OPENCITY_SWAP( w1, w2, int );
	OPENCITY_SWAP( h1, h2, int );


// we return if we don't have enough funds
	if (_liCityFund < 0)
		return;


// block all the sim threads while modifying the game datas
	SDL_LockMutex( gpmutexSim );

	switch (this->enumCurrentTool) {
	case OC_ZONE_RES:
		if ((enumErrCode = ptabLayer[ enumCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_RES, cost )) == OC_ERR_FREE) {
			gpAudioMgr->PlaySound( OC_SOUND_RCI );
		}
		break;

	case OC_ZONE_COM:
		if ((enumErrCode = ptabLayer[ enumCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_COM, cost )) == OC_ERR_FREE) {
			gpAudioMgr->PlaySound( OC_SOUND_RCI );
		}
		break;

	case OC_ZONE_IND:
		if ((enumErrCode = ptabLayer[ enumCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_IND, cost )) == OC_ERR_FREE) {
			gpAudioMgr->PlaySound( OC_SOUND_RCI );
		}
		break;

	case OC_BUILD_ROAD:
		if ((enumErrCode = ptabLayer[ enumCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_ROAD, cost )) == OC_ERR_FREE) {
			gpAudioMgr->PlaySound( OC_SOUND_ROAD );
		}
		break;

	case OC_BUILD_ELINE:
		if ((enumErrCode = ptabLayer[ enumCurrentLayer ]->
			BuildStructure( 
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_ELINE, cost )) == OC_ERR_FREE) {
			gpAudioMgr->PlaySound( OC_SOUND_ELINE );
		}
		break;

	case OC_BUILD_EPLANT:
		if ((enumErrCode = ptabLayer[ enumCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_EPLANT_COAL, cost )) == OC_ERR_FREE) {
			_pMSim->AddStructure( uiMapW1, uiMapL1, uiMapW2, uiMapL2, MainSim::OC_MICROSIM_ELE );
			gpAudioMgr->PlaySound( OC_SOUND_EPLANT );
		}
		break;

	case OC_BUILD_PARK:
		if ((enumErrCode = ptabLayer[ enumCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_PARK, cost )) == OC_ERR_FREE) {
			gpAudioMgr->PlaySound( OC_SOUND_EPLANT );
		}
		break;

	case OC_BUILD_FLORA:
		if ((enumErrCode = ptabLayer[ enumCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_FLORA, cost )) == OC_ERR_FREE) {
			gpAudioMgr->PlaySound( OC_SOUND_EPLANT );
		}
		break;

	case OC_BUILD_FIRE:
		if ((enumErrCode = ptabLayer[ enumCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_FIREDEPT, cost )) == OC_ERR_FREE) {
// not used			_pMSim->AddStructure( uiMapW1, uiMapL1, uiMapW2, uiMapL2, MainSim::OC_MICROSIM_ELE );
			gpAudioMgr->PlaySound( OC_SOUND_EPLANT );
		}
		break;

	case OC_BUILD_POLICE:
		if ((enumErrCode = ptabLayer[ enumCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_POLICEDEPT, cost )) == OC_ERR_FREE) {
// not used			_pMSim->AddStructure( uiMapW1, uiMapL1, uiMapW2, uiMapL2, MainSim::OC_MICROSIM_ELE );
			gpAudioMgr->PlaySound( OC_SOUND_EPLANT );
		}
		break;

	case OC_BUILD_HOSPITAL:
		if ((enumErrCode = ptabLayer[ enumCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_HOSPITALDEPT, cost )) == OC_ERR_FREE) {
// not used			_pMSim->AddStructure( uiMapW1, uiMapL1, uiMapW2, uiMapL2, MainSim::OC_MICROSIM_ELE );
			gpAudioMgr->PlaySound( OC_SOUND_EPLANT );
		}
		break;

	case OC_BUILD_EDUCATION:
		if ((enumErrCode = ptabLayer[ enumCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_EDUCATIONDEPT, cost )) == OC_ERR_FREE) {
// not used			_pMSim->AddStructure( uiMapW1, uiMapL1, uiMapW2, uiMapL2, MainSim::OC_MICROSIM_ELE );
			gpAudioMgr->PlaySound( OC_SOUND_EPLANT );
		}
		break;

	case OC_BUILD_TEST_BUILDING:
		if ((enumErrCode = ptabLayer[ enumCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_TEST, cost )) == OC_ERR_FREE) {
			gpAudioMgr->PlaySound( OC_SOUND_EPLANT );
		}
		break;

	case OC_BUILD_AGENT_POLICE:
		pstruct = ptabLayer[ BUILDING_LAYER ]->GetStructure( uiMapW1, uiMapL1 );
		if ((pstruct != NULL) && (pstruct->GetCode() == OC_STRUCTURE_ROAD))
		new AgentPolice(*gpKernel, *gpEnvironment, uiMapW1, uiMapL1);
		break;

	case OC_BUILD_AGENT_DEMONSTRATOR:
		pstruct = ptabLayer[ BUILDING_LAYER ]->GetStructure( uiMapW1, uiMapL1 );
		if ((pstruct != NULL) && (pstruct->GetCode() == OC_STRUCTURE_ROAD))
		new AgentDemonstrator(*gpKernel, *gpEnvironment, uiMapW1, uiMapL1);
		break;

	case OC_BUILD_AGENT_ROBBER:
		pstruct = ptabLayer[ BUILDING_LAYER ]->GetStructure( uiMapW1, uiMapL1 );
		if ((pstruct != NULL) && (pstruct->GetCode() == OC_STRUCTURE_ROAD))
		new AgentRobber(*gpKernel, *gpEnvironment, uiMapW1, uiMapL1);
		break;

//FIXME: cost
	case OC_HEIGHT_UP:
		enumErrCode = gpMapMgr->ChangeHeight( uiMapW1, uiMapL1, OC_MAP_UP );
		if ( enumErrCode == OC_ERR_FREE ) {
			gpRenderer->boolHeightChange = true;
			cost = 5;		// Quick hack
		}
		break;

	case OC_HEIGHT_DOWN:
		enumErrCode = gpMapMgr->ChangeHeight( uiMapW1, uiMapL1, OC_MAP_DOWN );
		if ( enumErrCode == OC_ERR_FREE ) {
			gpRenderer->boolHeightChange = true;
			cost = 5;		// Quick hack
		}
		break;

	case OC_QUERY:
	// Delete the old query container
	// we don't need to assign it to NULL since
	// it will contain another value later
		if (pctrQ != NULL)
			delete pctrQ;

	// get the new query container
		pctrQ = ptabLayer[ enumCurrentLayer ]->
			QueryStructure( uiMapW1, uiMapL1 );
	// reset the old container
		pctr->ResetAttribute(
			OC_GUIMAIN_CLICKED | OC_GUIMAIN_MOUSEOVER );
		pctr->Unset( OC_GUIMAIN_VISIBLE );
	// show the informations queried
		pctr = pctrQ;
		pctr->SetLocation(
			sdlMBEvent.x - 70,
			iWinHeight - sdlMBEvent.y - 70 );
		pctr->Set( OC_GUIMAIN_VISIBLE );
		enumErrCode = OC_ERR_SOMETHING;		// avoid to calculate the cost
		break;

	case OC_DESTROY:
	// If it is a part of a bigger structure (an EPLANT for example),
	// the whole structure will be then destroyed
	// The following part tell the simulators to remove the collected data concerning
	// the structures which are going to be destroyed
		_pMSim->RemoveStructure( uiMapW1, uiMapL1, uiMapW2, uiMapL2 );

		enumErrCode = ptabLayer[ enumCurrentLayer ]->
			DestroyStructure( uiMapW1, uiMapL1, uiMapW2, uiMapL2, cost );
		break;

	default:
		enumErrCode = OC_ERR_SOMETHING;// which tool is this ?
	} // switch

// now unlock the mutex and let the sims run
	SDL_UnlockMutex( gpmutexSim );

	if (enumErrCode == OC_ERR_FREE) {
		_liCityFund -= cost;
	}
}


   /*=====================================================================*/
bool
City::_HandleKeyPressed()
{
	static int actionFactor = 1;
	int key;
	bool boolKeyDown;

	if (this->booltabKeyPressed[KEY_ALT] == true) {
		actionFactor = OC_ACTION_FACTOR;
	}
	else {
		actionFactor = 1;
	}

// there is no key pressed
	boolKeyDown = false;

// look for pressed keys
	for (key = KEY_UP; key < KEY_NUMBER; key++) {
		if (this->booltabKeyPressed[key] == true) {
			switch (key) {
			case KEY_UP:
				gpRenderer->MoveDown(actionFactor);
				break;
			case KEY_DOWN:
				gpRenderer->MoveUp(actionFactor);
				break;
			case KEY_RIGHT:
				gpRenderer->MoveLeft(actionFactor);
				break;
			case KEY_LEFT:
				gpRenderer->MoveRight(actionFactor);
				break;

			case KEY_PAGEUP:
				gpRenderer->RotateLeft(actionFactor);
				break;
			case KEY_PAGEDOWN:
				gpRenderer->RotateRight(actionFactor);
				break;

			case KEY_INSERT: // zoom in
				gpRenderer->ZoomIn();
				break;
			case KEY_DELETE: // zoom out
				gpRenderer->ZoomOut();
				break;
			} // switch

		// there's at least one key pressed
			boolKeyDown = true;
		} // if
	} // for

// tell the caller that there's whether at least a key pressed or not
	return boolKeyDown;
}


   /*=====================================================================*/
void
City::_DoBill(
	const OPENCITY_PROPERTY_CODE & enumProperty )
{
	uint maintenance;
	uint index, surface;
	Structure* pStruct;
	static uint income = 0;

	surface = _uiWidth * _uiHeight;
	maintenance = 0;
	for (index = 0; index < surface; index++) {
		pStruct = ptabLayer[ BUILDING_LAYER ]->GetLinearStructure( index );
		if (pStruct != NULL)
			maintenance += gpPropertyMgr->Get( 
				OC_MAINTENANCE_COST, pStruct->GetCode() );
	}

	_liCityFund -= maintenance;

// accumulate the income each month
	income += (_pMSim->GetValue(MainSim::OC_MICROSIM_RES) * OC_R_INCOME_TAX / 100)
	        + (_pMSim->GetValue(MainSim::OC_MICROSIM_COM) * OC_C_INCOME_TAX / 100)
	        + (_pMSim->GetValue(MainSim::OC_MICROSIM_IND) * OC_I_INCOME_TAX / 100);

// add the income only if we reach the end of the year
	if (enumProperty == OC_INCOME ) {
	// here is the gouvernment's help for this year :D
		income += income * OC_INCOME_HELP / 100;

		_liCityFund += income;
		income = 0;
	}
}


   /*=====================================================================*/
void
City::_HandleGUIClick()
{
	uint uiObject;
	GUIContainer* pctrOld;
	int iX, iY;

	pctrOld = pctr;
	uiObject = this->pctr->GetClick();

// is this the main container ?
	if (pctr == pctrMain)
	switch (uiObject) {
		case 1: // switch to Z toolcircle
			pctr = pctrZ;
			pctr->Set( 1, OC_GUIMAIN_MOUSEOVER );
			break;
		case 2: // load/save toolcircle
			pctr = pctrS;
			pctr->Set( 1, OC_GUIMAIN_MOUSEOVER );
			break;
		case 3:  // L button, open the L toolcircle
			pctr = pctrL;
			pctr->Set( 1, OC_GUIMAIN_MOUSEOVER );
			break;
		case 4:  // P button, set tool for "building road"
			enumCurrentTool = OC_BUILD_ROAD;
			_cTool = 'P';
			break;
		case 5: // T button, open the T toolcircle
			pctr = pctrT;
			pctr->Set( 1, OC_GUIMAIN_MOUSEOVER );
			break;
		case 6: // G button, open the government toolcircle
			pctr = pctrG;
			pctr->Set( 1, OC_GUIMAIN_MOUSEOVER );
			break;

		default: // never reached
			OPENCITY_DEBUG( "WARNING: What's going wrong ?" );
			assert(0);
			break;
	}

// the user clicked on the Zone toolcircle
	else if (pctr == pctrZ )
	switch (uiObject) {
		case 1: // back button, open the main toolcircle
			pctr = pctrMain;
			pctr->Set( 1, OC_GUIMAIN_MOUSEOVER );
			break;
		case 2: // R button
			enumCurrentTool = OC_ZONE_RES;
			_cTool = 'R';
			break;
		case 3:  // C button, set tool for "zone commercial"
			enumCurrentTool = OC_ZONE_COM;
			_cTool = 'C';
			break;
		case 4:  // I button, set tool for "zone industrial"
			enumCurrentTool = OC_ZONE_IND;
			_cTool = 'I';
			break;

		default:
			OPENCITY_DEBUG("Design error");
			assert( 0 );
			break;
	}

// the user clicked on the eLectric toolcircle
	else if (pctr == pctrL)
	switch (uiObject) {
		case 1: // back button, open the main toolcircle
			pctr = pctrMain;
		// highlight the previous button under the mouse cursor
			pctr->Set( 3, OC_GUIMAIN_MOUSEOVER );
			break;
		case 2:  // L button, set tool for building electric lines
			enumCurrentTool = OC_BUILD_ELINE;
			_cTool = 'L';
			break;
		case 3:  // set tool for building electric plants
			enumCurrentTool = OC_BUILD_EPLANT;
			_cTool = 'E';
			break;

		default:
			OPENCITY_DEBUG( "WARNING: What's going wrong ?" );
			assert(0);
			break;
	}

// the user clicked on the Terrain toolcircle
	else if (pctr == pctrT)
	switch (uiObject) {
		case 1: // back button, open the main toolcircle
			pctr = pctrMain;
			pctr->Set( 5, OC_GUIMAIN_MOUSEOVER );
			break;
		case 2:  // height up
			enumCurrentTool = OC_HEIGHT_UP;
			_cTool = 'U';
			break;
		case 3:  // height down
			enumCurrentTool = OC_HEIGHT_DOWN;
			_cTool = 'D';
			break;
		case 4:  // destroy tool
			enumCurrentTool = OC_DESTROY;
			_cTool = 'X';
			break;
		case 5: // query tool
			enumCurrentTool = OC_QUERY;
			_cTool = 'Q';
			break;

		default:
			OPENCITY_DEBUG( "WARNING: What's going wrong ?" );
			assert(0);
			break;
	}

// the user clicked on the government toolcircle
	else if (pctr == pctrG)
	switch (uiObject) {
		case 1: // back button, open the main toolcircle
			pctr = pctrMain;
			pctr->Set( 6, OC_GUIMAIN_MOUSEOVER );
			break;
		case 2:  // nature toolcircle
			pctr = pctrN;
			pctr->Set( 1, OC_GUIMAIN_MOUSEOVER );
			break;
		case 3:
			enumCurrentTool = OC_BUILD_EDUCATION;
			_cTool = '?';
			break;
		case 4:
			enumCurrentTool = OC_BUILD_HOSPITAL;
			_cTool = '?';
			break;
		case 5:
			enumCurrentTool = OC_BUILD_POLICE;
			_cTool = '?';
			break;
		case 6:
			enumCurrentTool = OC_BUILD_FIRE;
			_cTool = '?';
			break;

		default:
			OPENCITY_DEBUG( "WARNING: Unknown command" );
			assert(0);
			break;
	}

// the user clicked on the load/save toolcircle
	else if (pctr == pctrN)
	switch (uiObject) {
		case 1: // back button, open the government toolcircle
			pctr = pctrG;
			pctr->Set( 2, OC_GUIMAIN_MOUSEOVER );
			break;
		case 2:  // build park
			enumCurrentTool = OC_BUILD_PARK;
			_cTool = '?';
			break;
		case 3:  // build tree
			enumCurrentTool = OC_BUILD_FLORA;
			_cTool = '?';
			break;

		default:
			OPENCITY_DEBUG( "WARNING: Unknown tool" );
			assert(0);
			break;
	}

// the user clicked on the load/save toolcircle
	else if (pctr == pctrS)
	switch (uiObject) {
		case 1: // back button, open the main toolcircle
			pctr = pctrMain;
			pctr->Set( 5, OC_GUIMAIN_MOUSEOVER );
			break;
		case 2:  // save
			_Save( ocHomeDirPrefix( "opencity.save" ) );
			break;
		case 3:  // load
			_Load( ocHomeDirPrefix( "opencity.save" ) );
			break;

		default:
			OPENCITY_DEBUG( "WARNING: Unknown command" );
			assert(0);
			break;
	}

// the user clicked on the Path toolcircle
	else if (pctr == pctrPath)
	switch (uiObject) {
		case 1: // start button
			_cTool = 'N';
			enumCurrentTool = OC_NONE;
			boolPathGo = false;
//debug cout << "changed to false" << endl;
			break;
		case 2: // stop button
			_cTool = 'N';
			enumCurrentTool = OC_NONE;
			boolPathGo = true;
			this->uiVehicleType = Vehicle::VEHICLE_BUS;
			break;
		case 3: // stop button
			_cTool = 'N';
			enumCurrentTool = OC_NONE;
			boolPathGo = true;
			this->uiVehicleType = Vehicle::VEHICLE_SPORT;
//debug cout << "changed to true" << endl;
			break;
		case 4: //
			enumCurrentTool = OC_BUILD_TEST_BUILDING;
			_cTool = '?';
			break;
		default:
			break;
	}

// The user has clicked on the MAS toolcircle
	else if (pctr == pctrMAS)
	switch (uiObject) {
		case 1: // start button
			_cTool = 'N';
			enumCurrentTool = OC_BUILD_AGENT_POLICE;
			break;
		case 2: // stop button
			_cTool = 'N';
			enumCurrentTool = OC_BUILD_AGENT_DEMONSTRATOR;
			break;
		case 3: // stop button
			_cTool = 'N';
			enumCurrentTool = OC_BUILD_AGENT_ROBBER;
			break;

		default:
			break;
	}


// if the container has been changed then we reset
// the MouseOver & Clicked attributes
// otherwise we reset only the Clicked attribute
	if (pctr != pctrOld) {
		pctrOld->ResetAttribute(
			OC_GUIMAIN_CLICKED | OC_GUIMAIN_MOUSEOVER );
		pctrOld->GetLocation( iX, iY );
		pctr->SetLocation( iX, iY );
		pctr->Set( OC_GUIMAIN_VISIBLE );
	}
	else {
		pctrOld->ResetAttribute(
			OC_GUIMAIN_CLICKED );
	}
	pctrOld->Unset( OC_GUIMAIN_VISIBLE );
}


   /*=====================================================================*/
void
City::_HandleMouseXY()
{
	#define OC_MOUSE_AUTOSCROLL 15

	static int mouseX, mouseY;

// return immediately if the app doesn't have mouse focus
	if (!(SDL_GetAppState() & SDL_APPMOUSEFOCUS ))
		return;

	SDL_GetMouseState( &mouseX, &mouseY );

// handle horizontal automatic map translation
	if ((mouseX < OC_MOUSE_AUTOSCROLL) && !(mouseY < OC_MOUSE_AUTOSCROLL))
		gpRenderer->MoveRight();
	if ((mouseX >= iWinWidth-OC_MOUSE_AUTOSCROLL) && !(mouseY < OC_MOUSE_AUTOSCROLL))
		gpRenderer->MoveLeft();

// handle vertical automatic map translation
	if (!(mouseX < OC_MOUSE_AUTOSCROLL)
	  &&!(mouseX >= iWinWidth-OC_MOUSE_AUTOSCROLL) && (mouseY < OC_MOUSE_AUTOSCROLL))
		gpRenderer->MoveDown();
	if ((mouseY >= iWinHeight-OC_MOUSE_AUTOSCROLL))
		gpRenderer->MoveUp();

// handle map rotation
	if ((mouseX < OC_MOUSE_AUTOSCROLL) && (mouseY < OC_MOUSE_AUTOSCROLL))
		gpRenderer->RotateLeft();

	if ((mouseY < OC_MOUSE_AUTOSCROLL) && (mouseX >= iWinWidth-OC_MOUSE_AUTOSCROLL))
		gpRenderer->RotateRight();
}


   /*=====================================================================*/
void
City::_TestPathfinding() {
	if (pctr == pctrPath)
		if (this->boolPathGo == false) {
			this->uiPathStartW = this->uiMapW2;
			this->uiPathStartH = this->uiMapL2;
		}
		else {
		//TODO: put this elsewhere
			vector<Destination> vdest;
	
			this->uiPathStopW = this->uiMapW2;
			this->uiPathStopH = this->uiMapL2;
	
		// Buses prefer short distance
			if (this->uiVehicleType == Vehicle::VEHICLE_BUS) {
				gpPathFinder->findShortestPath(
					uiPathStartW, uiPathStartH,
					uiPathStopW, uiPathStopH,
					vdest,
					PathFinder::OC_DISTANCE );
			}
		// Sport vehicle prefer less traffic
			else if (this->uiVehicleType == Vehicle::VEHICLE_SPORT) {
				gpPathFinder->findShortestPath(
					uiPathStartW, uiPathStartH,
					uiPathStopW, uiPathStopH,
					vdest,
					PathFinder::OC_TRAFFIC );
			}
	
		// Now create the new vehicle if a path was found
			if ( vdest.size() > 0 ) {
				pvehicle = new Vehicle(
					(Vehicle::VEHICLE_TYPE)this->uiVehicleType );
				pvehicle->SetPath( vdest );	// path init
				pvehicle->Start();		// vehicle init
				if (gpMoveMgr->Add( pvehicle ) < 0) {
					OPENCITY_DEBUG("MoveMgr full");
					delete pvehicle;
				}
			}
		}
//debug: pathfinding
/*
cout << "StW: " << uiPathStartW << " / " << " StH: " << uiPathStartH
     << " SpW: " << uiPathStopW << " / " << "SpH: " << uiPathStopH << endl;
*/
}


   /*=====================================================================*/
void
City::_BuildPreview()
{
	static OPENCITY_STRUCTURE_CODE scode = OC_STRUCTURE_UNDEFINED;
	static OPENCITY_GRAPHIC_CODE gcode = OC_EMPTY;
	static OPENCITY_TOOL_CODE tcode = OC_NONE;
	static OPENCITY_ERR_CODE ecode = OC_ERR_FREE;


// Get the corresponding structure code of the tool
	if (tcode != this->enumCurrentTool) {
		tcode = this->enumCurrentTool;
		switch (tcode) {
		/* not implemented yet
			case OC_ZONE_RES:
				scode = OC_STRUCTURE_RES;
				break;
			case OC_ZONE_COM:
				scode = OC_STRUCTURE_COM;
				break;
			case OC_ZONE_IND:
				scode = OC_STRUCTURE_IND;
				break;
			case OC_BUILD_ROAD:
				scode = OC_STRUCTURE_ROAD;
				break;	
			case OC_BUILD_ELINE:
				scode = OC_STRUCTURE_ELINE;
				break;
		*/
	
			case OC_BUILD_EPLANT:
				scode = OC_STRUCTURE_EPLANT_COAL;
				break;
			case OC_BUILD_PARK:
				scode = OC_STRUCTURE_PARK;
				break;
			case OC_BUILD_FLORA:
				scode = OC_STRUCTURE_FLORA;
				break;
			case OC_BUILD_FIRE:
				scode = OC_STRUCTURE_FIREDEPT;
				break;
			case OC_BUILD_POLICE:
				scode = OC_STRUCTURE_POLICEDEPT;
				break;
			case OC_BUILD_HOSPITAL:
				scode = OC_STRUCTURE_HOSPITALDEPT;
				break;
			case OC_BUILD_EDUCATION:
				scode = OC_STRUCTURE_EDUCATIONDEPT;
				break;
		
			case OC_BUILD_TEST_BUILDING:
				scode = OC_STRUCTURE_TEST;
				break;

			default:
				scode = OC_STRUCTURE_UNDEFINED;
				break;
		} // switch
	} // if

// Get the corresponding graphic code
	if ((scode != OC_STRUCTURE_UNDEFINED)
	 && (this->boolLMBPressed == true)) {
		ecode = ptabLayer[ enumCurrentLayer ]->
			BuildPreview( uiMapW1, uiMapL1, scode, gcode );

		if (ecode == OC_ERR_FREE) {
			gpRenderer->DisplayBuildPreview(
				uiMapW1, uiMapL1, OC_GREEN_COLOR, gcode );
		}
		else {
			gpRenderer->DisplayBuildPreview(
				uiMapW1, uiMapL1, OC_RED_COLOR, gcode );
		}
	}
}


   /*=====================================================================*/
bool
City::_Save( const string& strFilename )
{
	fstream fs;

	fs.open( strFilename.c_str(), ios_base::out | ios_base::binary | ios_base::trunc );
	if (!fs.good()) {
		OPENCITY_DEBUG( "File opening error: " << strFilename );
		return false;
	}

// Lock the simulator
	SDL_LockMutex( gpmutexSim );

// Save city data
	this->SaveTo( fs );

// Save map data
	gpMapMgr->SaveTo( fs );

// Save layers's data
	ptabLayer[ BUILDING_LAYER ]->SaveTo( fs );

// Save simulators data
	_pMSim->SaveTo( fs );

// Unlock the simulator
	SDL_UnlockMutex( gpmutexSim );

	fs.close();
	return true;
}

   /*=====================================================================*/
bool
City::_Load( const string& strFilename )
{
	fstream fs;
	uint w, l;

	fs.open( strFilename.c_str(), ios_base::in | ios_base::binary );
	if (!fs.good()) {
		OPENCITY_DEBUG( "File opening error" << strFilename );
		return false;
	}

// Lock the simulator
	SDL_LockMutex( gpmutexSim );

// Load city data
	this->LoadFrom( fs );

// Load map data
	gpMapMgr->LoadFrom( fs );
	gpRenderer->boolHeightChange = true;

// Load layers' data
	ptabLayer[ BUILDING_LAYER ]->LoadFrom( fs );

// Load simulators' data
	_pMSim->LoadFrom( fs );

// Manually add the structures to the simulators
	for ( w = 0; w < _uiWidth; w++ ) {
		for ( l = 0; l < _uiHeight; l++ ) {
			_pMSim->AddStructure( w, l, w, l );
		}
	}

// Unlock the simulator
	SDL_UnlockMutex( gpmutexSim );

	fs.close();
	return true;
}




































