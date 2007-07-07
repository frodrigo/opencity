/***************************************************************************
						city.cpp  -  description
							-------------------
	begin                : may 28th, 2003
	copyright            : (C) 2003-2007 by Duong-Khang NGUYEN
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

// Useful enumerations
#include "opencity_direction.h"
#include "opencity_structure_type.h"

// OpenCity headers
#include "city.h"
#include "vehicle.h"
#include "mainsim.h"				// simulator
#include "structure.h"
#include "buildinglayer.h"
#include "guibar.h"
#include "guibutton.h"				// GUI management
#include "guilabel.h"
#include "guicontainer.h"
#include "agentpolice.h"
#include "agentdemonstrator.h"
#include "agentrobber.h"

// Global settings
#include "globalvar.h"
extern GlobalVar gVars;

// Standard headers
#include <sstream>					// For text output with data conversion


#define OC_ACTION_FACTOR 10


   /*=====================================================================*/
City::City
(
	const uint width,
	const uint length,
	const int difficulty,
	const bool bGUIEnabled
):
strCityName("OpenCity"),
iDifficulty( difficulty ),
_bGUIEnabled( bGUIEnabled ),
strFileName(""),
_uiIncome( 0 ),
_liCityFund( OC_FUND_START ),
_uiPopulation( 0 ),

_uiDay( 1 ),
_uiMonth( 1 ),
_uiYear( 0 ),

_uiWidth( width ),
_uiLength( length ),
_cTool('N'),

boolLMBPressed( false ),
_eCurrentLayer( OC_LAYER_BUILDING ),
_eSpeed( OC_SPEED_NORMAL ),
_eCurrentTool( OC_NONE )
{
	OPENCITY_DEBUG( "City ctor - default parameters" );

// Set the windows's WH
	_iWinWidth = gVars.guiScreenWidth;
	_iWinHeight = gVars.guiScreenHeight;

// Registering our call-back interface for SDL events' treatment
	ocSetNewUI( this );

// Keyboard bool table's initialization
	int i;
	for (i = 0; i < KEY_NUMBER; i++)
		_abKeyPressed[i] = false;

// Initialize the statistics
	Ressource res = {0, 0, 0, 0, 0, 0, 0, 0};
	for (i = 0; i < OC_MAX_RESSOURCE_RECORD; i++) {
		_dqRessource.push_back(res);
	}

// Layers' initialization
	_apLayer[ OC_LAYER_BUILDING ] = new BuildingLayer( *this );
	gVars.gpMapMgr->SetLayer( _apLayer[OC_LAYER_BUILDING] );

// Put few trees on the building layer
	_CreateTree();

// Pathfinder initialization
	gVars.gpPathFinder = new PathFinder(
		gVars.gpmutexSim,
		(BuildingLayer*)_apLayer[ OC_LAYER_BUILDING ],
		gVars.gpMapMgr,
		_uiWidth, _uiLength );

// Simulators' initialization
	_CreateSimulator();

	if (_bGUIEnabled) {
	// Debug toolcircle
		boolPathGo = false;
		uiPathStartW = 0; uiPathStartH = 0;
		uiPathStopW = 0; uiPathStopH = 0;
		pctrPath = new GUIContainer( 100, 100, 140, 140, ocHomeDirPrefix( "graphism/gui/toolcircle_bg.png" ));
		pbtnPathStart = new GUIButton( 20,  20,  30, 30, ocHomeDirPrefix( "graphism/gui/raise" ));
		pbtnPathStop1 = new GUIButton( 60,  0,   30, 30, ocHomeDirPrefix( "graphism/gui/lower" ));
		pbtnPathStop2 = new GUIButton( 100, 20,  30, 30, ocHomeDirPrefix( "graphism/gui/lower" ));
		pbtnTestBuilding = new GUIButton(  20,  70, 30, 30, ocHomeDirPrefix( "graphism/gui/residential" ));
		pctrPath->Add( pbtnPathStart );
		pctrPath->Add( pbtnPathStop1 );
		pctrPath->Add( pbtnPathStop2 );
		pctrPath->Add( pbtnTestBuilding );
		pvehicle = NULL;
	
	// create the GUI
		_CreateGUI();
	}
}


   /*=====================================================================*/
City::~City(){
	OPENCITY_DEBUG( "City dtor" );

	if (_bGUIEnabled) {
	// delete the GUI
		_DeleteGUI();
	
	// testing, delete the pathfinder
		delete pctrPath;
		delete pbtnTestBuilding;
		delete pbtnPathStart;
		delete pbtnPathStop1;
		delete pbtnPathStop2;
	}

// delete all the simulators
	_DeleteSimulator();

// Delete the pathfinder
	delete gVars.gpPathFinder;
	gVars.gpPathFinder = NULL;

// delete only the OC_LAYER_BUILDING instead of delete [] _apLayer
	delete _apLayer[ OC_LAYER_BUILDING ];
	_apLayer[ OC_LAYER_BUILDING ] = NULL;

// this must be done AFTER deleting the layers
//	delete gVars.gpMapMgr;
}


   /*=====================================================================*/
void
City::SaveTo( std::fstream& rfs )
{
	OPENCITY_DEBUG( __PRETTY_FUNCTION__ << "saving" );

// Save the data
	rfs << _uiIncome << std::ends;
	rfs << _liCityFund << std::ends;
	rfs << _uiPopulation << std::ends;
	rfs << _uiDay << std::ends;
	rfs << _uiMonth << std::ends;
	rfs << _uiYear << std::ends;
	rfs << _uiWidth << std::ends;
	rfs << _uiLength << std::ends;

// Store the ressource statistics
	Ressource res;
	int i;
	for (i = 0; i < OC_MAX_RESSOURCE_RECORD; i++) {
		res = _dqRessource[i];

		rfs << res.fund << std::ends;
		rfs << res.population << std::ends;
		rfs << res.r << std::ends;
		rfs << res.c << std::ends;
		rfs << res.i << std::ends;
		rfs << res.w << std::ends;
		rfs << res.e << std::ends;
		rfs << res.g << std::ends;
	}
}


   /*=====================================================================*/
void
City::LoadFrom( std::fstream& rfs )
{
	OPENCITY_DEBUG( __PRETTY_FUNCTION__ << "loading" );

// Load the data
	rfs >> _uiIncome; rfs.ignore();
	rfs >> _liCityFund; rfs.ignore();
	rfs >> _uiPopulation; rfs.ignore();
	rfs >> _uiDay; rfs.ignore();
	rfs >> _uiMonth; rfs.ignore();
	rfs >> _uiYear; rfs.ignore();
	rfs >> _uiWidth; rfs.ignore();
	rfs >> _uiLength; rfs.ignore();

// Load the ressource statistics
	Ressource res;
	int i;
	for (i = 0; i < OC_MAX_RESSOURCE_RECORD; i++) {
		rfs >> res.fund; rfs.ignore();
		rfs >> res.population; rfs.ignore();
		rfs >> res.r; rfs.ignore();
		rfs >> res.c; rfs.ignore();
		rfs >> res.i; rfs.ignore();
		rfs >> res.w; rfs.ignore();
		rfs >> res.e; rfs.ignore();
		rfs >> res.g; rfs.ignore();

		_dqRessource[i] = res;
	}
}


   /*=====================================================================*/
void City::SetCurrentLayer( OPENCITY_CITY_LAYER enumNewLayer )
{}


   /*=====================================================================*/
void City::Run()
{
	static uint uiNumberFrame = 0;

// Send the movement manager the move order 
	gVars.gpMoveMgr->Move();

// IF the audio is enable THEN autoplay the background music
// TODO: optimize this
	if (gVars.gboolUseAudio && !gVars.gpAudioMgr->PlayingMusic()) {
		gVars.gpAudioMgr->PlayNextMusic();
	}

// Do not process further if we are in pause mode
	if (_eSpeed == OC_SPEED_PAUSE)
		return;

// IF not new day THEN return
	if ( ++uiNumberFrame*gVars.guiMsPerFrame <= OC_MS_PER_DAY )
		return;

// New day
	uint r = _pMSim->GetValue(Simulator::OC_RESIDENTIAL);
	uint c = _pMSim->GetValue(Simulator::OC_COMMERCIAL);
	uint i = _pMSim->GetValue(Simulator::OC_INDUSTRIAL);

	if ( ++_uiDay > 30 ) {
	// New month
		_uiDay = 1;

	// Calculate the current population
		_uiPopulation = r + c/2 + i/3;

		if ( ++_uiMonth > 12 ) {
		// New year
			_uiMonth = 1;
			_uiYear++;

			_DoBill( OC_INCOME );
			_RecordRessource();
		}
		else {
			_DoBill( OC_MAINTENANCE_COST );
		}
	}
	uiNumberFrame = 0;

// Update the screen information every 3 days
	if ( _uiDay%3 == 0 ) {
		uint initialValue = 0;

	// Update the RCI bar value
		initialValue = (r+c+i) + 1;
		pbarResidence->SetInitialValue( initialValue );
		pbarCommerce->SetInitialValue( initialValue );
		pbarIndustry->SetInitialValue( initialValue );
		pbarResidence->SetValue( r );
		pbarCommerce->SetValue( c );
		pbarIndustry->SetValue( i );

	// Update the power bar value
		initialValue = _pMSim->GetMaxValue(Simulator::OC_ELECTRIC) + 1;
		pbarPower->SetInitialValue( initialValue );
		pbarPower->SetValue( _pMSim->GetValue(Simulator::OC_ELECTRIC) );

	// Request the renderer to update the minimap
		gVars.gpRenderer->bMinimapChange = true;
	}
}


   /*=====================================================================*/
void City::Display()
{
	static ostringstream ossStatus;
	static bool boolKeyDown;
	static int iMouseX, iMouseY;


// Process key events such as: up, down, left, right etc..
	boolKeyDown = _HandleKeyPressed();

// IF the mouse reach the border of the screen, translate the map
// NOTE: unused at the moment because it disturbs the GUI
//	_HandleMouseXY();

// NOTE: We can move the following part to City::uiMouseMotion 
// however, in this case City::uiMouseMotion is called each time
// when the mouse moves, and this is no good.
// The user is dragging
	if ((this->boolLMBPressed == true) && (_eCurrentTool != OC_NONE )) {
	// IF the user is dragging with the left mouse button THEN
		if ( SDL_GetMouseState( &iMouseX, &iMouseY ) & SDL_BUTTON(1) ) {
			gVars.gpRenderer->GetSelectedWHFrom(
				iMouseX, iMouseY,
				this->uiMapW2, this->uiMapL2,
				gVars.gpMapMgr, _apLayer[ _eCurrentLayer ] );

		// draw the map with the highlighted area
			gVars.gpRenderer->DisplayHighlight(
				gVars.gpMapMgr, _apLayer[ _eCurrentLayer ],
				this->uiMapW1, this->uiMapL1,
				this->uiMapW2, this->uiMapL2,
				_eCurrentTool );

			goto cityrun_swap;
		}
	}

// Display the screen as usual
	gVars.gpRenderer->Display( gVars.gpMapMgr, _apLayer[ _eCurrentLayer ] );

cityrun_swap:
// Display build preview
	_BuildPreview();

// Display the city's funds
	ossStatus.str("");
	ossStatus << _liCityFund;
	plblFund->SetText( ossStatus.str() );

// Display the city population
	ossStatus.str("");
	ossStatus << _uiPopulation;
	plblPopulation->SetText( ossStatus.str() );

// display the tool
	ossStatus.str("");
	ossStatus << "Tool: " << _cTool;
	gVars.gpRenderer->DisplayText( 550, this->_iWinHeight-15, OC_WHITE_COLOR, ossStatus.str() );

// display the date
	ossStatus.str("");
	ossStatus << _uiDay << "/" << _uiMonth << "/" << _uiYear;
	plblDate->SetText( ossStatus.str() );


// Display all the contained movements
//	gVars.gpMoveMgr->Move();		// called by Run()
	gVars.gpMoveMgr->Display();

// FIXME: buggy MAS environment
//	gVars.gpEnvironment->displayAgent();

// Display the current container
	pctr->Display();

// Display the status bar
	pctrStatus->Display();

// Swap the buffers and update the screen
	SDL_GL_SwapBuffers();
}


   /*=====================================================================*/
Layer*
City::GetLayer( OPENCITY_CITY_LAYER enumLayer ) const
{
	return _apLayer[ enumLayer ];
}


   /*=====================================================================*/
const void
City::GetWL(
	uint & w, uint & l ) const
{
	w = _uiWidth;
	l = _uiLength;
}


   /*=====================================================================*/
   /*                    BASE CLASS 'UI' IMPLEMENTATION                   */
   /*=====================================================================*/



   /*=====================================================================*/
void City::Keyboard( const SDL_KeyboardEvent& rcEvent )
{
//	OPENCITY_DEBUG( "Keydown event received" );

// SDL_KEYDOWN or SDL_PRESSED
	if (rcEvent.type == SDL_KEYDOWN) {
	// test if ALT is pressed
		if (rcEvent.keysym.mod & KMOD_ALT) {
			_abKeyPressed[KEY_ALT] = true;
		}

	// key symbols treatment
		switch (rcEvent.keysym.sym) {
		case SDLK_PAGEUP:
			_abKeyPressed[KEY_PAGEUP] = true;
			break;
		case SDLK_PAGEDOWN:
			_abKeyPressed[KEY_PAGEDOWN] = true;
			break;

		case SDLK_UP:
			_abKeyPressed[KEY_UP] = true;
			break;
		case SDLK_DOWN:
			_abKeyPressed[KEY_DOWN] = true;
			break;
		case SDLK_RIGHT:
			_abKeyPressed[KEY_RIGHT] = true;
			break;
		case SDLK_LEFT:
			_abKeyPressed[KEY_LEFT] = true;
			break;

	// Establish a connection to OCZen
		case SDLK_z:
			OPENCITY_NET_CODE netCode;
			netCode = gVars.gpNetworking->Open( gVars.gsZenServer );
			switch (netCode) {
				case OC_NET_CLIENT_CONNECTED:
					OPENCITY_INFO( "OpenCity is already connected to a server." );
					break;
				case OC_NET_CLIENT_ACCEPTED:
					OPENCITY_INFO( "The connection request has been accepted." );
					break;
				case OC_NET_CLIENT_REJECTED:
					OPENCITY_INFO( "The connection request has been rejected. Is the server full ?" );
					break;
				default:
					OPENCITY_INFO( "The connection to \"" << gVars.gsZenServer << "\" has failed." );
			}
			break;

		case SDLK_n:	// set the tool to "None"
			_eCurrentTool = OC_NONE;
			_cTool = 'N';
			break;
		case SDLK_r:	// set tool for "zone residential"
			_eCurrentTool = OC_ZONE_RES;
			_cTool = 'R';
			break;
		case SDLK_c:	// set tool for "zone commercial"
			_eCurrentTool = OC_ZONE_COM;
			_cTool = 'C';
			break;
		case SDLK_i:	// set tool for "zone industrial"
			_eCurrentTool = OC_ZONE_IND;
			_cTool = 'I';
			break;

		case SDLK_p:	// set tool for "building road"
			_eCurrentTool = OC_BUILD_ROAD;
			_cTool = 'P';
			break;
		case SDLK_l:	// set tool for building electric lines
			_eCurrentTool = OC_BUILD_ELINE;
			_cTool = 'L';
			break;
		case SDLK_e:	// set tool for building electric plants
			_eCurrentTool = OC_BUILD_EPLANT_NUCLEAR;
			_cTool = 'E';
			break;

		case SDLK_u:	// height up
			_eCurrentTool = OC_HEIGHT_UP;
			_cTool = 'U';
			break;
		case SDLK_d:	// height down
			_eCurrentTool = OC_HEIGHT_DOWN;
			_cTool = 'D';
			break;
		case SDLK_q:	//query tool
			_eCurrentTool = OC_QUERY;
			_cTool = 'Q';
			break;
		case SDLK_x:	// destroy
			_eCurrentTool = OC_DESTROY;
			_cTool = 'X';
			break;


		case SDLK_b:	// toggle structures on/off
			gVars.gpRenderer->ToggleStructure();
			break;
		case SDLK_f:	// toggle wireframe on/off
			gVars.gpRenderer->ToggleWireFrame();
			break;
		case SDLK_g:	// toggle grid on/off
			gVars.gpRenderer->ToggleGrid();
			break;
		case SDLK_k:	// toggle compass on/off
			gVars.gpRenderer->ToggleCompass();
			if (pctrStatus->IsSet( OC_GUIMAIN_VISIBLE ))
				pctrStatus->Unset( OC_GUIMAIN_VISIBLE );
			else
				pctrStatus->Set( OC_GUIMAIN_VISIBLE );
			break;
		case SDLK_o:	// toggle projection mode
			gVars.gpRenderer->ToggleProjection();
			break;
		case SDLK_t:	// toggle terrain display
			gVars.gpRenderer->ToggleTerrain();
			break;
		case SDLK_w:	// toggle water display
			gVars.gpRenderer->ToggleWater();
			break;


		case SDLK_INSERT: // zoom in
			_abKeyPressed[KEY_INSERT] = true;
			break;
		case SDLK_DELETE: // zoom out
			_abKeyPressed[KEY_DELETE] = true;
			break;


	// manipulating the music player
		case SDLK_GREATER:
			gVars.gpAudioMgr->PlayNextMusic();
			break;
		case SDLK_LESS:
			gVars.gpAudioMgr->PlayPreviousMusic();
			break;
		case SDLK_s:
			gVars.gpAudioMgr->ToggleSound();
			break;
		case SDLK_m:
			gVars.gpAudioMgr->ToggleMusic();
			break;


	// Save and load
		case SDLK_F2:
			_Save( ocSaveDirPrefix( "opencity.save" ) );
			break;
		case SDLK_F6:
			_Load( ocSaveDirPrefix( "opencity.save" ) );
			break;


		case SDLK_h:
			gVars.gpRenderer->Home();
			break;
		case SDLK_ESCAPE:
		// quit the program when ESCAPE is pressed
			ocQuit();
			break;

#ifndef NDEBUG
	// Testing PathFinder
		case SDLK_a:
			pctr->ResetAttribute( OC_GUIMAIN_CLICKED | OC_GUIMAIN_MOUSEOVER );
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
			pctr->ResetAttribute( OC_GUIMAIN_CLICKED | OC_GUIMAIN_MOUSEOVER );
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
		if (!(rcEvent.keysym.mod & KMOD_ALT)) {
			_abKeyPressed[KEY_ALT] = false;
		}

	// other key symbols treatment
		switch (rcEvent.keysym.sym) {

		case SDLK_PAGEUP:
			_abKeyPressed[KEY_PAGEUP] = false;
			break;
		case SDLK_PAGEDOWN:
			_abKeyPressed[KEY_PAGEDOWN] = false;
			break;

		case SDLK_UP:
			_abKeyPressed[KEY_UP] = false;
			break;
		case SDLK_DOWN:
			_abKeyPressed[KEY_DOWN] = false;
			break;
		case SDLK_RIGHT:
			_abKeyPressed[KEY_RIGHT] = false;
			break;
		case SDLK_LEFT:
			_abKeyPressed[KEY_LEFT] = false;
			break;

		case SDLK_INSERT: // zoom in
			_abKeyPressed[KEY_INSERT] = false;
			break;
		case SDLK_DELETE: // zoom out
			_abKeyPressed[KEY_DELETE] = false;
			break;

		default:
			break;
		} // switch
	} // key released

}


   /*=====================================================================*/
void
City::MouseMotion( const SDL_MouseMotionEvent& rcEvent )
{
//	OPENCITY_DEBUG("Mouse moved");
	pctr->MouseMotion( rcEvent );
	pctrStatus->MouseMotion( rcEvent );
}


   /*=====================================================================*/
void
City::MouseButton( const SDL_MouseButtonEvent& rcsMBE )
{
//	OPENCITY_DEBUG("Mouse button event received" );

// Process the mouse click on the status bar
	pctrStatus->MouseButton( rcsMBE );
	if ( pctrStatus->GetClick() != 0 ) {
		_HandleStatusClick();
		this->boolLMBPressed = false;
		return;
	}

// Process the click concerning the GUI
	pctr->MouseButton( rcsMBE );
	if ( pctr->GetClick() != 0 ) {
		_HandleGUIClick();
		this->boolLMBPressed = false;
		return;
	}

// The user didn't click on a GUI object so we look for clicks on the map
	switch (rcsMBE.state) {
		case SDL_PRESSED: {
			this->boolLMBPressed = false;
			if ((rcsMBE.button == SDL_BUTTON_LEFT)
				&& (gVars.gpRenderer->GetSelectedWHFrom(
					rcsMBE.x, rcsMBE.y,
					this->uiMapW1, this->uiMapL1,
					gVars.gpMapMgr,
					_apLayer[ _eCurrentLayer ] ) == true ))
			{
				this->boolLMBPressed = true;
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
						_iWinHeight - rcsMBE.y - 70 );
					pctr->Set( OC_GUIMAIN_VISIBLE );
				}
			}
	
		// Wheel button forward
			if (rcsMBE.button == 4) {
			// move right if CTRL pressed
				if (SDL_GetModState() & KMOD_CTRL)
					gVars.gpRenderer->MoveRight();
	
			// move up if SHIFT pressed
				if (SDL_GetModState() & KMOD_SHIFT)
					gVars.gpRenderer->MoveUp();
	
			// zoom in if nothing pressed
				if (!(SDL_GetModState() & (KMOD_SHIFT | KMOD_CTRL)))
					gVars.gpRenderer->ZoomIn();
			}
	
		// Wheel button backward
			if (rcsMBE.button == 5) {
			// move right if CTRL pressed
				if (SDL_GetModState() & KMOD_CTRL)
					gVars.gpRenderer->MoveLeft();
	
			// move up if SHIFT pressed
				if (SDL_GetModState() & KMOD_SHIFT)
					gVars.gpRenderer->MoveDown();
	
			// zoom in if nothing pressed
				if (!(SDL_GetModState() & (KMOD_SHIFT | KMOD_CTRL)))
					gVars.gpRenderer->ZoomOut();
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
				&& gVars.gpRenderer->GetSelectedWHFrom(
					rcsMBE.x, rcsMBE.y,
					this->uiMapW2, this->uiMapL2,
					gVars.gpMapMgr,
					_apLayer[ _eCurrentLayer ] ))
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
City::Expose( const SDL_ExposeEvent& rcEvent )
{
	OPENCITY_DEBUG( "Expose event received" );

	gVars.gpRenderer->Display( gVars.gpMapMgr, _apLayer[ _eCurrentLayer ] );
	pctr->Expose( rcEvent );
	pctrStatus->Expose( rcEvent );

	SDL_GL_SwapBuffers();
}


   /*=====================================================================*/
void City::Resize( const SDL_ResizeEvent& rcEvent )
{
	OPENCITY_DEBUG( "Resize event received" );

// Set the new window's size
	_iWinWidth = rcEvent.w;
	_iWinHeight = rcEvent.h;
	gVars.gpRenderer->SetWinSize( _iWinWidth, _iWinHeight );

// Resize the main status bar and reposition it
	pctrStatus->Resize( rcEvent );
	pctrStatus->SetLocation( (_iWinWidth-512)/2, 0 );

// tell the containers about the event
	pctrMain->Resize( rcEvent );
	pctrL->Resize( rcEvent );
	pctrT->Resize( rcEvent );
	pctrZ->Resize( rcEvent );
	pctrG->Resize( rcEvent );
	pctrN->Resize( rcEvent );
	pctrS->Resize( rcEvent );
	pctrPath->Resize( rcEvent );
	pctrMAS->Resize( rcEvent );
}














   /*=====================================================================*/
   /*                        PRIVATE     METHODS                          */
   /*=====================================================================*/






void City::_CreateTree()
{
// Create a new tree density map
	MapGen::MapMaker mapMaker =
		MapGen::MapMaker(
			_uiWidth, _uiLength,
			gVars.gsGeneratorHeightMap,
			gVars.guiGeneratorMapType,
			gVars.guiGeneratorWaterType,
			gVars.guiGeneratorMapShapeType,
			gVars.guiGeneratorTreeDensityType,
			gVars.guiGeneratorSeed
		);
	int* treeDensity = mapMaker.getTreeDensity();

// Build the trees according to the density map
	uint cost = 0;
	for (uint w = 0, linear = 0; w < _uiWidth; w++)
	for (uint l = 0; l < _uiLength; l++, linear++) {
		if (treeDensity[linear] > 0) {
			_apLayer[ OC_LAYER_BUILDING ]->BuildStructure( w, l, w, l, OC_STRUCTURE_FLORA, cost );
		}
	}

	delete [] treeDensity;
}


   /*=====================================================================*/
void City::_CreateSimulator()
{
// Simulators' initialization
	_pMSim = new MainSim( gVars.gpmutexSim, (BuildingLayer*)_apLayer[ OC_LAYER_BUILDING ], gVars.gpMapMgr );

// Now initialize simulators threads
	_pthreadMSim = SDL_CreateThread( Simulator::ThreadWrapper, _pMSim );

// Kept for future reference
// How can I put funcTSim into the TrafficSim class ?
//
//	int (*fn)(void*);
//	fn = reinterpret_cast<int (*)(void*)>(&TrafficSim::Run);
//	pthreadTSim = SDL_CreateThread( TrafficSim::Run, pTSim );
//

// Put all the simulators' threads into RUN state
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
	ostringstream ossTemp;

// The status bar
	pbtnPause = new GUIButton( 54, 4, 24, 24, ocHomeDirPrefix( "graphism/gui/status/speed_pause" ));
	pbtnPlay  = new GUIButton( 54, 4, 24, 24, ocHomeDirPrefix( "graphism/gui/status/speed_play" ));
	pbtnPause->Unset( OC_GUIMAIN_VISIBLE );

	ossTemp << _liCityFund;
	plblFund = new GUILabel( 125, 11, 80, 10, ossTemp.str() );
	plblFund->SetAlign( GUILabel::OC_ALIGN_RIGHT );
	plblFund->SetForeground( OPENCITY_PALETTE[Color::OC_WHITE] );

	ossTemp.str("");
	ossTemp << _uiPopulation;
	plblPopulation = new GUILabel( 240, 11, 80, 10, ossTemp.str() );
	plblPopulation->SetAlign( GUILabel::OC_ALIGN_RIGHT );
	plblPopulation->SetForeground( OPENCITY_PALETTE[Color::OC_WHITE] );

	ossTemp.str("");
	ossTemp << _uiDay << "/" << _uiMonth << "/" << _uiYear;
	plblDate = new GUILabel( 348, 11, 80, 10, ossTemp.str() );
	plblDate->SetAlign( GUILabel::OC_ALIGN_CENTER );
	plblDate->SetForeground( OPENCITY_PALETTE[Color::OC_WHITE] );

	pbarResidence = new GUIBar( 5, 5, 7, 53 );
	pbarResidence->SetForeground( OPENCITY_PALETTE[Color::OC_GREEN] );

	pbarCommerce = new GUIBar( 18, 5, 7, 53 );
	pbarCommerce->SetForeground( OPENCITY_PALETTE[Color::OC_BLUE] );

	pbarIndustry = new GUIBar( 29, 5, 7, 53 );
	pbarIndustry->SetForeground( OPENCITY_PALETTE[Color::OC_YELLOW] );

	pbarPower = new GUIBar( 42, 5, 7, 53 );
	pbarPower->SetForeground( OPENCITY_PALETTE[Color::OC_PINK] );

	pctrStatus = new GUIContainer( (_iWinWidth-512) / 2, 0, 512, 64, ocHomeDirPrefix( "graphism/gui/main_status_bar.png" ));
	pctrStatus->Add( pbtnPause );
	pctrStatus->Add( pbtnPlay );
	pctrStatus->Add( plblFund );
	pctrStatus->Add( plblPopulation );
	pctrStatus->Add( plblDate );
	pctrStatus->Add( pbarResidence );
	pctrStatus->Add( pbarCommerce );
	pctrStatus->Add( pbarIndustry );
	pctrStatus->Add( pbarPower );
	pctrStatus->Set( OC_GUIMAIN_VISIBLE );

// GUI main toolcircle
	pbtnZ = new GUIButton(  20,  70, 30, 30, ocHomeDirPrefix( "graphism/gui/residential" ));
	pbtnS = new GUIButton( 100,  70, 30, 30, ocHomeDirPrefix( "graphism/gui/save" ));
	pbtnL = new GUIButton(  20,  20, 30, 30, ocHomeDirPrefix( "graphism/gui/power" ));
	pbtnP = new GUIButton(  60,   0, 30, 30, ocHomeDirPrefix( "graphism/gui/road" ));
	pbtnX = new GUIButton(  100, 20, 30, 30, ocHomeDirPrefix( "graphism/gui/bulldozer" ));
	pbtnG = new GUIButton(  60,  90, 30, 30, ocHomeDirPrefix( "graphism/gui/government" ));

	pctrMain = new GUIContainer( 100, 100, 140, 140 );
	pctrMain->Add( pbtnZ );
	pctrMain->Add( pbtnS );
	pctrMain->Add( pbtnL );
	pctrMain->Add( pbtnP );
	pctrMain->Add( pbtnX );
	pctrMain->Add( pbtnG );


// GUI Z toolcircle for RCI buttons
	pbtnZB = new GUIButton( 20,  70,  30, 30, ocHomeDirPrefix( "graphism/gui/back" ));
	pbtnZR = new GUIButton( 20,  20,  30, 30, ocHomeDirPrefix( "graphism/gui/residential" ));
	pbtnZC = new GUIButton( 60,  0,   30, 30, ocHomeDirPrefix( "graphism/gui/commercial" ));
	pbtnZI = new GUIButton( 100, 20,  30, 30, ocHomeDirPrefix( "graphism/gui/industrial" ));

	pctrZ = new GUIContainer( 100, 100, 140, 140 );
	pctrZ->Add( pbtnZB );
	pctrZ->Add( pbtnZR );
	pctrZ->Add( pbtnZC );
	pctrZ->Add( pbtnZI );


// GUI L toolcircle ( electric lines, electric plants )
	pbtnLB = new GUIButton( 20,  20,  30, 30, ocHomeDirPrefix( "graphism/gui/back" ));
	pbtnLL = new GUIButton( 60,  0,   30, 30, ocHomeDirPrefix( "graphism/gui/power_line" ));
	pbtnLN = new GUIButton( 100, 20,  30, 30, ocHomeDirPrefix( "graphism/gui/power_plant_nuclear" ));
	pbtnLC = new GUIButton( 100, 70,  30, 30, ocHomeDirPrefix( "graphism/gui/power_plant_coal" ));

	pctrL = new GUIContainer( 100, 100, 140, 140 );
	pctrL->Add( pbtnLB );
	pctrL->Add( pbtnLL );
	pctrL->Add( pbtnLN );
	pctrL->Add( pbtnLC );


// GUI T toolcircle ( raise, lower terrain )
	pbtnTU = new GUIButton( 20,  20,  30, 30, ocHomeDirPrefix( "graphism/gui/raise" ));
	pbtnTD = new GUIButton( 60,  0,   30, 30, ocHomeDirPrefix( "graphism/gui/lower" ));
	pbtnTB = new GUIButton( 100, 20,  30, 30, ocHomeDirPrefix( "graphism/gui/back" ));
	pbtnTX = new GUIButton( 20,  70,  30, 30, ocHomeDirPrefix( "graphism/gui/destroy" ));
	pbtnTQ = new GUIButton( 100, 70, 30, 30, ocHomeDirPrefix( "graphism/gui/query" ));

	pctrT = new GUIContainer( 100, 100, 140, 140 );
	pctrT->Add( pbtnTB );
	pctrT->Add( pbtnTU );
	pctrT->Add( pbtnTD );
	pctrT->Add( pbtnTX );
	pctrT->Add( pbtnTQ );


// GUI Gouvernement toolcircle ( park, education, hospital, police and fire )
	pbtnGB = new GUIButton(  60,  90, 30, 30, ocHomeDirPrefix( "graphism/gui/back" ));
	pbtnGP = new GUIButton(  20,  70, 30, 30, ocHomeDirPrefix( "graphism/gui/park" ));
	pbtnGE = new GUIButton( 100,  70, 30, 30, ocHomeDirPrefix( "graphism/gui/education" ));
	pbtnGH = new GUIButton(  20,  20, 30, 30, ocHomeDirPrefix( "graphism/gui/hospital" ));
	pbtnGL = new GUIButton(  60,   0, 30, 30, ocHomeDirPrefix( "graphism/gui/police" ));
	pbtnGF = new GUIButton(  100, 20, 30, 30, ocHomeDirPrefix( "graphism/gui/fire" ));

	pctrG = new GUIContainer( 100, 100, 140, 140 );
	pctrG->Add( pbtnGB );
	pctrG->Add( pbtnGP );
	pctrG->Add( pbtnGE );
	pctrG->Add( pbtnGH );
	pctrG->Add( pbtnGL );
	pctrG->Add( pbtnGF );


// Create the nature container
	pbtnNB = new GUIButton(  20,  70, 30, 30, ocHomeDirPrefix( "graphism/gui/back" ));
	pbtnNP = new GUIButton(  60,  90, 30, 30, ocHomeDirPrefix( "graphism/gui/park" ));
	pbtnNT = new GUIButton( 100,  70, 30, 30, ocHomeDirPrefix( "graphism/gui/tree" ));

	pctrN = new GUIContainer( 100, 100, 140, 140 );
	pctrN->Add( pbtnNB );
	pctrN->Add( pbtnNP );
	pctrN->Add( pbtnNT );


// Create save/load buttons and the container
	pbtnSL = new GUIButton(  20,  70, 30, 30, ocHomeDirPrefix( "graphism/gui/save_load" ));
	pbtnSS = new GUIButton(  60,  90, 30, 30, ocHomeDirPrefix( "graphism/gui/save_save" ));
	pbtnSB = new GUIButton( 100,  70, 30, 30, ocHomeDirPrefix( "graphism/gui/back" ));

	pctrS = new GUIContainer( 100, 100, 140, 140 );
	pctrS->Add( pbtnSB );
	pctrS->Add( pbtnSS );
	pctrS->Add( pbtnSL );


// MAS toolcircle
	pctrMAS = new GUIContainer( 100, 100, 140, 140 );
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
// Delete the status bar
	delete pctrStatus;
	delete pbarPower;
	delete pbarIndustry;
	delete pbarCommerce;
	delete pbarResidence;
	delete plblFund;
	delete plblPopulation;
	delete plblDate;
	delete pbtnPlay;
	delete pbtnPause;

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
	delete pbtnLN;
	delete pbtnLC;

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
	if ( _eCurrentTool == OC_NONE )
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
	SDL_LockMutex( gVars.gpmutexSim );

	switch (_eCurrentTool) {
	case OC_ZONE_RES:
		if ((enumErrCode = _apLayer[ _eCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_RES, cost )) == OC_ERR_FREE) {
			gVars.gpAudioMgr->PlaySound( OC_SOUND_RCI );
		}
		break;

	case OC_ZONE_COM:
		if ((enumErrCode = _apLayer[ _eCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_COM, cost )) == OC_ERR_FREE) {
			gVars.gpAudioMgr->PlaySound( OC_SOUND_RCI );
		}
		break;

	case OC_ZONE_IND:
		if ((enumErrCode = _apLayer[ _eCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_IND, cost )) == OC_ERR_FREE) {
			gVars.gpAudioMgr->PlaySound( OC_SOUND_RCI );
		}
		break;

	case OC_BUILD_ROAD:
		if ((enumErrCode = _apLayer[ _eCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_ROAD, cost )) == OC_ERR_FREE) {
			gVars.gpAudioMgr->PlaySound( OC_SOUND_ROAD );
		}
		break;

	case OC_BUILD_ELINE:
		if ((enumErrCode = _apLayer[ _eCurrentLayer ]->
			BuildStructure( 
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_ELINE, cost )) == OC_ERR_FREE) {
			gVars.gpAudioMgr->PlaySound( OC_SOUND_ELINE );
		}
		break;

	case OC_BUILD_EPLANT_COAL:
		if ((enumErrCode = _apLayer[ _eCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_EPLANT_COAL, cost )) == OC_ERR_FREE) {
			_pMSim->AddStructure( uiMapW1, uiMapL1, uiMapW2, uiMapL2, Simulator::OC_ELECTRIC );
			gVars.gpAudioMgr->PlaySound( OC_SOUND_EPLANT );
		}
		break;

	case OC_BUILD_EPLANT_NUCLEAR:
		if ((enumErrCode = _apLayer[ _eCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_EPLANT_NUCLEAR, cost )) == OC_ERR_FREE) {
			_pMSim->AddStructure( uiMapW1, uiMapL1, uiMapW2, uiMapL2, Simulator::OC_ELECTRIC );
			gVars.gpAudioMgr->PlaySound( OC_SOUND_EPLANT );
		}
		break;

	case OC_BUILD_PARK:
		if ((enumErrCode = _apLayer[ _eCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_PARK, cost )) == OC_ERR_FREE) {
			gVars.gpAudioMgr->PlaySound( OC_SOUND_PARK );
		}
		break;

	case OC_BUILD_FLORA:
		if ((enumErrCode = _apLayer[ _eCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_FLORA, cost )) == OC_ERR_FREE) {
			gVars.gpAudioMgr->PlaySound( OC_SOUND_PARK );
		}
		break;

	case OC_BUILD_FIRE:
		if ((enumErrCode = _apLayer[ _eCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_FIREDEPT, cost )) == OC_ERR_FREE) {
// not used			_pMSim->AddStructure( uiMapW1, uiMapL1, uiMapW2, uiMapL2, MainSim::OC_MICROSIM_ELE );
			gVars.gpAudioMgr->PlaySound( OC_SOUND_EPLANT );
		}
		break;

	case OC_BUILD_POLICE:
		if ((enumErrCode = _apLayer[ _eCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_POLICEDEPT, cost )) == OC_ERR_FREE) {
// not used			_pMSim->AddStructure( uiMapW1, uiMapL1, uiMapW2, uiMapL2, MainSim::OC_MICROSIM_ELE );
			gVars.gpAudioMgr->PlaySound( OC_SOUND_EPLANT );
		}
		break;

	case OC_BUILD_HOSPITAL:
		if ((enumErrCode = _apLayer[ _eCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_HOSPITALDEPT, cost )) == OC_ERR_FREE) {
// not used			_pMSim->AddStructure( uiMapW1, uiMapL1, uiMapW2, uiMapL2, MainSim::OC_MICROSIM_ELE );
			gVars.gpAudioMgr->PlaySound( OC_SOUND_EPLANT );
		}
		break;

	case OC_BUILD_EDUCATION:
		if ((enumErrCode = _apLayer[ _eCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_EDUCATIONDEPT, cost )) == OC_ERR_FREE) {
// not used			_pMSim->AddStructure( uiMapW1, uiMapL1, uiMapW2, uiMapL2, MainSim::OC_MICROSIM_ELE );
			gVars.gpAudioMgr->PlaySound( OC_SOUND_EPLANT );
		}
		break;

	case OC_BUILD_TEST_BUILDING:
		if ((enumErrCode = _apLayer[ _eCurrentLayer ]->
			BuildStructure(
				uiMapW1, uiMapL1, uiMapW2, uiMapL2,
				OC_STRUCTURE_TEST, cost )) == OC_ERR_FREE) {
			gVars.gpAudioMgr->PlaySound( OC_SOUND_EPLANT );
		}
		break;

	case OC_BUILD_AGENT_POLICE:
		pstruct = _apLayer[ OC_LAYER_BUILDING ]->GetStructure( uiMapW1, uiMapL1 );
		if ((pstruct != NULL) && (pstruct->GetCode() == OC_STRUCTURE_ROAD))
		new AgentPolice(*gVars.gpKernel, *gVars.gpEnvironment, uiMapW1, uiMapL1);
		break;

	case OC_BUILD_AGENT_DEMONSTRATOR:
		pstruct = _apLayer[ OC_LAYER_BUILDING ]->GetStructure( uiMapW1, uiMapL1 );
		if ((pstruct != NULL) && (pstruct->GetCode() == OC_STRUCTURE_ROAD))
		new AgentDemonstrator(*gVars.gpKernel, *gVars.gpEnvironment, uiMapW1, uiMapL1);
		break;

	case OC_BUILD_AGENT_ROBBER:
		pstruct = _apLayer[ OC_LAYER_BUILDING ]->GetStructure( uiMapW1, uiMapL1 );
		if ((pstruct != NULL) && (pstruct->GetCode() == OC_STRUCTURE_ROAD))
		new AgentRobber(*gVars.gpKernel, *gVars.gpEnvironment, uiMapW1, uiMapL1);
		break;

//FIXME: cost
	case OC_HEIGHT_UP:
		enumErrCode = gVars.gpMapMgr->ChangeHeight( uiMapW1, uiMapL1, OC_MAP_UP );
		if ( enumErrCode == OC_ERR_FREE ) {
			gVars.gpRenderer->bHeightChange = true;
			gVars.gpAudioMgr->PlaySound( OC_SOUND_TERRAIN );
			cost = 5;		// Quick hack
		}
		break;

	case OC_HEIGHT_DOWN:
		enumErrCode = gVars.gpMapMgr->ChangeHeight( uiMapW1, uiMapL1, OC_MAP_DOWN );
		if ( enumErrCode == OC_ERR_FREE ) {
			gVars.gpRenderer->bHeightChange = true;
			gVars.gpAudioMgr->PlaySound( OC_SOUND_TERRAIN );
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
		pctrQ = _apLayer[ _eCurrentLayer ]->
			QueryStructure( uiMapW1, uiMapL1 );
	// reset the old container
		pctr->ResetAttribute(
			OC_GUIMAIN_CLICKED | OC_GUIMAIN_MOUSEOVER );
		pctr->Unset( OC_GUIMAIN_VISIBLE );
	// show the informations queried
		pctr = pctrQ;
		pctr->SetLocation(
			sdlMBEvent.x - 70,
			_iWinHeight - sdlMBEvent.y - 70 );
		pctr->Set( OC_GUIMAIN_VISIBLE );
		enumErrCode = OC_ERR_SOMETHING;		// avoid to calculate the cost
		break;

	case OC_DESTROY:
	// If it is a part of a bigger structure (an EPLANT for example),
	// the whole structure will be then destroyed
	// The following part tell the simulators to remove the collected data concerning
	// the structures which are going to be destroyed
		_pMSim->RemoveStructure( uiMapW1, uiMapL1, uiMapW2, uiMapL2 );
		enumErrCode = _apLayer[ _eCurrentLayer ]->
			DestroyStructure( uiMapW1, uiMapL1, uiMapW2, uiMapL2, cost );
		if (enumErrCode == OC_ERR_FREE) {
			gVars.gpAudioMgr->PlaySound( OC_SOUND_DESTROY );
		}
		break;

	default:
		enumErrCode = OC_ERR_SOMETHING;// which tool is this ?
	} // switch

// now unlock the mutex and let the sims run
	SDL_UnlockMutex( gVars.gpmutexSim );

	if (enumErrCode == OC_ERR_FREE) {
		_liCityFund -= cost;
	}
}


   /*=====================================================================*/
bool
City::_HandleKeyPressed()
{
	int actionFactor = 1;
	int key;
	bool boolKeyDown = false;		// There is no key pressed

// Key multiplier
	if (_abKeyPressed[KEY_ALT] == true)
		actionFactor = OC_ACTION_FACTOR;
	else
		actionFactor = 1;

// Look for pressed keys
	for (key = KEY_UP; key < KEY_NUMBER; key++) {
		if (_abKeyPressed[key] == true) {
			switch (key) {
			case KEY_UP:
				gVars.gpRenderer->MoveDown(actionFactor);
				break;
			case KEY_DOWN:
				gVars.gpRenderer->MoveUp(actionFactor);
				break;
			case KEY_RIGHT:
				gVars.gpRenderer->MoveLeft(actionFactor);
				break;
			case KEY_LEFT:
				gVars.gpRenderer->MoveRight(actionFactor);
				break;

			case KEY_PAGEUP:
				gVars.gpRenderer->RotateLeft(actionFactor);
				break;
			case KEY_PAGEDOWN:
				gVars.gpRenderer->RotateRight(actionFactor);
				break;

			case KEY_INSERT: // zoom in
				gVars.gpRenderer->ZoomIn();
				break;
			case KEY_DELETE: // zoom out
				gVars.gpRenderer->ZoomOut();
				break;
			} // switch

		// There's at least one key pressed
			boolKeyDown = true;
		} // if
	} // for

// Tell the caller that there's whether at least a key pressed or not
	return boolKeyDown;
}


   /*=====================================================================*/
void
City::_RecordRessource()
{
// Accumulate the income each month
	uint r, c, i;
	uint w = 0, e = 0, g = 0;
	r = _pMSim->GetValue(Simulator::OC_RESIDENTIAL);
	c = _pMSim->GetValue(Simulator::OC_COMMERCIAL);
	i = _pMSim->GetValue(Simulator::OC_INDUSTRIAL);

// Create a new record
	Ressource res;
	res.fund = _liCityFund;
	res.population = _uiPopulation;
	res.r = r;
	res.c = c;
	res.i = i;
	res.w = w;
	res.e = e;
	res.g = g;

// Pop the oldest record if available (safe)
	if (!_dqRessource.empty())
		_dqRessource.pop_front();
	_dqRessource.push_back(res);
}


   /*=====================================================================*/
void
City::_DoBill(
	const OPENCITY_PROPERTY_CODE & enumProperty )
{
	uint maintenance;
	uint index, surface;
	Structure* pStruct;
	uint r, c, i;

	surface = _uiWidth * _uiLength;
	maintenance = 0;
	for (index = 0; index < surface; index++) {
		pStruct = _apLayer[ OC_LAYER_BUILDING ]->GetLinearStructure( index );
		if (pStruct != NULL)
			maintenance +=
				gVars.gpPropertyMgr->Get(OC_MAINTENANCE_COST, pStruct->GetCode());
	}

	_liCityFund -= maintenance;

// Accumulate the income each month
	r = _pMSim->GetValue(Simulator::OC_RESIDENTIAL);
	c = _pMSim->GetValue(Simulator::OC_COMMERCIAL);
	i = _pMSim->GetValue(Simulator::OC_INDUSTRIAL);

	_uiIncome += (r*OC_R_INCOME_TAX + c*OC_C_INCOME_TAX + i*OC_I_INCOME_TAX) / 100;

// Add the income only if we reach the end of the year
	if (enumProperty == OC_INCOME ) {
	// Here is the gouvernment's help for this year :D
		_uiIncome += _uiIncome * OC_INCOME_HELP/100;
		_liCityFund += _uiIncome;
		OPENCITY_INFO(
			"Happy new year ! " <<
			"Income: " << _uiIncome <<
			" d/m/y: " << _uiDay << "/" << _uiMonth << "/" << _uiYear <<
			" R/C/I: " << r << "/" << c << "/" << i
		);

		_uiIncome = 0;
	}
}


   /*=====================================================================*/
void
City::_HandleStatusClick()
{
	uint uiObject = this->pctrStatus->GetClick();

// WARNING: the GUI button displays the current speed
	switch (uiObject) {
		case 1:		// Pause button
			OPENCITY_DEBUG( "Normal speed mode" );
			pbtnPause->Unset( OC_GUIMAIN_VISIBLE );
			pbtnPlay->Set( OC_GUIMAIN_VISIBLE );
			_eSpeed = OC_SPEED_NORMAL;
			_pMSim->Run();
			break;
		case 2:		// Play button
			OPENCITY_DEBUG( "Pause mode" );
			pbtnPlay->Unset( OC_GUIMAIN_VISIBLE );
			pbtnPause->Set( OC_GUIMAIN_VISIBLE );
			_eSpeed = OC_SPEED_PAUSE;
			_pMSim->Stop();
			break;

		default:
			OPENCITY_DEBUG( "WARNING: What's this control -> " << uiObject);
			assert(0);
			break;
	}

	pctrStatus->ResetAttribute( OC_GUIMAIN_CLICKED | OC_GUIMAIN_MOUSEOVER );
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
			_eCurrentTool = OC_BUILD_ROAD;
			_cTool = 'P';
			break;
		case 5: // T button, open the "Terrain" toolcircle
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
			_eCurrentTool = OC_ZONE_RES;
			_cTool = 'R';
			break;
		case 3:  // C button, set tool for "zone commercial"
			_eCurrentTool = OC_ZONE_COM;
			_cTool = 'C';
			break;
		case 4:  // I button, set tool for "zone industrial"
			_eCurrentTool = OC_ZONE_IND;
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
			_eCurrentTool = OC_BUILD_ELINE;
			_cTool = 'L';
			break;
		case 3:  // set tool for building nuclear power plant
			_eCurrentTool = OC_BUILD_EPLANT_NUCLEAR;
			_cTool = 'E';
			break;
		case 4:  // set tool for building coal power plant
			_eCurrentTool = OC_BUILD_EPLANT_COAL;
			_cTool = '?';
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
			_eCurrentTool = OC_HEIGHT_UP;
			_cTool = 'U';
			break;
		case 3:  // height down
			_eCurrentTool = OC_HEIGHT_DOWN;
			_cTool = 'D';
			break;
		case 4:  // destroy tool
			_eCurrentTool = OC_DESTROY;
			_cTool = 'X';
			break;
		case 5: // query tool
			_eCurrentTool = OC_QUERY;
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
			_eCurrentTool = OC_BUILD_EDUCATION;
			_cTool = '?';
			break;
		case 4:
			_eCurrentTool = OC_BUILD_HOSPITAL;
			_cTool = '?';
			break;
		case 5:
			_eCurrentTool = OC_BUILD_POLICE;
			_cTool = '?';
			break;
		case 6:
			_eCurrentTool = OC_BUILD_FIRE;
			_cTool = '?';
			break;

		default:
			OPENCITY_DEBUG( "WARNING: Unknown command" );
			assert(0);
			break;
	}

// the user clicked on the "nature" toolcircle
	else if (pctr == pctrN)
	switch (uiObject) {
		case 1: // back button, open the government toolcircle
			pctr = pctrG;
			pctr->Set( 2, OC_GUIMAIN_MOUSEOVER );
			break;
		case 2:  // build park
			_eCurrentTool = OC_BUILD_PARK;
			_cTool = '?';
			break;
		case 3:  // build tree
			_eCurrentTool = OC_BUILD_FLORA;
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
			pctr->Set( 2, OC_GUIMAIN_MOUSEOVER );
			break;
		case 2:  // save
			_Save( ocSaveDirPrefix( "opencity.save" ) );
			break;
		case 3:  // load
			_Load( ocSaveDirPrefix( "opencity.save" ) );
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
			_eCurrentTool = OC_NONE;
			boolPathGo = false;
//debug cout << "changed to false" << endl;
			break;
		case 2: // stop button
			_cTool = 'N';
			_eCurrentTool = OC_NONE;
			boolPathGo = true;
			this->uiVehicleType = Vehicle::VEHICLE_BUS;
			break;
		case 3: // stop button
			_cTool = 'N';
			_eCurrentTool = OC_NONE;
			boolPathGo = true;
			this->uiVehicleType = Vehicle::VEHICLE_SPORT;
//debug cout << "changed to true" << endl;
			break;
		case 4: // build test building
			_eCurrentTool = OC_BUILD_TEST_BUILDING;
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
			_eCurrentTool = OC_BUILD_AGENT_POLICE;
			break;
		case 2: // stop button
			_cTool = 'N';
			_eCurrentTool = OC_BUILD_AGENT_DEMONSTRATOR;
			break;
		case 3: // stop button
			_cTool = 'N';
			_eCurrentTool = OC_BUILD_AGENT_ROBBER;
			break;

		default:
			break;
	}


// IF the container has been changed then we reset
// the MouseOver & Clicked attributes
// otherwise we reset only the Clicked attribute
	if (pctr != pctrOld) {
		pctrOld->ResetAttribute( OC_GUIMAIN_CLICKED | OC_GUIMAIN_MOUSEOVER );
		pctrOld->GetLocation( iX, iY );
		pctr->SetLocation( iX, iY );
		pctr->Set( OC_GUIMAIN_VISIBLE );
	}
	else {
		pctrOld->ResetAttribute( OC_GUIMAIN_CLICKED );
	}
	pctrOld->Unset( OC_GUIMAIN_VISIBLE );
}


   /*=====================================================================*/
// Unused at the moment because it disturbs the GUI
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
		gVars.gpRenderer->MoveRight();
	if ((mouseX >= _iWinWidth-OC_MOUSE_AUTOSCROLL) && !(mouseY < OC_MOUSE_AUTOSCROLL))
		gVars.gpRenderer->MoveLeft();

// handle vertical automatic map translation
	if (!(mouseX < OC_MOUSE_AUTOSCROLL)
	  &&!(mouseX >= _iWinWidth-OC_MOUSE_AUTOSCROLL) && (mouseY < OC_MOUSE_AUTOSCROLL))
		gVars.gpRenderer->MoveDown();
	if ((mouseY >= _iWinHeight-OC_MOUSE_AUTOSCROLL))
		gVars.gpRenderer->MoveUp();

// handle map rotation
	if ((mouseX < OC_MOUSE_AUTOSCROLL) && (mouseY < OC_MOUSE_AUTOSCROLL))
		gVars.gpRenderer->RotateLeft();

	if ((mouseY < OC_MOUSE_AUTOSCROLL) && (mouseX >= _iWinWidth-OC_MOUSE_AUTOSCROLL))
		gVars.gpRenderer->RotateRight();
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
		//TODO: put this somewhere else
			vector<Destination> vdest;
	
			this->uiPathStopW = this->uiMapW2;
			this->uiPathStopH = this->uiMapL2;
	
		// Buses prefer short distance
			if (this->uiVehicleType == Vehicle::VEHICLE_BUS) {
				gVars.gpPathFinder->findShortestPath(
					uiPathStartW, uiPathStartH,
					uiPathStopW, uiPathStopH,
					vdest,
					PathFinder::OC_DISTANCE );
			}
		// Sport vehicle prefer less traffic
			else if (this->uiVehicleType == Vehicle::VEHICLE_SPORT) {
				gVars.gpPathFinder->findShortestPath(
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
				if (gVars.gpMoveMgr->Add( pvehicle ) < 0) {
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
	if (tcode != _eCurrentTool) {
		tcode = _eCurrentTool;
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

			case OC_BUILD_EPLANT_COAL:
				scode = OC_STRUCTURE_EPLANT_COAL;
				break;
			case OC_BUILD_EPLANT_NUCLEAR:
				scode = OC_STRUCTURE_EPLANT_NUCLEAR;
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
		ecode = _apLayer[ _eCurrentLayer ]->
			BuildPreview( uiMapW1, uiMapL1, scode, gcode );

		if (ecode == OC_ERR_FREE) {
			gVars.gpRenderer->DisplayBuildPreview(
				uiMapW1, uiMapL1, OC_GREEN_COLOR, gcode );
		}
		else {
			gVars.gpRenderer->DisplayBuildPreview(
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

// Save the signature and version
	string strSignature = "";
	strSignature = "OpenCity_" + ocStrVersion();
	long lVersion = ocLongVersion();

// FIXME: we use "space" to end a string
	fs << strSignature << " ";
	fs << lVersion << std::ends;

// Lock the simulator
	SDL_LockMutex( gVars.gpmutexSim );

// Save city data
	this->SaveTo( fs );

// Save map data
	gVars.gpMapMgr->SaveTo( fs );

// Save layers's data
	_apLayer[ OC_LAYER_BUILDING ]->SaveTo( fs );

// Save simulators data
	_pMSim->SaveTo( fs );

// Unlock the simulator
	SDL_UnlockMutex( gVars.gpmutexSim );

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
		OPENCITY_DEBUG( "File opening error in: " << strFilename );
		return false;
	}

// Load the signature and version
	string strSignature;
	long currentVersion = ocLongVersion();
	long lVersion;

	fs >> strSignature; fs.ignore();
	fs >> lVersion; fs.ignore();

// Version checking
	if (lVersion > currentVersion) {
		OPENCITY_INFO( "File version error in: " << strFilename );
		return false;
	}

// Lock the simulator
	SDL_LockMutex( gVars.gpmutexSim );

// Remove all moving objects
	gVars.gpMoveMgr->Remove();

// Load city data
	this->LoadFrom( fs );

// Load map data
	gVars.gpMapMgr->LoadFrom( fs );
	gVars.gpRenderer->bHeightChange = true;
	gVars.gpRenderer->bMinimapChange = true;

// Load layers' data
	_apLayer[ OC_LAYER_BUILDING ]->LoadFrom( fs );

// Load simulators' data
	_pMSim->LoadFrom( fs );

// Manually add the structures to the simulators
	for ( w = 0; w < _uiWidth; w++ ) {
		for ( l = 0; l < _uiLength; l++ ) {
			_pMSim->AddStructure( w, l, w, l );
		}
	}

// Refresh/recalculate the simulators' value
	_pMSim->RefreshSimValue();

// Unlock the simulator
	SDL_UnlockMutex( gVars.gpmutexSim );

	fs.close();
	return true;
}




































