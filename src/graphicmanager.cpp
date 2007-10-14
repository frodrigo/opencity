/***************************************************************************
						graphicmanager.cpp  -  description
							-------------------
	begin                : september 21th, 2003
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
#include "graphicmanager.h"
#include "structure.h"
#include "model.h"
#include "modelloader.h"
#include "movement.h"
#include "agent.h"
#include "conf.h"				// Parser for .conf file

// Global settings
#include "globalvar.h"
extern GlobalVar gVars;

// Standard headers
#include <cmath>				// For floorf
#include <sstream>				// We use std::stringstream for data conversion


static GLfloat red, green, blue;


   /*======================================================================*/
GraphicManager::GraphicManager():
_uiDisplayListMask( OC_OPAQUE_ONESIZE_LIST | OC_OPAQUE_TWOSIZE_LIST | OC_ALPHA_LIST )
{
	OPENCITY_DEBUG( "GraphicManager ctor" );

// Initialize the model table
	int i;
	for (i = 0; i < OC_GRAPHIC_CODE_MAX; i++ ) {
		tabpModel[i] = NULL;
	}

// Try to open the graphism config file
	Conf* pConf = new Conf();
	if (pConf->Open( ocHomeDirPrefix(OC_GRAPHISM_FILE_FILENAME) ) != OC_ERR_FREE) {
		OPENCITY_FATAL( "Failed to open the graphism config file" );
		delete pConf;
		abort();
	}

// Load all the specified models from the config file
	string strPath;
	std::stringstream ss;
	for (i = 0; i < OC_GRAPHIC_CODE_MAX; i++ ) {
		ss.str("");
		ss << i;
		strPath = pConf->GetValue( ss.str() );
		if (strPath == "")
			continue;

		tabpModel[i] = ModelLoader::Load( ocHomeDirPrefix(strPath) );
		if (tabpModel[i] == NULL) {
			OPENCITY_FATAL( "Failed to load the file: " << ocHomeDirPrefix(strPath) );
			abort();
		}
	} // for

	delete pConf;
}


   /*======================================================================*/
GraphicManager::~GraphicManager()
{
	OPENCITY_DEBUG( "GraphicManager dtor" );

	for (int i = 0; i < OC_GRAPHIC_CODE_MAX; i++ ) {
		delete tabpModel[i];	// delete NULL won't hurt
		tabpModel[i] = NULL;	// safe
	}
}


   /*======================================================================*/
const uint
GraphicManager::GetSelection(
	const uint & rcuiWinX,
	const uint & rcuiWinY ) const
{
	static GLubyte rgbTab[3];
	static GLshort uiValue = 0;
	static GLfloat fValue = .0;

//---- read the back buffer, double-buffering must be supported !
//	glReadBuffer( GL_BACK );
// 32 bits
	glReadPixels( rcuiWinX, rcuiWinY, 1, 1, GL_RGB, GL_FLOAT, &fValue );
	glReadPixels( rcuiWinX, rcuiWinY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, rgbTab );
	glReadPixels( rcuiWinX, rcuiWinY, 1, 1, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, &uiValue );

//debug begins
	cout << "WinX: " << rcuiWinX << "/ WinY: " << rcuiWinY << endl;
	cout << "RGB got: " << (int)rgbTab[0] << "/" << (int)rgbTab[1] << "/" << (int)rgbTab[2]
		 << "/ Float code: " << fValue
		 << "/ Uint code: " << uiValue;
	if (glGetError() != GL_NO_ERROR) {
		cout << "OMG: bug !" << endl;
	}

//debug ends

// 32 bpp
	return ((rgbTab[0] << 16) + (rgbTab[1] << 8) + rgbTab[2]);
// 16 bpp
//	return ((rgbTab[0] << 11) + (rgbTab[1] << 5) + rgbTab[2]);
//	return uiValue;
}


   /*======================================================================*/
void
GraphicManager::DisplayTerrain(
	const uint & rcuiW,
	const uint & rcuiL ) const
{
	static OC_BYTE tabH [4];
	static GLfloat ax, ay, az;
	static GLfloat bx, by, bz;
	static GLfloat nx, ny, nz;

	gVars.gpMapMgr->GetSquareHeight( rcuiW, rcuiL, tabH );

//debug begin
/*
cout << (int)tabH[0] << ":"
     << (int)tabH[1] << ":"
     << (int)tabH[2] << ":"
     << (int)tabH[3] << ":"
     << endl;
*/
//debug end

//WARNING: GL_COLOR_MATERIAL is enabled
	glColor3f( .25, .2, .15 );

// do we need to calculate the new normal ?
	if ((tabH[0] == tabH[1]) && (tabH[1] == tabH[2]) && (tabH[2] == tabH[3])) {
		glVertex3i( rcuiW,   tabH[0], rcuiL );
		glVertex3i( rcuiW,   tabH[1], rcuiL+1 );
		glVertex3i( rcuiW+1, tabH[2], rcuiL+1 );
		glVertex3i( rcuiW+1, tabH[3], rcuiL );
	}
	else {
	// calculate the new normal (the cross product)
		ax = .0; ay = (GLfloat)tabH[2]; az = 1.;
		bx = 1.; by = (GLfloat)tabH[1]; bz = .0;

		nx = by;
		ny = 1.;
		nz = ay;
		glNormal3f( nx, ny, nz );
/*
		cx = ay * bz - by * az;
		cy = bx * az - ax * bz;
		cz = ax * by - bx * ay;
*/

	// draw the stuff
		glVertex3i( rcuiW,   tabH[0], rcuiL );
		glVertex3i( rcuiW,   tabH[1], rcuiL+1 );
		glVertex3i( rcuiW+1, tabH[2], rcuiL+1 );
		glVertex3i( rcuiW+1, tabH[3], rcuiL );

	// then restore the default normal
		glNormal3f( 0., 0., 1. );
	}
}


   /*======================================================================*/
void
GraphicManager::DisplayStructure(
	const Structure* pcStructure,
	const uint & rcuiW,
	const uint & rcuiL ) const
{
	static OPENCITY_GRAPHIC_CODE enumGC;
	static OC_BYTE tabH [4];
	static GLfloat delta;

//NOTE: avoid segfault
	assert( pcStructure != NULL );

// Get the heights from the map manager
	gVars.gpMapMgr->GetSquareHeight( rcuiW, rcuiL, tabH );

// get the level of the structure
	delta = (GLfloat)(pcStructure->GetLevel() + 1) / 20;

/* debug test for the electricity conductivity
	if (pcStructure->IsSet( OC_STRUCTURE_E ))
		redE = .15;	//20
	else
		redE = .0;
*/


// Get the graphic code
	enumGC = pcStructure->GetGraphicCode();
	
//NOTE: null pointer can lead to SIGSEGV
	assert( (enumGC == OC_EMPTY) || (tabpModel[enumGC] != NULL) );

//NOTE: the "switch" instruction will be removed once we have enough models
	switch (enumGC) {
		case OC_RES_ZONE0:
		case OC_RES_ZONE1:
		case OC_RES_ZONE2:
		case OC_RES_ZONE3:
		case OC_RES_ZONE4:
		case OC_RES_ZONE5:
		case OC_RES_ZONE6:
		case OC_RES_ZONE7:
		case OC_RES_ZONE8:
		case OC_RES_ZONE9:

		case OC_COM_ZONE0:
		case OC_COM_ZONE1:
		case OC_COM_ZONE2:
		case OC_COM_ZONE3:
		case OC_COM_ZONE4:
		case OC_COM_ZONE5:
		case OC_COM_ZONE6:
		case OC_COM_ZONE7:
		case OC_COM_ZONE8:
		case OC_COM_ZONE9:

		case OC_IND_ZONE0:
		case OC_IND_ZONE1:
		case OC_IND_ZONE2:
		case OC_IND_ZONE3:
		case OC_IND_ZONE4:
		case OC_IND_ZONE5:
		case OC_IND_ZONE6:
		case OC_IND_ZONE7:
		case OC_IND_ZONE8:
		case OC_IND_ZONE9:

		case OC_PARK0:
		case OC_TREE_FIR:
		case OC_TREE_007:
		case OC_TREE_PEKINGWILLOW:
		case OC_TREE_PINE1:
		case OC_TREE_PINE2:
			tabpModel[enumGC]->DisplayList( rcuiW, rcuiL, tabH, _uiDisplayListMask );
			break;

	// Road part
		case OC_ROAD_O_N:
		case OC_ROAD_O_E:
		case OC_ROAD_O_S:
		case OC_ROAD_O_W:
		case OC_ROAD_S_N:
		case OC_ROAD_W_E:
		case OC_ROAD_N_E:
		case OC_ROAD_N_W:
		case OC_ROAD_S_E:
		case OC_ROAD_S_W:
		case OC_ROAD_S_N_E:
		case OC_ROAD_S_W_E:
		case OC_ROAD_S_N_W:
		case OC_ROAD_N_W_E:
		case OC_ROAD_S_N_W_E:
			tabpModel[enumGC]->Display2( rcuiW, rcuiL, tabH );
//			tabpModel[enumGC]->DisplayList( rcuiW, rcuiL, tabH );
			break;

	// Electric lines part
		case OC_ELINE_O_N:
		case OC_ELINE_O_E:
		case OC_ELINE_O_S:
		case OC_ELINE_O_W:
		case OC_ELINE_S_N:
		case OC_ELINE_W_E:
		case OC_ELINE_N_E:
		case OC_ELINE_N_W:
		case OC_ELINE_S_E:
		case OC_ELINE_S_W:
		case OC_ELINE_S_N_E:
		case OC_ELINE_S_W_E:
		case OC_ELINE_S_N_W:
		case OC_ELINE_N_W_E:
		case OC_ELINE_S_N_W_E:
			tabpModel[enumGC]->DisplayList( rcuiW, rcuiL, tabH, _uiDisplayListMask );
			break;

		case OC_EPLANT_COAL:
		case OC_EPLANT_NUCLEAR:
		case OC_FIRE_DEPT:
		case OC_POLICE_DEPT:
		case OC_HOSPITAL_DEPT:
		case OC_EDUCATION_DEPT:
			tabpModel[enumGC]->DisplayList( rcuiW, rcuiL, tabH, _uiDisplayListMask );
			break;

		case OC_EMPTY:
//debug cout << "empty" << endl;
			break;

		// Test codes used by graphists
		case OC_TEST_BUILDING:
			if (tabpModel[OC_TEST_BUILDING] != NULL) {
				tabpModel[OC_TEST_BUILDING]->DisplayList( rcuiW, rcuiL, tabH, _uiDisplayListMask );
			}
			break;

		default:
			red = .0; green = .0; blue = .0;
		// Display_draw_polygon:
			glBegin( GL_QUADS );
				glColor3f( red, green, blue );
				glVertex3i( rcuiW, tabH[0], rcuiL  );
				glVertex3i( rcuiW , tabH[1], (rcuiL+1)   );
				glVertex3i( rcuiW+1  , tabH[2], (rcuiL+1)   );
				glVertex3i( rcuiW+1  , tabH[3], rcuiL  );
			glEnd();
			break;
	}
}


   /*======================================================================*/
void
GraphicManager::DisplayGC(
	const OPENCITY_GRAPHIC_CODE & enumGC,
	const uint & rcuiW,
	const uint & rcuiL ) const
{
	static OC_BYTE tabH [4];

// Get the heights from the map manager
	gVars.gpMapMgr->GetSquareHeight( rcuiW, rcuiL, tabH );

//NOTE: null pointer can lead to SIGSEGV
	assert( (enumGC == OC_EMPTY) || (tabpModel[enumGC] != NULL) );

	tabpModel[enumGC]->DisplayList( rcuiW, rcuiL, tabH );
}


   /*======================================================================*/
void
GraphicManager::DisplayTerrainHighlight(
	const uint & rcuiW,
	const uint & rcuiL,
	const OPENCITY_TOOL_CODE & enumTool ) const
{
	OC_BYTE tabH [4];

// warning: see warning in DisplayTerrain
	gVars.gpMapMgr->GetSquareHeight( rcuiW, rcuiL, tabH );

// compare the tool code
	switch (enumTool) {
		case OC_TOOL_ZONE_RES:
			red = .2; green = .5; blue = .2;
			break;

		case OC_TOOL_ZONE_COM:
			red = .2; green = .2; blue = .5;
			break;

		case OC_TOOL_ZONE_IND:
			red = .5; green = .5; blue = .2;
			break;

		case OC_TOOL_ROAD:
			red = .5; green = .65; blue = .5;
			break;

		case OC_TOOL_ELINE:
			red = .55; green = .55; blue = .45;
			break;

		default:
			red = .1; green = .1; blue = .1;
	}


	glBegin( GL_QUADS );
		glColor3f( red, green, blue );
		glVertex3f( .1 + rcuiW, tabH[0], 0.1 + rcuiL);
		glVertex3f( .1 + rcuiW, tabH[1], 0.9 + rcuiL );
		glVertex3f( .9 + rcuiW, tabH[2], 0.9 + rcuiL );
		glVertex3f( .9 + rcuiW, tabH[3], 0.1 + rcuiL);
	glEnd();
}


   /*======================================================================*/
void
GraphicManager::DisplayStructureHighlight(
	const Structure* pcStructure,
	const uint & rcuiW,
	const uint & rcuiL,
	const OPENCITY_TOOL_CODE & enumTool ) const
{
//TODO: check the pcStructure graphic code
	DisplayTerrainHighlight(
		rcuiW,
		rcuiL,
		enumTool );
}


  /*======================================================================*/
void
GraphicManager::DisplayTerrainSelection(
	const uint & rcuiW,
	const uint & rcuiL,
	const uint & rcuiID ) const
{
	static OC_BYTE tabH [4];

// warning: we draw the polygon counter-clock wise
//          however, the polygon OY heights are
//          stored as the left,right heights of the first line
//          then left, right heights of the second line
	gVars.gpMapMgr->GetSquareHeight( rcuiW, rcuiL, tabH );

	glLoadName( rcuiID );
	glBegin( GL_QUADS );
		glVertex3i( rcuiW,   tabH[0], rcuiL);
		glVertex3i( rcuiW,   tabH[1], rcuiL+1 );
		glVertex3i( rcuiW+1, tabH[2], rcuiL+1 );
		glVertex3i( rcuiW+1, tabH[3], rcuiL);
	glEnd();
}


   /*======================================================================*/
void
GraphicManager::DisplayStructureSelection(
	const Structure* pcStructure,
	const uint & rcuiW,
	const uint & rcuiL,
	const uint & rcuiID ) const
{
	glLoadName( rcuiID );

	this->DisplayStructure( pcStructure, rcuiW, rcuiL );
}


   /*======================================================================*/
void
GraphicManager::Display(
	const OC_FLOAT & rcfW,
	const OC_FLOAT & rcfL,
	const OC_FLOAT & rcfH,
	const Movement* const pm ) const
{
	assert( pm != NULL );
//	static OC_BYTE tabH [4];

// warning: see warning in DisplayTerrain
//	gVars.gpMapMgr->GetSquareHeight( (uint)floorf(rcfW), (uint)floorf(rcfH), tabH );

	glPushMatrix();
// Rotation translation compensation
// The 0.05 value is the display translation vector of the other structures
	glTranslatef( rcfW+pm->_fTX, rcfH+pm->_fTY+0.05, rcfL+pm->_fTZ );
	glRotatef( pm->_fRY, 0, 1, 0 );			// Model rotation
	glRotatef( pm->_fRX, 1, 0, 0 );			// Slope
	glRotatef( pm->_fRZ, 0, 0, 1 );
	tabpModel[pm->GetGraphicCode()]->DisplayList();
	glPopMatrix();

/*
// FIXME: testing new AC3D vehicle models
	if (pm->GetGraphicCode() == OC_VEHICLE_STD) {
		tabpModel[OC_VEHICLE_STD]->DisplayPoly( rcfW, rcfH, tabH );
	}
	else {
		tabpModel[pm->GetGraphicCode()]->Display( rcfW, rcfH, tabH[0] );
	}
*/
}


   /*======================================================================*/
void
GraphicManager::SetLOD( const uint lod )
{
	switch (lod) {
		case OC_LOD_HIGH:
			_uiDisplayListMask = OC_OPAQUE_ONESIZE_LIST | OC_OPAQUE_TWOSIZE_LIST | OC_ALPHA_LIST;
			break;
		case OC_LOD_MEDIUM:
			_uiDisplayListMask = OC_OPAQUE_ONESIZE_LIST | OC_OPAQUE_TWOSIZE_LIST;
			break;
		case OC_LOD_LOW:
			_uiDisplayListMask = OC_OPAQUE_ONESIZE_LIST;
			break;
		default:
			break;
	}
}


   /*======================================================================*/

/* WARNING: this is a special function used for agent rendering */
void
GraphicManager::DisplayAgent(float x, float y, const Agent* const pAgent) const
 {
	static OC_BYTE tabH [4];

	assert(pAgent != NULL);

// warning: see warning in DisplayTerrain
	gVars.gpMapMgr->GetSquareHeight( (uint)floorf(x), (uint)floorf(y), tabH );
	tabpModel[pAgent->GetGraphicCode()]->DisplayList( x, y, tabH );
 }













