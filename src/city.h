/***************************************************************************
							city.h  -  description
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

#ifndef _OPENCITY_CITY_H_
#define _OPENCITY_CITY_H_ 1

#include "main.h"

#include "persistence.h"
#include "ui.h"

#include <deque>

#define OC_MAX_RESSOURCE_RECORD	20


class PathFinder;		// testing pathfinding
class Vehicle;
class MovementManager;

class MainSim;			// main simulator which handles the micro simulators

class Layer;
class BuildingLayer;

class GUIBar;			// GUI, toolcircle
class GUIButton;	
class GUILabel;
class GUIContainer;


//========================================================================
/** This structure is used to keep track of city ressources
*/
struct Ressource {
	OC_LINT		fund;			///< Money, money, money !
	uint		population;		///< Number of "oczens"
	uint		r, c, i;		///< RCI values
	uint		w, e, g;		///< WEG values
};


//========================================================================
/** The game is here ! This class controls all other classes.
*/
class City : public Persistence, public UI
{
public:
	City(
		const uint width = OC_CITY_W,
		const uint length = OC_CITY_L,
		const int difficulty = 2,
		const bool bGUIEnabled = true );

	~City();


//========================================================================
/** Save the data to the specified fstream
	\param rfs A reference to a file stream which is ready for writing
*/
	void
	SaveTo( std::fstream& rfs );


//========================================================================
/** Load the data from the specified stream
	\param rfs A reference to a file stream which is ready for reading
*/
	void
	LoadFrom( std::fstream& rfs );


//========================================================================
	void
	SetCurrentLayer( OPENCITY_CITY_LAYER enumNewLayer );


//========================================================================
/** Process the city's task
*/
	void Run();


//========================================================================
/** Update the display
*/
	void Display();


	Layer*
	GetLayer( OPENCITY_CITY_LAYER enumLayer ) const;


	const void GetWL(
		uint & w, uint & l ) const;


//========================================================================
// Inherited methods from UI
//========================================================================
	void Keyboard( const SDL_KeyboardEvent& rcEvent );
	void MouseMotion( const SDL_MouseMotionEvent& rcEvent );
	void MouseButton( const SDL_MouseButtonEvent& rcEvent );
	void Expose( const SDL_ExposeEvent& rcEvent );
	void Resize( const SDL_ResizeEvent& rcEvent );


private:
// City's informations
	string		strCityName;		///< The name of the city
	int			iDifficulty;		///< The difficulty of current game
	bool		_bGUIEnabled;		///< GUI mode ?
	string		strFileName;		///< The fileName of current city

	uint		_uiIncome;			///< Monthly income accumulator
	OC_LINT		_liCityFund;		///< Money, money, money !
	uint		_uiPopulation;		///< Number of "oczens"
	std::deque<Ressource>	_dqRessource;	///< Store the last city ressources

	uint		_uiDay;
	uint		_uiMonth;
	uint		_uiYear;

	uint		_uiWidth;			///< City's width, length
	uint		_uiLength;

	OC_CHAR		_cTool;				///< Tool hotkey
	int			_iWinWidth;			///< Windows' width, height
	int			_iWinHeight;

// Dragging mode and mouse click
	bool		boolLMBPressed;
	uint		uiMapW1;
	uint		uiMapL1;
	uint		uiMapW2;
	uint		uiMapL2;

// Handle pressed keyboards
	bool		booltabKeyPressed [KEY_NUMBER];

// Current city's layer
	OPENCITY_CITY_LAYER enumCurrentLayer;

// Current city's speed
	OPENCITY_CITY_SPEED _eSpeed;

// Current active tool
	OPENCITY_TOOL_CODE enumCurrentTool;


// Others classes
	Layer*		ptabLayer [4];			// table of our 4 differents layer

// Testing pathfinder, TODO maybe we need to move it to main.cpp
	bool		boolPathGo;
	uint		uiPathStartW, uiPathStartH;
	uint		uiPathStopW, uiPathStopH;
	GUIContainer* pctrPath;
	GUIButton*	pbtnPathStart;
	GUIButton*	pbtnPathStop1;
	GUIButton*	pbtnPathStop2;
	GUIButton*	pbtnTestBuilding;		// Used by graphists
	Vehicle*	pvehicle;
	uint		uiVehicleType;

// Testing MAS
	GUIContainer* pctrMAS;
	GUIButton*	pbtnMAS;
	GUIButton*	pbtnMASPolice;
	GUIButton*	pbtnMASDemonstrator;
	GUIButton*	pbtnMASRobber;

// Simulator
	MainSim*	_pMSim;
	SDL_Thread*	_pthreadMSim;

// GUI Splash screen
//debug, test
	GUIButton*	pbtnSplash;

// The status bar
	GUIContainer* pctrStatus;	// The main status bar container
	GUILabel* plblFund;			// The fund label
	GUILabel* plblPopulation;	// The population label
	GUILabel* plblDate;			// The date label
	GUIBar* pbarResidence;		// The residence bar
	GUIBar* pbarCommerce;		// The commerce bar
	GUIBar* pbarIndustry;		// The industry bar
	GUIBar* pbarPower;			// The power bar
	GUIButton* pbtnPause;		// Pause button
	GUIButton* pbtnPlay;		// Play button

// GUI containers for main, zone, eLectric and Terrain toolcircles
	GUIContainer* pctr;
	GUIContainer* pctrMain;
	GUIContainer* pctrL;		// electricic tool
	GUIContainer* pctrT;		// terrain tool
	GUIContainer* pctrQ;		// query tool
	GUIContainer* pctrZ;		// zone tool
	GUIContainer* pctrG;		// Government tool circle
	GUIContainer* pctrN;		// Nature tool circle
	GUIContainer* pctrS;		// Save tool circle

// Main container, Z, LPX buttons
	GUIButton* pbtnZ;		// RCI zoning tools
	GUIButton* pbtnS;		// save/load tools
	GUIButton* pbtnL;		// electric Line
	GUIButton* pbtnP;		// Path
	GUIButton* pbtnX;		// Xestroy
	GUIButton* pbtnG;		// Government tool

// RCI buttons
	GUIButton* pbtnZB;		// Back button
	GUIButton* pbtnZR;		// Residential
	GUIButton* pbtnZC;		// Commercial
	GUIButton* pbtnZI;		// Industrial

// EL buttons
	GUIButton* pbtnLB;		// Back button
	GUIButton* pbtnLL;		// Electric lines
	GUIButton* pbtnLN;		// Nuclear power plant
	GUIButton* pbtnLC;		// Coal power plant

// (U)p / (D)own buttons
	GUIButton* pbtnTB;		// Back button
	GUIButton* pbtnTU;		// Raise terrain
	GUIButton* pbtnTD;		// Lower terrain
	GUIButton* pbtnTX;		// Destroy tool
	GUIButton* pbtnTQ;		// query button

// (P)ark buttons
	GUIButton* pbtnGB;		// Back button
	GUIButton* pbtnGP;		// Build a park
	GUIButton* pbtnGE;		// Education department
	GUIButton* pbtnGH;		// Hospital
	GUIButton* pbtnGL;		// Police department
	GUIButton* pbtnGF;		// Fire department

// Nature buttons
	GUIButton* pbtnNB;		// Back button
	GUIButton* pbtnNP;		// Park
	GUIButton* pbtnNT;		// Tree

// Save buttons
	GUIButton* pbtnSB;		// Back button
	GUIButton* pbtnSS;		// Save
	GUIButton* pbtnSL;		// Load


   /*=====================================================================*/
   /*                        PRIVATE     METHODS                          */
   /*=====================================================================*/
//========================================================================
/** Create all the threads which handle the simulation aspect
	\sa _DeleteSimulator()
*/
	void _CreateSimulator();


//========================================================================
/** Wait for the simulation thread's exit
	\sa _CreateSimulator()
*/
	void _DeleteSimulator();


//========================================================================
/** Create the Graphic User Interface or ToolCircle
	\sa _DeleteGUI()
*/
	void _CreateGUI();

//========================================================================
/** Free the memory used by the GUI
	\sa _CreateGUI()
*/
	void _DeleteGUI();

	void _DoTool( const SDL_MouseButtonEvent & sdlMBEvent );

	bool _HandleKeyPressed();

//========================================================================
/** Save the ressource record to the queue
*/
	void _RecordRessource();

	void _DoBill( const OPENCITY_PROPERTY_CODE & );

	void  _HandleStatusClick();

	void _HandleGUIClick();

	void _HandleMouseXY();

	void _TestPathfinding();

	void _BuildPreview();


//========================================================================
/** Save the game
	\return true if everything goes fine, false otherwise
*/
	bool
	_Save( const string& strFilename );

//========================================================================
/** Save the game
	\return true if everything goes fine, false otherwise
*/
	bool
	_Load( const string& strFilename );

};

#endif



















