/***************************************************************************
                          city.h    -  description
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

#ifndef _OPENCITY_CITY_H_
#define _OPENCITY_CITY_H_ 1

#include "main.h"

#include "persistence.h"
#include "ui.h"


class PathFinder;		// testing pathfinding
class Vehicle;
class MovementManager;

class MainSim;			// main simulator which handles the micro simulators

class Layer;
class BuildingLayer;

class GUIButton;		// GUI, toolcircle
class GUIContainer;


   /** The game is here ! This class controls all other classes.
   */
class City : public Persistence, public UI
{
public:
	City(
		const bool & rcUseDL = true,
		const uint & width = OC_CITY_W,
		const uint & height = OC_CITY_H,
		const OC_DATE & foundedDate = 0,
		const int & difficulty = 2 );

	City( const int & rciFileHandle );

	~City(  );

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
   // Inherited methods from UI
   //========================================================================
	void
	uiKeyboard( const SDL_KeyboardEvent & rcsSDLKeyboardEvent );

	void
	uiMouseMotion( const SDL_MouseMotionEvent & rcsSDLMouseMotionEvent );

	void
	uiMouseButton( const SDL_MouseButtonEvent & rcsSDLMouseButtonEvent );

	void
	uiExpose( const SDL_ExposeEvent & rcsSDLExposeEvent );

	void
	uiResize( const SDL_ResizeEvent & rcsSDLResizeEvent );
   //========================================================================

	void
	SetCurrentLayer( OPENCITY_CITY_LAYER enumNewLayer );

	void
	Run( OPENCITY_CITY_SPEED enumSpeed = LAST_SPEED );

	Layer*
	GetLayer( OPENCITY_CITY_LAYER enumLayer ) const;

	const uint&
	cityGetWidth(  ) const;

	const uint&
	cityGetHeight(  ) const;



private:
// Current city's informations
	string		strCityName;		///< The name of the city
	int			iDifficulty;		///< The difficulty of current game
	string		strFileName;		///< The fileName of current city
	OC_DATE		cityFoundedDate;
	OC_LINT		_liCityFund;		///< Money, money, money !
	bool		boolModified;
/* not used
	uint		uiPoliceDpt;
	uint		uiFireDpt;
	uint		uiPark;
*/

	uint		_uiDay;
	uint		_uiMonth;
	uint		_uiYear;

	OC_CHAR		_cTool;				///< Tool hotkey

	uint		_uiWidth;			///< City's width, height
	uint		_uiHeight;

	int			iWinWidth;			///< Windows' width, height
	int			iWinHeight;

// Some GL options
	bool		boolUseDisplayList;

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
	OPENCITY_CITY_SPEED enumCurrentSpeed;

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
	GUIButton* pbtnLE;		// Electric plant

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

	void _DoTool(
		const SDL_MouseButtonEvent & sdlMBEvent );

	bool _HandleKeyPressed();

	void _DoBill( const OPENCITY_PROPERTY_CODE & );

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



















