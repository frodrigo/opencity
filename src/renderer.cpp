/***************************************************************************
                          renderer.cpp  -  description
        $Id$
                             -------------------
    begin                : jeu mai 29 2003
    copyright            : (C) 2003-2006 by Duong-Khang NGUYEN
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

#include "renderer.h"

#include "layer.h"
#include "map.h"
#include "graphicmanager.h"
#include "propertymanager.h"
#include "texture.h"					// Terrain texturing

#include "font_8x8.h"					// 8x8 font definition

//WARNING: we can have more than one height map
extern GraphicManager* gpGraphicMgr;	// The global graphicmanager from main.cpp
extern PropertyManager* gpPropertyMgr;	// The global Property Manager
extern Map* gpMapMgr;					// The global mapmanager from main.cpp

extern SDL_Surface* gpVideoSrf;			// global video screen surface

   /*=====================================================================*/
Renderer::Renderer(
	const uint & cityW, const uint & cityH,
	const bool & rcboolUseDL ):
boolHeightChange( true ),
bDisplayGrid( true ),
bDisplayCompass( true ),
bWireFrame( false ),
ubProjectionType( OC_PERSPECTIVE ),
boolUseDisplayList( rcboolUseDL ),
uiCityWidth( cityW ),
uiCityHeight( cityH )
{
	OPENCITY_DEBUG( "Renderer ctor" );

// Load the terrain texture
	uiTerrainTex = Texture::Load( ocHomeDirPrefix( "texture/terrain_plane_128.png" ));

// Initialize the window's size, the viewport
// and set the perspective's ratio
	assert( gpVideoSrf != NULL );
	Renderer::SetWinSize( gpVideoSrf->w, gpVideoSrf->h );

// Settle "home" down ;)
	this->Home();


/* not need for the moment
//--- translate all the scence according to dDeltaX & dDeltaZ
	glTranslated( this->dDeltaX, 0.0, this->dDeltaZ );

//--- rotate the scence to the required angle
	glRotated( this->dYRotateAngle, 0.0, 1.0, 0.0 );
*/


// Enable polygon culling. This can effect the texture alpha blending
/*
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );
*/

// select the flat rendering mode, default is GL_SMOOTH
	glShadeModel( GL_FLAT );

//debug testing
//the maximal number of lights we can set is: 8
/*	GLint nbLight;
	glGetIntegerv( GL_MAX_LIGHTS, &nbLight );
	cout << "max lights : " << (int) nbLight << endl;
*/
//debug testing end

// Create 8x8 font
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	this->uiFontBase = glGenLists( 256 );
	for ( uint i = 0; i < 256; i++ ) {
		glNewList( this->uiFontBase + i, GL_COMPILE );
		glBitmap( 8, 8, .0, .0, 10., .0, fontdata_8x8 + i*8 );
		glEndList();
	}


// initialize few display lists
	if (boolUseDisplayList == true)
		uiGridList = glGenLists( 1 );

// Define the global ambient light value
// NOTE: we declare the variables here for better readable codes
	GLfloat fvLightModelAmbient[] = { OC_LIGHT_MODEL_AMBIENT };
	GLfloat fvLightAmbient [] = { .6, .6, .4, 1. };
	GLfloat fvLightDiffuse [] = { .8, .8, .6, 1. };
	GLfloat fvLightSpecular [] = { .4, .4, .4, 1. };

	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, fvLightModelAmbient );
	glLightfv( GL_LIGHT0, GL_AMBIENT, fvLightAmbient );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, fvLightDiffuse );
	glLightfv( GL_LIGHT0, GL_SPECULAR, fvLightSpecular );
	glLighti( GL_LIGHT0, GL_SPOT_CUTOFF, 180 );

// Enable lighting
// WARNING: the light position is transformed by the current MODELVIEW matrix
	static GLint ivLightPos [] = { 0, 2, 1, 0};		// Directional light
	static GLint ivLightDir [] = { 0, 2, 0 };
	glLightiv( GL_LIGHT0, GL_POSITION, ivLightPos );
	glLightiv( GL_LIGHT0, GL_SPOT_DIRECTION, ivLightDir );

	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );

// Enable the GL automatic normals rescaling it's useful when the user zooms
// in and out ( zooming is uniform here, in OpenCity)
// The surface's normal needs to be unit length
	glEnable( GL_RESCALE_NORMAL );
// GL_NORMALIZE is replaced by the RESCALE_NORMAL provided by OpenGL 1.2
//	glEnable( GL_NORMALIZE );

// Allow the material to track glColor()
// WARNING: glEnable( GL_COLOR_MATERIAL ) tracks immediatly the current color
//          The default settings are GL_FRONT_AND_BACK and GL_AMBIENT_AND_DIFFUSE
	glColor4ub( 255, 255, 255, 255 );
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glEnable( GL_COLOR_MATERIAL );

// this can help improving texture lighting
//	glLightModelf( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );

// Enable depth test
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_DEPTH_TEST );

// Choose the right blending function
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}


   /*=====================================================================*/
Renderer::~Renderer(  )
{
	OPENCITY_DEBUG("dtor");

// Destroy GL list
	glDeleteLists( this->uiFontBase, 256 );
	glDeleteLists( this->uiGridList, 1 );

// Free the terrain texture
	glDeleteTextures( 1, &this->uiTerrainTex );
}


   /*=====================================================================*/
void
Renderer::RotateLeft( const uint & factor )
{
	GLdouble dYCurrentAngle = OC_Y_ROTATE_STEP*factor;
	this->dYRotateAngle = (GLint)(this->dYRotateAngle + dYCurrentAngle) % 360;

// Toggle to the model view matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

// Rotate the scence to the required angle
	glRotated( dYCurrentAngle, 0.0, 1.0, 0.0 );

// Translate all the scence according to dDeltaX & dDeltaZ
	glTranslated( this->dDeltaX, 0.0, this->dDeltaZ );

// Apply the new changes to the old rotation's matrix
	glMultMatrixd( this->dmatrixRotate );

// Clear the translation
	this->dDeltaX = 0.0;
	this->dDeltaZ = 0.0;

// Save the calculated rotation matrix
	glGetDoublev( GL_MODELVIEW_MATRIX, dmatrixRotate );
}


   /*=====================================================================*/
void
Renderer::RotateRight( const uint & factor )
{
	GLdouble dYCurrentAngle = -OC_Y_ROTATE_STEP*factor;
	this->dYRotateAngle = (GLint)(this->dYRotateAngle + dYCurrentAngle) % 360;

// Toggle to the model view matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

// Rotate the scence to the required angle
	glRotated( dYCurrentAngle, 0.0, 1.0, 0.0 );

// Translate all the scence according to dDeltaX & dDeltaZ
	glTranslated( this->dDeltaX, 0.0, this->dDeltaZ );

// Apply the new changes to the old rotation's matrix
	glMultMatrixd( this->dmatrixRotate );

// Clear the translation
	this->dDeltaX = 0.0;
	this->dDeltaZ = 0.0;

// Save the calculated rotation matrix
	glGetDoublev( GL_MODELVIEW_MATRIX, dmatrixRotate );
}


   /*=====================================================================*/
void
Renderer::MoveLeft( const uint & factor )
{
	this->dDeltaX -= fXTransDelta*factor;
}


   /*=====================================================================*/
void
Renderer::MoveRight( const uint & factor )
{
	this->dDeltaX += fXTransDelta*factor;
}


   /*=====================================================================*/
void
Renderer::MoveUp( const uint & factor )
{
	this->dDeltaZ -= fZTransDelta*factor;
}


   /*=====================================================================*/
void
Renderer::MoveDown( const uint & factor )
{
	this->dDeltaZ += fZTransDelta*factor;
}


   /*=====================================================================*/
void
Renderer::Home()
{
	this->fScaleRatio = 1.0;
	this->fXTransDelta = OC_DELTA_X_STEP;
	this->fZTransDelta = OC_DELTA_Z_STEP;
	this->dEyeX = OC_EYE_X_START;
	this->dEyeY = OC_EYE_Y_START;
	this->dEyeZ = OC_EYE_Z_START;
	this->dDeltaX = OC_DELTA_X_START;
	this->dDeltaZ = OC_DELTA_Z_START;
	this->dYRotateAngle = OC_Y_ROTATE_ANGLE;

//--- reinit the rotation matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glGetDoublev( GL_MODELVIEW_MATRIX, dmatrixRotate );
}


   /*=====================================================================*/
void
Renderer::ZoomIn(  )
{
	if (this->fScaleRatio < 15.) {
		this->fScaleRatio += .1;
		if (this->fScaleRatio < 3.) {
			this->fXTransDelta -= .09;
			this->fZTransDelta -= .09;
		}
	}
}


   /*=====================================================================*/
void Renderer::ZoomOut(  )
{
	if (this->fScaleRatio > .1) {
		this->fScaleRatio -= .1;
		if (this->fScaleRatio < 3.) {
			this->fXTransDelta += .09;
			this->fZTransDelta += .09;
		}
	}
}


   /*=====================================================================*/
   /*
	Divide the map to something like this :
	_________
	| A | B |
	_________
	| C | D |
	_________

	A: 1, B: 2, C: 3, D: 4
	This algorithm help speed up a lot while calculating the selected
	map W,H on a big big map
   */
//TOKILL, for future reference
/*
const bool
Renderer::GetSelectedWHFromLayer(
		const uint & rcuiMouseX,
		const uint & rcuiMouseY,
		uint & ruiMapW, uint & ruiMapH,
		const Layer & rcLayer,
		const uint & rcW1, const uint & rcH1,
		const uint & rcW2, const uint & rcH2 ) const
{
#define MAX_MAP_DELTA 20 // cut the map WH down to this size in the worst case

//debug
//	cout << "W1,H1,W2,H2: " << rcW1 << "," << rcH1 << "," << rcW2 << "," << rcH2 << endl;
//debug
	if ((rcW2 - rcW1 > MAX_MAP_DELTA)
	 && (rcH2 - rcH1 > MAX_MAP_DELTA)) {
//debug
//	cout << "W1,H1,W2,H2: " << rcW1 << "," << rcH1 << "," << rcW2 << "," << rcH2 << endl;
//debug
		uint aW1 = rcW1; uint aH1 = rcH1;
		uint aW2 = (rcW1+rcW2) / 2; uint aH2 = (rcH1 + rcH2) / 2;

		uint bW1 = aW2 + 1; uint bH1 = aH1;
		uint bW2 = rcW2; uint bH2 = aH2;

		uint cW1 = aW1; uint cH1 = aH2 + 1;
		uint cW2 = aW2; uint cH2 = rcH2;

		uint dW1 = aW2 + 1; uint dH1 = aH2 + 1;
		uint dW2 = rcW2; uint dH2 = rcH2;

		GLubyte rgbTab[3];

	//---- prepare the world for rendering
		this->rendererPrepareView();

	//---- save the current ModelView matrix ----
		glPushMatrix();

	//---- clear the color buffer ( the screen )
		glClearColor( 0.0, 0.0, 0.0, 0.0 );
		glClear( GL_COLOR_BUFFER_BIT );

		glBegin( GL_QUADS );
		glColor3ub( 0, 0, 10 ); // 10 for polygon A
		glVertex3d( aW1 * OC_SQUARE_WIDTH, 0.0, aH1 * OC_SQUARE_HEIGHT );
		glVertex3d( aW1 * OC_SQUARE_WIDTH, 0.0, (aH2+1) * OC_SQUARE_HEIGHT );
		glVertex3d( (aW2+1) * OC_SQUARE_WIDTH, 0.0, (aH2+1) * OC_SQUARE_HEIGHT );
		glVertex3d( (aW2+1) * OC_SQUARE_WIDTH, 0.0, aH1 * OC_SQUARE_HEIGHT );

		glColor3ub( 0, 0, 20 ); // 20 for polygon B
		glVertex3d( bW1 * OC_SQUARE_WIDTH, 0.0, bH1 * OC_SQUARE_HEIGHT );
		glVertex3d( bW1 * OC_SQUARE_WIDTH, 0.0, (bH2+1) * OC_SQUARE_HEIGHT );
		glVertex3d( (bW2+1) * OC_SQUARE_WIDTH, 0.0, (bH2+1) * OC_SQUARE_HEIGHT );
		glVertex3d( (bW2+1) * OC_SQUARE_WIDTH, 0.0, bH1 * OC_SQUARE_HEIGHT );

		glColor3ub( 0, 0, 30 ); // 30 for polygon C
		glVertex3d( cW1 * OC_SQUARE_WIDTH, 0.0, cH1 * OC_SQUARE_HEIGHT );
		glVertex3d( cW1 * OC_SQUARE_WIDTH, 0.0, (cH2+1) * OC_SQUARE_HEIGHT );
		glVertex3d( (cW2+1) * OC_SQUARE_WIDTH, 0.0, (cH2+1) * OC_SQUARE_HEIGHT );
		glVertex3d( (cW2+1) * OC_SQUARE_WIDTH, 0.0, cH1 * OC_SQUARE_HEIGHT );

		glColor3ub( 0, 0, 40 ); // 40 for polygon D
		glVertex3d( dW1 * OC_SQUARE_WIDTH, 0.0, dH1 * OC_SQUARE_HEIGHT );
		glVertex3d( dW1 * OC_SQUARE_WIDTH, 0.0, (dH2+1) * OC_SQUARE_HEIGHT );
		glVertex3d( (dW2+1) * OC_SQUARE_WIDTH, 0.0, (dH2+1) * OC_SQUARE_HEIGHT );
		glVertex3d( (dW2+1) * OC_SQUARE_WIDTH, 0.0, dH1 * OC_SQUARE_HEIGHT );

		glEnd();
		glPopMatrix();

	//---- read the back buffer, double-buffering must be supported ! ----
		glReadBuffer( GL_BACK );
		glReadPixels( rcuiMouseX, iWinHeight-rcuiMouseY, 1, 1,
				GL_RGB, GL_UNSIGNED_BYTE, rgbTab );

		switch (rgbTab[2]) {
			case 0: return false;
			case 10: // polygon A;
				return GetSelectedWHFromLayer(
					rcuiMouseX, rcuiMouseY,
					ruiMapW, ruiMapH,
					rcLayer,
					aW1, aH1, aW2, aH2 );

			case 20: // polygon B;
				return GetSelectedWHFromLayer(
					rcuiMouseX, rcuiMouseY,
					ruiMapW, ruiMapH,
					rcLayer,
					bW1, bH1, bW2, bH2 );

			case 30: // polygon C;
				return GetSelectedWHFromLayer(
					rcuiMouseX, rcuiMouseY,
					ruiMapW, ruiMapH,
					rcLayer,
					cW1, cH1, cW2, cH2 );

			case 40: // polygon D;
				return GetSelectedWHFromLayer(
					rcuiMouseX, rcuiMouseY,
					ruiMapW, ruiMapH,
					rcLayer,
					dW1, dH1, dW2, dH2 );
		}  // end of switch (rgbTab[2])
	}
	else {
// the layer size is small enough so we consider each square as a polygon
		DisplayLayerSelection( rcLayer,
					rcW1, rcH1, rcW2, rcH2 );
		return GetSelectedMapWH( rcuiMouseX, rcuiMouseY,
						 ruiMapW, ruiMapH );
	}
// never reached
}
*/


   /*=====================================================================*/
void
Renderer::SetGrid( const bool & rcbState )
{
	this->bDisplayGrid = rcbState;
}


   /*=====================================================================*/
void
Renderer::ToggleGrid()
{
	this->bDisplayGrid = bDisplayGrid ? false : true;
}


   /*=====================================================================*/
void
Renderer::ToggleCompass()
{
	this->bDisplayCompass = !(bDisplayCompass);
}


   /*=====================================================================*/
void
Renderer::ToggleProjection()
{
	if (this->ubProjectionType == OC_PERSPECTIVE) {
		ubProjectionType = OC_ORTHOGONAL;
	}
	else {
		ubProjectionType = OC_PERSPECTIVE;
	}

// reinit the projection matrix;
	SetWinSize( this->iWinWidth, this->iWinHeight );
}

   /*=====================================================================*/
void
Renderer::ToggleWireFrame()
{
	this->bWireFrame = !(bWireFrame);
	if (this->bWireFrame) {
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	else {
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
}


   /*=====================================================================*/
void
Renderer::DisplaySplash(
	const uint & rcuiX,
	const uint & rcuiY ) const
{
	GLuint uiSplash;
	uint w, h;

	uiSplash = Texture::Load( ocHomeDirPrefix( "graphism/gui/splash.png"), w, h );

// Store and translate the splash to the specified OpenGL coordinates
	glPushAttrib( GL_ENABLE_BIT );
	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );
//	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); already choosen
	glEnable( GL_TEXTURE_2D );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glBindTexture( GL_TEXTURE_2D, uiSplash );

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(
		rcuiX + (this->iWinWidth-w) / 2, 
		rcuiY + (this->iWinHeight-h) / 2,
		0 );
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( 0, this->iWinWidth-1, 0, this->iWinHeight-1 );

//debug cout << "W: " << w << " /H: " << h << endl;

// Display the textured quad
	glBegin( GL_QUADS );
	glTexCoord2i( 0, 0 ); glVertex2i( 1, 1 );
	glTexCoord2i( 0, 1 ); glVertex2i( 1, h );
	glTexCoord2i( 1, 1 ); glVertex2i( w, h );
	glTexCoord2i( 1, 0 ); glVertex2i( w, 1 );
	glEnd();

// Retore the old matrix and attribs
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
	glPopAttrib();	
}


   /*=====================================================================*/
void
Renderer::Display(
	const Map* pcMap,
	const Layer* pcLayer )
{
	static uint linear;
	static int w, l;
	static const Structure* pStructure;

// Clear the color buffer ( the screen ) and the depth buffer
	glClearColor( OC_CLEAR_COLOR );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

//---- prepare the world for rendering
	this->rendererPrepareView();

// Call the private method to display the height map
	this->rendererDisplayTerrain();

// Display all the structure
	glTranslatef( 0., 0.05, 0. );
	linear = 0;
// IF the graphic manager is created THEN draw
	if (gpGraphicMgr != NULL)
	for (l = 0; l < (int)this->uiCityHeight; l++) {
		for (w = 0; w < (int)this->uiCityWidth; w++) {
			pStructure = pcLayer->GetLinearStructure( linear++ );
			if (pStructure != NULL)
				gpGraphicMgr->DisplayStructure( pStructure, w, l );
		}
	}

// Displays the grids of the map if the user wants it
// it is translated up a little along the Oz axis
	if (this->bDisplayGrid) {
		this->rendererDisplayMapGrid( pcMap );
	}

// Display the status bar
	this->rendererDisplayStatusBar();

	glFlush();

// GL error checking
	static GLint glerr;
	glerr = glGetError();
	if (glerr != GL_NO_ERROR) {
		OPENCITY_DEBUG( "GL ERROR" );
		cerr << "GLError was: " << glerr << endl;
	}
}


   /*=====================================================================*/
void
Renderer::DisplayHighlight(
	const Map* pcMap,
	const Layer* pcLayer,
	const OPENCITY_TOOL_CODE & enumTool )
{
	DisplayHighlight(
		pcMap,
		pcLayer,
		0,
		0,
		this->uiCityWidth-1,
		this->uiCityHeight-1,
		enumTool );
}


   /*=====================================================================*/
void
Renderer::DisplayHighlight(
	const Map* pcMap,
	const Layer* pcLayer,
	uint uiW1,
	uint uiL1,
	uint uiW2,
	uint uiL2,
	const OPENCITY_TOOL_CODE & enumTool )
{
	uint linear;
	uint w, l;
	const Structure * pStructure;

// Doing some swapping
	OPENCITY_SWAP( uiW1, uiW2, uint );
	OPENCITY_SWAP( uiL1, uiL2, uint );

// Display the current layer & map first
	Display( pcMap, pcLayer );

// Prepare the world for rendering
	this->rendererPrepareView();

// Now let's display all the structures in selection mode
	glTranslatef( 0., 0.1, 0. );
	for (l = uiL1; l <= uiL2; l++) {
		linear = l*this->uiCityWidth + uiW2;
		for (w = uiW1; w <= uiW2; w++) {
			pStructure = pcLayer->GetLinearStructure( linear );
		// display the correction structure/terrain
		// with "linear" as objectID
			if ( pStructure == NULL)
				gpGraphicMgr->DisplayTerrainHighlight( w, l, enumTool );
			else
				gpGraphicMgr->
				DisplayStructureHighlight( pStructure, w, l, enumTool );

			++linear;
		}
	}
}


   /*=====================================================================*/
void
Renderer::DisplayBuildPreview(
	const uint & uiW,
	const uint & uiL,
	const GLubyte & rcubR,
	const GLubyte & rcubG,
	const GLubyte & rcubB,
	const OPENCITY_GRAPHIC_CODE & gcode ) const
{
	static uint sw, sl, sh;

// Display the graphic code
	glTranslatef( 0., 0.15, 0. );
	gpGraphicMgr->DisplayGC( gcode, uiW, uiL );

// Get the graphic code dimensions in order to draw the bounding rectangle
	gpPropertyMgr->GetWLH( gcode, sw, 4, sl, 4, sh, 1 );
	glEnable( GL_BLEND );
	glTranslatef( 0., 0.10, 0. );
	glColor4ub( rcubR, rcubG, rcubB, 64 );
	glBegin( GL_QUADS );
		glVertex3i( uiW,    0, uiL   );
		glVertex3i( uiW,    0, uiL+sl );
		glVertex3i( uiW+sw, 0, uiL+sl );
		glVertex3i( uiW+sw, 0, uiL );
	glEnd();
	glDisable( GL_BLEND );
}


   /*=====================================================================*/
void
Renderer::DisplaySelection(
	const Map* pcMap,
	const Layer* pcLayer )
{
	DisplaySelection(
		pcMap,
		pcLayer,
		0,
		0,
		this->uiCityWidth-1,
		this->uiCityHeight-1 );
}


   /*=====================================================================*/
void
Renderer::DisplaySelection(
	const Map* pcMap,
	const Layer* pcLayer,
	const uint & rcuiW1,
	const uint & rcuiL1,
	const uint & rcuiW2,
	const uint & rcuiL2 )
{

}


   /*=====================================================================*/
/* TOKILL, old selection method, kept for reference
void
Renderer::DisplaySelection2(
	const Map* pcMap,
	const Layer* pcLayer,
	const uint & rcuiW1,
	const uint & rcuiL1,
	const uint & rcuiW2,
	const uint & rcuiL2 )
{
	uint linear;
	uint w, l;
	const Structure * pStructure;

// Save all the enabled states
	glPushAttrib( GL_ENABLE_BIT );
	glDisable( GL_LIGHTING );

// Clear the color buffer ( the screen )
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

// Prepare the world for rendering
	this->rendererPrepareView();

// Now let's display all the structures in selection mode
	glBegin( GL_QUADS );
	linear = 0;
	for (l = 0; l < this->uiCityHeight; l++) {
		for (w = 0; w < this->uiCityWidth; w++) {
			pStructure = pcLayer->GetLinearStructure( linear );
		// display the correction structure/terrain
		// with "linear" as objectID
		// note: linear = 0 is not used since it means blank
		// bland = there's no structure under the selection
			if ( pStructure == NULL)
				gpGraphicMgr->DisplayTerrainSelection( w, l, ++linear );
			else
				gpGraphicMgr->DisplayStructureSelection( pStructure, w, l, ++linear );
			//ATTENTION: "linear++;" already done !
		}
	}
	glEnd();

// Restore all the enabled states
	glPopAttrib();
	glFlush();

// GL error checking
	if (glGetError() != GL_NO_ERROR) {
		OPENCITY_DEBUG( "GL ERROR" );
	}
}
*/


   /*=====================================================================*/
void
Renderer::DisplayText(
	const uint & rcuiX,
	const uint & rcuiY,
	const GLubyte & rcubR,
	const GLubyte & rcubG,
	const GLubyte & rcubB,
	const string & rcText)
{
// Render in 2D, orthogonal projection
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( 0, this->iWinWidth-1, 0, this->iWinHeight-1 );

// Save the list base
	glTranslatef( .0, .0, .1 );
	glPushAttrib( GL_LIST_BIT | GL_ENABLE_BIT );
	glDisable( GL_LIGHTING );
	glListBase( this->uiFontBase );

	glColor4ub( rcubR, rcubG, rcubB, 255 );
	glRasterPos2i( rcuiX, rcuiY );
	glCallLists( (GLsizei)rcText.size(), GL_UNSIGNED_BYTE, (GLubyte*)rcText.c_str() );
	glPopAttrib();

	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
}


   /*=====================================================================*/
const bool
Renderer::GetSelectedWHFrom(
	const uint & rcuiMouseX,
	const uint & rcuiMouseY,
	uint & ruiW,
	uint & ruiL,
	const Map* pcMap,
	const Layer* pcLayer )
{
// call the right method with appropriate values
	return GetSelectedWHFrom(
		rcuiMouseX,
		rcuiMouseY,
		ruiW,
		ruiL,
		pcMap,
		pcLayer,
		0,
		0,
		this->uiCityWidth-1,
		this->uiCityHeight-1 );
}


   /*=====================================================================*/
const bool
Renderer::GetSelectedWHFrom(
	const uint & rcuiMouseX,
	const uint & rcuiMouseY,
	uint & ruiW,
	uint & ruiL,
	const Map* pcMap,
	const Layer* pcLayer,
	const uint & rcuiW1,
	const uint & rcuiL1,
	const uint & rcuiW2,
	const uint & rcuiL2 )
{
	static uint id;
	static uint linear;
	static uint w, l;
	static const Structure * pStructure;

	#define OC_SELECT_BUFFER_SIZE 100
	static GLuint selectBuffer[OC_SELECT_BUFFER_SIZE];
	static GLuint uiDepthMin;
	static GLint iHits;
	static GLint viewport[4] = {0, 0, 0, 0};

// Prepare the select buffer and enter selection mode
	glSelectBuffer( OC_SELECT_BUFFER_SIZE, selectBuffer );
	(void)glRenderMode( GL_SELECT );
	glInitNames();
	glPushName(0);

// Create the pick matrix
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	viewport[2] = this->iWinWidth;
	viewport[3] = this->iWinHeight;
	gluPickMatrix( rcuiMouseX, this->iWinHeight-rcuiMouseY, 2, 2, viewport );
	if ( this->ubProjectionType == OC_PERSPECTIVE ) {
		gluPerspective(
			OC_VIEW_ANGLE,
			(GLfloat)this->iWinWidth / (GLfloat)this->iWinHeight,
			OC_Z_NEAR, OC_Z_FAR );
	}
	else {
		glOrtho(
			0.0, (GLdouble)this->iWinWidth,
			0.0, (GLdouble)this->iWinHeight,
			OC_Z_NEAR_ORTHO, OC_Z_FAR);
	}

// Save all the enabled states
	glPushAttrib( GL_ENABLE_BIT );
	glDisable( GL_LIGHTING );

// Clear the color buffer ( the screen )
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

// Prepare the world for rendering
	this->rendererPrepareView();

// Now let's display all the structures in selection mode
	linear = 0;
	for (l = 0; l < this->uiCityHeight; l++) {
		for (w = 0; w < this->uiCityWidth; w++) {
			pStructure = pcLayer->GetLinearStructure( linear );
		// display the correction structure/terrain
		// with "linear" as objectID
		// note: linear = 0 is not used since it means blank
		// bland = there's no structure under the selection
			if ( pStructure == NULL)
				gpGraphicMgr->DisplayTerrainSelection( w, l, ++linear );
			else
				gpGraphicMgr->DisplayStructureSelection( pStructure, w, l, ++linear );
			//ATTENTION: "linear++;" already done !
		}
	}

// Restore all the enabled states
	glPopAttrib();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glFlush();

// GL error checking
	if (glGetError() != GL_NO_ERROR) {
		OPENCITY_DEBUG( "GL ERROR" );
	}

// Process the hits
	iHits = glRenderMode( GL_RENDER );
//debug cout << "Number of hits: " << iHits << endl;
	if (iHits > 0) {
	// We only consider the hit which is nearest to the user
		uiDepthMin = 0xFFFFFFFF - 1;
		while (iHits-- > 0) {
			if (uiDepthMin > selectBuffer[ iHits*4 + 1 ]) {
				uiDepthMin = selectBuffer[ iHits*4 + 1 ];
				id = selectBuffer[ iHits*4 + 3 ] - 1;
			}
		}
//debug // cout << "Min depth: " << uiDepthMin << endl;
		ruiL = id / uiCityWidth;
		ruiW = id % uiCityWidth;
		return true;
	}

	return false; // couldn't determine the W & L values
}


   /*=====================================================================*/
void
Renderer::SetWinSize(
	const int & rciWidth,
	const int & rciHeight )
{
	this->iWinWidth = rciWidth;
	this->iWinHeight = rciHeight;

// Set the view port
	glViewport( 0, 0, rciWidth, rciHeight );


// Set the projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	if ( this->ubProjectionType == OC_PERSPECTIVE ) {
		gluPerspective(
			OC_VIEW_ANGLE,
			(GLfloat)this->iWinWidth / (GLfloat)this->iWinHeight,
			OC_Z_NEAR, OC_Z_FAR );
	}
	else {
		glOrtho(
			0.0, (GLdouble)this->iWinWidth,
			0.0, (GLdouble)this->iWinHeight,
			OC_Z_NEAR_ORTHO, OC_Z_FAR);
	}
}


   /*=====================================================================*/
void
Renderer::rendererDisplayTerrain() const
{
	static OC_BYTE tabH [4];
	static GLfloat ax, ay, az;
	static GLfloat bx, by, bz;
	static GLfloat n1x, n1y, n1z;		// normal 1 coordinates
	static GLfloat n2x, n2y, n2z;		// normal 2 coordinates
	static int l, w;					// WARNING: yes, we use INT not UINT


// WARNING: this is used to calculated the final textured fragment.
	glColor4f( .3, .25, .2, 1. );

// Enable terrain texturing
	glPushAttrib( GL_ENABLE_BIT );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, this->uiTerrainTex );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

// BEGIN, draw the terrain as an unique TRIANGLE_STRIP
// which is known as the fastest figure in OpenGL
/* This is the secret formula: c = a^b ;)
		cx = ay * bz - by * az;
		cy = bx * az - ax * bz;
		cz = ax * by - bx * ay;
*/
	glBegin( GL_TRIANGLE_STRIP );

	for (l = 0; l < (int)this->uiCityHeight; l++) {
	// IF we draw the squares from left to right THEN
		if (l % 2 == 0) {
		// Get the 4 heights of the current square
			w = 0;
			gpMapMgr->GetSquareHeight( w, l, tabH );
		// calculate the new normal 1 (the cross product)
			ax = 0.;   ay = (GLfloat)(tabH[1]-tabH[0]); az = 1.;
			bx = 1.;   by = (GLfloat)(tabH[3]-tabH[0]); bz = .0;
			n1x = -by; n1y = 1.;                        n1z = -ay;
		// calculate the new normal 2 (the cross product)
			ax = 1.;   ay = (GLfloat)(tabH[2]-tabH[1]); az = .0;
			bx = .0;   by = (GLfloat)(tabH[3]-tabH[2]); bz = -1.;
			n2x = -ay; n2y = 1.;                        n2z = by;

		// Set the first normal and the first pair of vertices
			glNormal3f( n1x, n1y, n1z );
			glTexCoord2i( w, l );
			glVertex3i( w, tabH[0], l );
			glTexCoord2i( w, l+1 );
			glVertex3i( w, tabH[1], l+1 );

			for (w = 1; w < (int)this->uiCityWidth; w++) {
			// Get the 4 heights of the current square
				gpMapMgr->GetSquareHeight( w, l, tabH );
			// draw the stuff
				glNormal3f( n1x, n1y, n1z );
				glTexCoord2i( w, l );
				glVertex3i( w, tabH[0], l );
				glNormal3f( n2x, n2y, n2z );
				glTexCoord2i( w, l+1 );
				glVertex3i( w, tabH[1], l+1 );

			// calculate the new normal 1 (the cross product)
				ax = 0.;   ay = (GLfloat)(tabH[1]-tabH[0]); az = 1.;
				bx = 1.;   by = (GLfloat)(tabH[3]-tabH[0]); bz = .0;
				n1x = -by; n1y = 1.;                        n1z = -ay;
			// calculate the new normal 2 (the cross product)
				ax = 1.;   ay = (GLfloat)(tabH[2]-tabH[1]); az = .0;
				bx = .0;   by = (GLfloat)(tabH[3]-tabH[2]); bz = -1.;
				n2x = -ay; n2y = 1.;                        n2z = by;
			} // for

		// Draw the last edge
			glNormal3f( n1x, n1y, n1z );
			glTexCoord2i( w, l );
			glVertex3i( w, tabH[3], l );
			glNormal3f( n2x, n2y, n2z );
			glTexCoord2i( w, l+1 );
			glVertex3i( w, tabH[2], l+1 );
		// Then prepare the triangles for the next line
			glVertex3i( w, tabH[2], l+1 );
		}
		else {
		// WARNING: repeated codes as above ================================
		// We draw the square from right to left
		// Get the 4 heights of the current square
			w = this->uiCityWidth-1;
			gpMapMgr->GetSquareHeight( w, l, tabH );

		// calculate the new normal 1 (the cross product)
			ax = -1.;  ay = (GLfloat)(tabH[0]-tabH[3]); az = .0;
			bx =  .0;  by = (GLfloat)(tabH[2]-tabH[3]); bz = 1.;
			n1x = ay;  n1y = 1.;                        n1z = -by;
		// calculate the new normal 2 (the cross product)
			ax = 1.;   ay = (GLfloat)(tabH[2]-tabH[1]); az = .0;
			bx = .0;   by = (GLfloat)(tabH[0]-tabH[1]); bz = -1.;
			n2x = -ay; n2y = 1.;                        n2z = by;

		// Set the first normal and the first pair of vertices
			glNormal3f( n1x, n1y, n1z );
			glTexCoord2i( w+1, l );
			glVertex3i( w+1, tabH[3], l );
			glTexCoord2i( w+1, l+1 );
			glVertex3i( w+1, tabH[2], l+1 );

			for (w = this->uiCityWidth-2; w >= 0; w--) {
		// Get the 4 heights of the current square
				gpMapMgr->GetSquareHeight( w, l, tabH );
		// draw the stuff
				glNormal3f( n1x, n1y, n1z );
				glTexCoord2i( w+1, l );
				glVertex3i( w+1, tabH[3], l );
				glNormal3f( n2x, n2y, n2z );
				glTexCoord2i( w+1, l+1 );
				glVertex3i( w+1, tabH[2], l+1 );

			// calculate the new normal 1 (the cross product)
				ax = -1.;  ay = (GLfloat)(tabH[0]-tabH[3]); az = .0;
				bx =  .0;  by = (GLfloat)(tabH[2]-tabH[3]); bz = 1.;
				n1x = ay;  n1y = 1.;                        n1z = -by;
			// calculate the new normal 2 (the cross product)
				ax = 1.;   ay = (GLfloat)(tabH[2]-tabH[1]); az = .0;
				bx = .0;   by = (GLfloat)(tabH[0]-tabH[1]); bz = -1.;
				n2x = -ay; n2y = 1.;                        n2z = by;
			}

		// Draw the last edge
			glNormal3f( n1x, n1y, n1z );
			glTexCoord2i( w+1, l );
			glVertex3i( w+1, tabH[0], l );
			glNormal3f( n2x, n2y, n2z );
			glTexCoord2i( w+1, l+1 );
			glVertex3i( w+1, tabH[1], l+1 );
		// Then prepare the triangles for the next line
			glVertex3i( w+1, tabH[1], l+1 );
		}
	}
// then restore the default normal
	glNormal3f( 0., 0., 1. );
	glEnd();
// END: Draw the terrain by using GL_TRIANGLE_STRIP

// Restore old attribs
	glPopAttrib();
}


   /*=====================================================================*/
void
Renderer::rendererDisplayMapGrid( const Map* pcmap )
{
	if (boolHeightChange == false)
		goto displaymapgrid_return;

	uint linear;
	uint w, l;

//--- don't use display list when the user doesn't want it
	if (boolUseDisplayList == true)
		glNewList( uiGridList, GL_COMPILE );

// Enable line stipple
	glPushAttrib( GL_ENABLE_BIT );
	glDisable( GL_LIGHTING );
	glDisable( GL_BLEND );
	glLineStipple( 1, 0x3333 );
	glEnable( GL_LINE_STIPPLE );

// Translate it a bit for a better view
	glPushMatrix();
	glTranslatef( 0.0, 0.05, 0.0 );
	glColor3ub( 120, 120, 120 ); // bright silver

//--- horizontal lines ---
	linear = 0;
	for (l = 0; l <= this->uiCityHeight; l++) {
		glBegin( GL_LINE_STRIP );
		for (w = 0; w <= this->uiCityWidth; w++) {
			glVertex3s( w, pcmap->GetLinearHeight( linear++ ), l );
		}
		glEnd();
	}

//--- vertical lines ---
	for (w = 0; w <= this->uiCityWidth; w++) {
		glBegin( GL_LINE_STRIP );
		linear = w;
		for (l = 0; l <= this->uiCityHeight; l++) {
			glVertex3s( w, pcmap->GetLinearHeight( linear ), l );
			linear += this->uiCityWidth + 1;
		}
		glEnd();
	}

// Restore the old matrix and attribs
	glPopMatrix();
	glPopAttrib();

	if (boolUseDisplayList == true)
		glEndList();

// We have memorized the height's changes
	boolHeightChange = false;

displaymapgrid_return:
	if (boolUseDisplayList == true)
		glCallList( uiGridList );
}


   /*=====================================================================*/
void
Renderer::rendererDisplayCompass() const
{
// Display a simple compass
	glMatrixMode( GL_MODELVIEW );
	glTranslatef( iWinWidth-30, 30, 0 );
	glRotated( this->dYRotateAngle, .0, .0, 1. );
	glBegin( GL_LINES );
		glColor4f( .5, .5, .5, 1. );
		glVertex2i( -20, 0 );
		glVertex2i(  20, 0 );
		glColor4f( .8, .8, .8, 1. );
		glVertex2i( 0, -20 );
		glVertex2i( 0,  20 );
	glEnd();
	glBegin( GL_TRIANGLES );
		glVertex2i( -5, 14 );
		glVertex2i(  5, 14 );
		glVertex2i(  0, 20 );
	glEnd();
	glLoadIdentity();
	glMatrixMode( GL_PROJECTION );
}


   /*=====================================================================*/
void
Renderer::rendererDisplayStatusBar() const
{
// We save the modelview matrix
	glPushMatrix();
	glLoadIdentity();

// Save the old projection matrix before processing
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( 0, this->iWinWidth-1, 0, this->iWinHeight-1 );

// Draw the compass
	if (this->bDisplayCompass) {
		this->rendererDisplayCompass();
	}

// Enable alpha blending
	glEnable( GL_BLEND );

// Draw the blended status rectangle
	glColor4f( .3, .6, .3, .3 );
	glBegin( GL_QUADS );
		glVertex2i( 0, iWinHeight );
		glVertex2i( 0, iWinHeight-20 );
		glVertex2i( iWinWidth, iWinHeight-20 );
		glVertex2i( iWinWidth, iWinHeight );
	glEnd();
	glDisable( GL_BLEND );

// Restore the projection matrix
	glPopMatrix();

// Restore the modelview matrix
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
}


   /*=====================================================================*/
void
Renderer::rendererPrepareView() const
{
// Clear the ModelView matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	
// Translate & rotate the map to create an view angle
	if ( ubProjectionType == OC_PERSPECTIVE ) {
		glRotated( 35, 1., .0, .0 );
		glTranslated( .0, -14.0, -20.0 );
	}
	else {
		glRotated( 45, 1., .0, .0 );
	// Center the world
		glTranslatef( iWinWidth / 2, -5., -iWinHeight / 2 );
	// Zoom a bit
		glScalef( 24., 24., 24. );
	}
	glScalef( fScaleRatio, fScaleRatio, fScaleRatio );


/* you can replace the above commands by this // absolete, outdated
	gluLookAt( 0.0, 80.0, 200.0,
		   0.0, 0.0, 0.0,
		   0.0, 1.0, 0.0 );
*/

// Translate all the scence according to dDeltaX & dDeltaZ
// this translation is due to the direction keys
	glTranslated( this->dDeltaX, 0.0, this->dDeltaZ );

// Rotate the scence to the required angle
	glMultMatrixd( this->dmatrixRotate );
}






























