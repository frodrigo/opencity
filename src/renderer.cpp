/***************************************************************************
						renderer.cpp  -  description
							-------------------
	begin                : may 29th, 2003
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

//========================================================================
/* General OpenGL development notes. The subroutines must save and restore
that default states when they terminate.

	Here are the default OpenGL states when the renderer is initialized
for the first time:
	- Default matrix mode:
		glMatrixMode( GL_MODELVIEW );

	- Default color mode:
		glEnable( GL_COLOR_MATERIAL );

	- Default blending function:
		glDisable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	- Default shade mode:
		glShadeMode( GL_FLAT );

	- Default structure display translation vector:
		glTranslatef( 0., 0.05, 0. );

	- Default lighting:
		glEnable( GL_LIGHTING );
		glEnable( GL_LIGHT0 );

	- Default culling mode:
		glDisable( GL_CULL_FACE );
		glCullFace( GL_BACK );
		glFrontFace( GL_CCW );
*/
//========================================================================

// Useful enumerations
#include "opencity_direction.h"
#include "opencity_structure_type.h"

// OpenCity headers
#include "renderer.h"
#include "layer.h"
#include "texture.h"					// Terrain texturing
#include "font_8x8.h"					// 8x8 font definition

/* Test, dec 28th, 06
#include "font_mini_4x6.h"
#include "font_7x14.h"
#include "font_acorn_8x8.h"
#include "font_pearl_8x8.h"
*/

// Global settings
#include "globalvar.h"
extern GlobalVar gVars;


// OpenGL viewport default parameters
#define OC_VIEW_ANGLE		50.0
#define OC_Z_NEAR			1.0
#define OC_Z_FAR			400.0
#define OC_Z_NEAR_ORTHO		-10000.0
#define OC_Z_FAR_ORTHO		1000.0

// OpenGL other view parameters
#define OC_INITIAL_SCALE	12.0
#define OC_INITIAL_EYE_X	200.0			// used for gluLookAt();
#define OC_INITIAL_EYE_Y	100.0
#define OC_INITIAL_EYE_Z	300.0

#define OC_INITIAL_DELTA_X	-25.0			// used for the translation
#define OC_INITIAL_DELTA_Z	-30.0
#define OC_DELTA_X_STEP		2.0
#define OC_DELTA_Z_STEP		2.0

#define OC_Y_ROTATE_ANGLE	0.0				// used for the rotation
#define OC_Y_ROTATE_STEP	10.0

// OpenGL projection type paramaters
#define OC_PERSPECTIVE		1
#define OC_ORTHOGONAL		2


   /*=====================================================================*/
Renderer::Renderer
(
	const uint cityW,
	const uint cityL
):
bHeightChange( true ),
bMinimapChange( true ),
_bDisplayTerrain( true ),
_bDisplayWater( true ),
_bDisplayStructure( true ),
_bDisplayGrid( true ),
_bDisplayCompass( true ),
_bWireFrame( false ),
ubProjectionType( OC_PERSPECTIVE ),
_uiMinimapTex( 0 ),
_uiSplashTex( 0 ),
_bCalculateCulling( true ),
_uiCityWidth( cityW ),
_uiCityLength( cityL )
{
	OPENCITY_DEBUG( "Renderer ctor" );

// Load frequently used textures
//	_uiTerrainTex = Texture::Load( ocHomeDirPrefix( "texture/terrain_plane_128.png" ));
//	_uiTerrainTex = Texture::Load3D( ocHomeDirPrefix( "texture/terrain_128x512.png" ));
//	_uiTerrainTex = Texture::Load3D( ocHomeDirPrefix( "texture/terrain_128x512_texture.png" ));
//	_uiTerrainTex = Texture::Load3D( ocHomeDirPrefix( "texture/terrain_128x512_gradient.png" ));
//	_uiTerrainTex = Texture::Load3D( ocHomeDirPrefix( "texture/terrain_64x512_texture.png" ));
	_uiTerrainTex = Texture::Load3D( ocHomeDirPrefix( "texture/terrain_64x4096_texture.png" ));
//	_uiTerrainTex = Texture::Load3D( ocHomeDirPrefix( "texture/terrain_64x8192_texture.png" ));
	_uiWaterTex = Texture::Load( ocHomeDirPrefix( "graphism/water/texture/blue_water_512.png" ));

// Initialize the model culling grid
	uint size = _uiCityWidth * _uiCityLength;
	_baCulledModel = new bool[size];
	for ( uint i = 0; i < size; i++ ) {
		_baCulledModel[i] = true;
	}

// Initialize the window's size, the viewport
// and set the perspective's ratio
	assert( gVars.gpVideoSrf != NULL );
	Renderer::SetWinSize( gVars.gpVideoSrf->w, gVars.gpVideoSrf->h );

// Settle "home" down ;)
	this->Home();

/* not need for the moment
//--- translate all the scence according to dDeltaX & dDeltaZ
	glTranslated( this->dDeltaX, 0.0, this->dDeltaZ );

//--- rotate the scence to the required angle
	glRotated( this->dYRotateAngle, 0.0, 1.0, 0.0 );
*/


// Enable polygon culling. This can effect the texture alpha blending
//	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );


// Select the flat rendering mode, default is GL_SMOOTH
	glShadeModel( GL_FLAT );
//	glShadeModel( GL_SMOOTH );

//debug testing
//the maximal number of lights we can set is: 8
/*	GLint nbLight;
	glGetIntegerv( GL_MAX_LIGHTS, &nbLight );
	cout << "max lights : " << (int) nbLight << endl;
*/
//debug testing end

// Create 8x8 font
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	this->_uiFontBase = glGenLists( 256 );
	for ( uint i = 0; i < 256; i++ ) {
		glNewList( this->_uiFontBase + i, GL_COMPILE );
		glBitmap( 8,  8, .0, .0, 10., .0, fontdata_8x8 + i*8 );
/* Test, dec 28th, 06
//		glBitmap( 8,  8, .0, .0, 10., .0, fontdata_acorn_8x8 + i*8 );
//		glBitmap( 8,  8, .0, .0, 10., .0, fontdata_pearl_8x8 + i*8 );
//		glBitmap( 7, 14, .0, .0,  9., .0, fontdata_7x14 + i*14 );
//		glBitmap( 4,  6, .0, .0,  6., .0, fontdata_mini_4x6 + i*6 );
*/
		glEndList();
	}

// initialize few display lists
	_uiGridList = glGenLists( 1 );
	_uiTerrainList = glGenLists( 1 );
	_uiWaterList = glGenLists( 1 );

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

// Free GL list
	glDeleteLists( this->_uiFontBase, 256 );

	if (glIsList( this->_uiGridList ))
		glDeleteLists( this->_uiGridList, 1 );
	if (glIsList( _uiTerrainList ))
		glDeleteLists( _uiTerrainList, 1 );
	if (glIsList( _uiWaterList ))
		glDeleteLists( _uiWaterList, 1 );

// Free textures
	glDeleteTextures( 1, &_uiSplashTex );
	glDeleteTextures( 1, &_uiWaterTex );
	glDeleteTextures( 1, &_uiTerrainTex );
	glDeleteTextures( 1, &_uiMinimapTex );

// Free the model culling grid
	delete [] _baCulledModel;
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

// The culling grid must be recalculated
	_bCalculateCulling = true;
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

// The culling grid must be recalculated
	_bCalculateCulling = true;
}


   /*=====================================================================*/
void
Renderer::MoveLeft( const uint & factor )
{
	this->dDeltaX -= fXTransDelta*factor;

// The culling grid must be recalculated
	_bCalculateCulling = true;
}


   /*=====================================================================*/
void
Renderer::MoveRight( const uint & factor )
{
	this->dDeltaX += fXTransDelta*factor;

// The culling grid must be recalculated
	_bCalculateCulling = true;
}


   /*=====================================================================*/
void
Renderer::MoveUp( const uint & factor )
{
	this->dDeltaZ -= fZTransDelta*factor;

// The culling grid must be recalculated
	_bCalculateCulling = true;
}


   /*=====================================================================*/
void
Renderer::MoveDown( const uint & factor )
{
	this->dDeltaZ += fZTransDelta*factor;

// The culling grid must be recalculated
	_bCalculateCulling = true;
}


   /*=====================================================================*/
void
Renderer::Home()
{
	this->fScaleRatio = OC_INITIAL_SCALE;
	this->fXTransDelta = OC_DELTA_X_STEP;
	this->fZTransDelta = OC_DELTA_Z_STEP;
	this->dEyeX = OC_INITIAL_EYE_X;
	this->dEyeY = OC_INITIAL_EYE_Y;
	this->dEyeZ = OC_INITIAL_EYE_Z;
	this->dDeltaX = OC_INITIAL_DELTA_X;
	this->dDeltaZ = OC_INITIAL_DELTA_Z;
	this->dYRotateAngle = OC_Y_ROTATE_ANGLE;

// Reinit the rotation matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glGetDoublev( GL_MODELVIEW_MATRIX, dmatrixRotate );

// Reset the LOD
	_SetLOD();

// The culling grid must be recalculated
	_bCalculateCulling = true;
}


   /*=====================================================================*/
void
Renderer::ZoomIn(  )
{
	if (this->fScaleRatio >= 140)
		return;

	this->fScaleRatio += 1;
	if (this->fScaleRatio < 30) {
		this->fXTransDelta -= .09;
		this->fZTransDelta -= .09;
	}
	_SetLOD();

// The culling grid must be recalculated
	_bCalculateCulling = true;
}


   /*=====================================================================*/
void Renderer::ZoomOut(  )
{
	if (this->fScaleRatio <= 2)
		return;

	this->fScaleRatio -= 1;
	if (this->fScaleRatio < 30) {
		this->fXTransDelta += .09;
		this->fZTransDelta += .09;
	}
	_SetLOD();

// The culling grid must be recalculated
	_bCalculateCulling = true;
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
		_PrepareView();

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
		glReadPixels( rcuiMouseX, _iWinHeight-rcuiMouseY, 1, 1,
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
Renderer::ToggleTerrain()
{
	_bDisplayTerrain = !_bDisplayTerrain;
}


   /*=====================================================================*/
void
Renderer::ToggleWater()
{
	_bDisplayWater = !_bDisplayWater;
}


   /*=====================================================================*/
void
Renderer::ToggleStructure()
{
	_bDisplayStructure = !_bDisplayStructure;
}


   /*=====================================================================*/
void
Renderer::ToggleGrid()
{
	_bDisplayGrid = !_bDisplayGrid;

// If we turn the grid display back on, we need to update the display list
	this->bHeightChange = true;
}


   /*=====================================================================*/
void
Renderer::ToggleCompass()
{
	_bDisplayCompass = !_bDisplayCompass;
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

// Reinit the projection matrix;
	SetWinSize( _iWinWidth, _iWinHeight );

// The culling grid must be recalculated
	_bCalculateCulling = true;
}


   /*=====================================================================*/
void
Renderer::ToggleWireFrame()
{
	_bWireFrame = !_bWireFrame;
	if (_bWireFrame) {
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glPolygonMode( GL_FRONT, GL_LINE );
		glPolygonMode( GL_BACK, GL_POINT );
	}
	else {
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
}


   /*=====================================================================*/
void
Renderer::DisplaySplash(
	const uint & rcuiX,
	const uint & rcuiY )
{
	static uint w, h;

	if (!glIsTexture(_uiSplashTex))
		_uiSplashTex = Texture::Load( ocHomeDirPrefix( "graphism/gui/splash.png"), w, h );

// Store and translate the splash to the specified OpenGL coordinates
	glPushAttrib( GL_ENABLE_BIT );
	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );
	glEnable( GL_TEXTURE_2D );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glBindTexture( GL_TEXTURE_2D, _uiSplashTex );

	glPushMatrix();
	glLoadIdentity();
	glTranslatef( rcuiX + (_iWinWidth-w) / 2, rcuiY + (_iWinHeight-h) / 2, 0 );
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( 0, _iWinWidth-1, 0, _iWinHeight-1 );

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
Renderer::Display
(
	const Map* pcMap,
	const Layer* pcLayer
)
{
	static uint linear;
	static int w, l;
	static const Structure* pStructure;

// Clear the color buffer ( the screen ) and the depth buffer
	glClearColor( OC_CLEAR_COLOR );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

// Prepare the world for rendering
	_PrepareView();

// Display all the structures built on the layer
	if (_bDisplayStructure) {
		glPushMatrix();
		glTranslatef( 0., 0.05, 0. );
		linear = 0;
//		uint counter = 0;
	// IF the graphic manager is created THEN draw
		if (gVars.gpGraphicMgr != NULL)
		for (l = 0; l < (int)_uiCityLength; l++) {
			for (w = 0; w < (int)_uiCityWidth; w++, linear++) {
				if (!_baCulledModel[linear]) {
//					counter++;
					continue;
				}
	
				pStructure = pcLayer->GetLinearStructure( linear );
				if (pStructure != NULL)
					gVars.gpGraphicMgr->DisplayStructure( pStructure, w, l );
			}
		}
//		OPENCITY_DEBUG( "There were " << counter << " structures clipped out" );
		glPopMatrix();
	}

// Displays the grids of the map if the user wants it
// it is translated up a little along the Oz axis
	if (_bDisplayGrid) {
		_DisplayMapGrid( pcMap );
	}

// Calculate the minimap
	if (bMinimapChange) {
		Texture::Building2Texture( (BuildingLayer*)pcLayer, _uiMinimapTex );
		bMinimapChange = false;
	}

// Display the status bar
	_DisplayStatusBar();

// Call the private method to display the height map
	if (_bDisplayTerrain) {
		_DisplayTerrain();
	}

// Display the water texture
	if (_bDisplayWater) {
		_DisplayWater();
	}

	glFlush();

// The height map changes have been memorized
// in the grid and the terrain displaylist
	bHeightChange = false;

// GL error checking
	static GLint glerr = glGetError();
	if (glerr != GL_NO_ERROR) {
		OPENCITY_ERROR( "GL error: " << glerr );
	}
}


   /*=====================================================================*/
void
Renderer::DisplayHighlight(
	const Map* pcMap,
	const Layer* pcLayer,
	const OPENCITY_TOOL_CODE & enumTool )
{
	OPENCITY_DEBUG( "Not implemented" );
	assert( 0 );

	DisplayHighlight(
		pcMap, pcLayer,
		0, 0,
		_uiCityWidth-1, _uiCityLength-1,
		enumTool
	);
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
	const Structure* pStructure;

// Doing some swapping
	OPENCITY_SWAP( uiW1, uiW2, uint );
	OPENCITY_SWAP( uiL1, uiL2, uint );

// Display the current layer & map first
	Display( pcMap, pcLayer );

// Prepare the world for rendering
	_PrepareView();

// Now let's display all the structures in selection mode
	glPushAttrib( GL_ENABLE_BIT );	
	glDisable( GL_LIGHTING );
	glPushMatrix();
	glTranslatef( 0., 0.1, 0. );
	for (l = uiL1; l <= uiL2; l++) {
		linear = l*_uiCityWidth + uiW2;
		for (w = uiW1; w <= uiW2; w++) {
			pStructure = pcLayer->GetLinearStructure( linear );
		// Display the correction structure/terrain with "linear" as objectID
			if ( pStructure == NULL)
				gVars.gpGraphicMgr->DisplayTerrainHighlight( w, l, enumTool );
			else
				gVars.gpGraphicMgr->
				DisplayStructureHighlight( pStructure, w, l, enumTool );

			++linear;
		}
	}
	glPopMatrix();
	glPopAttrib();
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
	static int iH;

// Save the current model view matrix
	glPushMatrix();

// Display the graphic code
	glTranslatef( 0., 0.15, 0. );
	gVars.gpGraphicMgr->DisplayGC( gcode, uiW, uiL );

// Get the maximum square height
	iH = gVars.gpMapMgr->GetSquareMaxHeight( uiW, uiL );

// Get the graphic code dimensions in order to draw the bounding rectangle
	gVars.gpPropertyMgr->GetWLH( gcode, sw, 4, sl, 4, sh, 1 );
	glEnable( GL_BLEND );
	glTranslatef( .0f, 0.1f, .0f );
	glColor4ub( rcubR, rcubG, rcubB, 64 );
	glBegin( GL_QUADS );
		glVertex3i( uiW,    iH, uiL   );
		glVertex3i( uiW,    iH, uiL+sl );
		glVertex3i( uiW+sw, iH, uiL+sl );
		glVertex3i( uiW+sw, iH, uiL );
	glEnd();
	glDisable( GL_BLEND );

// Restore the model view matrix
	glPopMatrix();
}


   /*=====================================================================*/
void
Renderer::DisplaySelection(
	const Map* pcMap,
	const Layer* pcLayer )
{
	DisplaySelection(
		pcMap, pcLayer,
		0, 0,
		_uiCityWidth-1, _uiCityLength-1
	);
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
	OPENCITY_DEBUG( "Not implemented" );
	assert( 0 );
}


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
	gluOrtho2D( 0, _iWinWidth-1, 0, _iWinHeight-1 );

// Save the list base
	glTranslatef( .0, .0, .1 );
	glPushAttrib( GL_LIST_BIT | GL_ENABLE_BIT );
	glDisable( GL_LIGHTING );
	glListBase( this->_uiFontBase );

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
Renderer::GetSelectedWHFrom
(
	const uint & rcuiMouseX,
	const uint & rcuiMouseY,
	uint & ruiW,
	uint & ruiL,
	const Map* pcMap,
	const Layer* pcLayer
)
{
// call the right method with appropriate values
	return GetSelectedWHFrom(
		rcuiMouseX,
		rcuiMouseY,
		ruiW,
		ruiL,
		pcMap,
		pcLayer,
		0, 0,
		_uiCityWidth-1, _uiCityLength-1
	);
}


   /*=====================================================================*/
const bool
Renderer::GetSelectedWHFrom
(
	const uint & rcuiMouseX,
	const uint & rcuiMouseY,
	uint & ruiW,
	uint & ruiL,
	const Map* pcMap,
	const Layer* pcLayer,
	const uint & rcuiW1,
	const uint & rcuiL1,
	const uint & rcuiW2,
	const uint & rcuiL2
)
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
	viewport[2] = _iWinWidth;
	viewport[3] = _iWinHeight;
	gluPickMatrix( rcuiMouseX, _iWinHeight-rcuiMouseY, 2, 2, viewport );
	if ( this->ubProjectionType == OC_PERSPECTIVE ) {
		gluPerspective(
			OC_VIEW_ANGLE,
			(GLfloat)_iWinWidth / (GLfloat)_iWinHeight,
			OC_Z_NEAR, OC_Z_FAR );
	}
	else {
		glOrtho(
			0.0, (GLdouble)_iWinWidth,
			0.0, (GLdouble)_iWinHeight,
			OC_Z_NEAR_ORTHO, OC_Z_FAR_ORTHO);
	}

// Save all the enabled states
	glPushAttrib( GL_ENABLE_BIT );
	glDisable( GL_LIGHTING );

// Clear the color buffer ( the screen )
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

// Prepare the world for rendering
	_PrepareView();

// Now let's display all the structures in selection mode
	linear = 0;
	for (l = 0; l < _uiCityLength; l++) {
		for (w = 0; w < _uiCityWidth; w++) {
			pStructure = pcLayer->GetLinearStructure( linear );
		// display the correction structure/terrain
		// with "linear" as objectID
		// note: linear = 0 is not used since it means blank
		// bland = there's no structure under the selection
			if ( pStructure == NULL)
				gVars.gpGraphicMgr->DisplayTerrainSelection( w, l, ++linear );
			else
				gVars.gpGraphicMgr->DisplayStructureSelection( pStructure, w, l, ++linear );
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
		ruiL = id / _uiCityWidth;
		ruiW = id % _uiCityWidth;
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
	_iWinWidth = rciWidth;
	_iWinHeight = rciHeight;

// Set the view port
	glViewport( 0, 0, rciWidth, rciHeight );

// Set the projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	if ( this->ubProjectionType == OC_PERSPECTIVE ) {
		gluPerspective(
			OC_VIEW_ANGLE,
			(GLfloat)_iWinWidth / (GLfloat)_iWinHeight,
			OC_Z_NEAR, OC_Z_FAR );
	}
	else {
		glOrtho(
			0.0, (GLdouble)_iWinWidth,
			0.0, (GLdouble)_iWinHeight,
			OC_Z_NEAR_ORTHO, OC_Z_FAR_ORTHO);
	}
}


   /*=====================================================================*/
/* TOKILL, kept for reference 2D version
void
Renderer::_DisplayTerrain() const
{
	if (bHeightChange == false)
		goto displayterrain_return;

	static OC_BYTE tabH [4];
	static GLfloat ax, ay, az;
	static GLfloat bx, by, bz;
	static GLfloat n1x, n1y, n1z;		// normal 1 coordinates
	static GLfloat n2x, n2y, n2z;		// normal 2 coordinates
	static int l, w;					// WARNING: yes, we use INT not UINT

// Reserve a new display list for the terrain
	glNewList( _uiTerrainList, GL_COMPILE );

// WARNING: this is used to calculated the final textured fragment.
	glColor4f( .3, .25, .2, 1. );

// Enable terrain texturing
	glPushAttrib( GL_ENABLE_BIT );
	glEnable( GL_CULL_FACE );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, _uiTerrainTex );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

// BEGIN, draw the terrain as an unique TRIANGLE_STRIP
// which is known as the fastest figure in OpenGL
// This is the secret formula: c = a^b ;)
//		cx = ay * bz - by * az;
//		cy = bx * az - ax * bz;
//		cz = ax * by - bx * ay;
//
	glBegin( GL_TRIANGLE_STRIP );

	for (l = 0; l < (int)_uiCityLength; l++) {
	// IF we draw the squares from left to right THEN
		if (l % 2 == 0) {
		// Get the 4 heights of the current square
			w = 0;
			gVars.gpMapMgr->GetSquareHeight( w, l, tabH );
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

			for (w = 1; w < (int)_uiCityWidth; w++) {
			// Get the 4 heights of the current square
				gVars.gpMapMgr->GetSquareHeight( w, l, tabH );
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
			w = _uiCityWidth-1;
			gVars.gpMapMgr->GetSquareHeight( w, l, tabH );

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

			for (w = _uiCityWidth-2; w >= 0; w--) {
		// Get the 4 heights of the current square
				gVars.gpMapMgr->GetSquareHeight( w, l, tabH );
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
	glEndList();

displayterrain_return:
	glCallList( _uiTerrainList );
}
*/


   /*=====================================================================*/
void
Renderer::_DisplayTerrain() const
{
#define OC_TERRAIN_TEXTURE_DEPTH	64

//	static const GLfloat envColor[4] = {.5, .5, .5, 1};
//	static const GLfloat rPlane[4] = { 0, 0, .5, -.5 };

	if (bHeightChange == false)
		goto displayterrain_return;

	static OC_BYTE tabH[4];				// Terrain height
	static GLfloat tabR[4];				// Texture R (depth) coordinate
	static GLfloat ax, ay, az;
	static GLfloat bx, by, bz;
	static GLfloat n1x, n1y, n1z;		// normal 1 coordinates
	static GLfloat n2x, n2y, n2z;		// normal 2 coordinates
	static int l, w;					// WARNING: yes, we use INT not UINT

// Reserve a new display list for the terrain
	glNewList( _uiTerrainList, GL_COMPILE );

// WARNING: this is used to calculated the final textured fragment.
//	glColor4f( .3, .25, .2, 1. );
	glColor4f( .8, .7, .6, .8 );

// Enable terrain texturing
	glPushAttrib( GL_ENABLE_BIT );
//	glDisable( GL_LIGHTING );
//	glDisable( GL_LIGHT0 );
//	glEnable( GL_BLEND );
	glEnable( GL_CULL_FACE );
	glEnable( GL_TEXTURE_3D );
//	glEnable( GL_TEXTURE_GEN_S );
//	glEnable( GL_TEXTURE_GEN_T );
//	glEnable( GL_TEXTURE_GEN_R );
	glBindTexture( GL_TEXTURE_3D, _uiTerrainTex );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
//	glTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, envColor );			// Used by TexEnvf - GL_BLEND
//	glTexGeni( GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
//	glTexGenfv( GL_R, GL_EYE_PLANE, rPlane );
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

// BEGIN, draw the terrain as an unique TRIANGLE_STRIP
// which is known as the fastest figure in OpenGL
/* This is the secret formula: c = a^b ;)
		cx = ay * bz - by * az;
		cy = bx * az - ax * bz;
		cz = ax * by - bx * ay;
*/
	glBegin( GL_TRIANGLE_STRIP );

	for (l = 0; l < (int)_uiCityLength; l++) {
	// IF we draw the squares from left to right THEN
		if (l % 2 == 0) {
		// Get the 4 heights of the current square
			w = 0;
			gVars.gpMapMgr->GetSquareHeight( w, l, tabH );
			for (int i = 0; i < 4; i++) {
				if (tabH[i] >= 0)
					tabR[i] = (((float)tabH[i] / OC_MAP_HEIGHT_MAX) * 55 / 64) + 9.0 / 64;
				else
					tabR[i] = ((float)OC_MAP_HEIGHT_MAX+tabH[i]) / OC_MAP_HEIGHT_MAX * 8 / 64;
			}

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
			glTexCoord3f( w, l, tabR[0] );
			glVertex3i( w, tabH[0], l );
			glTexCoord3f( w, l+1, tabR[1] );
			glVertex3i( w, tabH[1], l+1 );

			for (w = 1; w < (int)_uiCityWidth; w++) {
			// Get the 4 heights of the current square
				gVars.gpMapMgr->GetSquareHeight( w, l, tabH );
				for (int i = 0; i < 4; i++) {
					if (tabH[i] >= 0)
						tabR[i] = (((float)tabH[i] / OC_MAP_HEIGHT_MAX) * 55 / 64) + 9.0 / 64;
					else
						tabR[i] = ((float)OC_MAP_HEIGHT_MAX+tabH[i]) / OC_MAP_HEIGHT_MAX * 8 / 64;
				}

			// draw the stuff
				glNormal3f( n1x, n1y, n1z );
				glTexCoord3f( w, l, tabR[0] );
				glVertex3i( w, tabH[0], l );
				glNormal3f( n2x, n2y, n2z );
				glTexCoord3f( w, l+1, tabR[1] );
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
			glTexCoord3f( w, l, tabR[3] );
			glVertex3i( w, tabH[3], l );
			glNormal3f( n2x, n2y, n2z );
			glTexCoord3f( w, l+1, tabR[2] );
			glVertex3i( w, tabH[2], l+1 );
		// Then prepare the triangles for the next line
			glVertex3i( w, tabH[2], l+1 );
		}
		else {
		// WARNING: repeated codes as above ================================
		// We draw the square from right to left
		// Get the 4 heights of the current square
			w = _uiCityWidth-1;
			gVars.gpMapMgr->GetSquareHeight( w, l, tabH );
			for (int i = 0; i < 4; i++) {
				if (tabH[i] >= 0)
					tabR[i] = (((float)tabH[i] / OC_MAP_HEIGHT_MAX) * 55 / 64) + 9.0 / 64;
				else
					tabR[i] = ((float)OC_MAP_HEIGHT_MAX+tabH[i]) / OC_MAP_HEIGHT_MAX * 8 / 64;
			}

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
			glTexCoord3f( w+1, l, tabR[3] );
			glVertex3i( w+1, tabH[3], l );
			glTexCoord3f( w+1, l+1, tabR[2] );
			glVertex3i( w+1, tabH[2], l+1 );

			for (w = _uiCityWidth-2; w >= 0; w--) {
			// Get the 4 heights of the current square
				gVars.gpMapMgr->GetSquareHeight( w, l, tabH );
				for (int i = 0; i < 4; i++) {
					if (tabH[i] >= 0)
						tabR[i] = (((float)tabH[i] / OC_MAP_HEIGHT_MAX) * 55 / 64) + 9.0 / 64;
					else
						tabR[i] = ((float)OC_MAP_HEIGHT_MAX+tabH[i]) / OC_MAP_HEIGHT_MAX * 8 / 64;
				}

			// draw the stuff
				glNormal3f( n1x, n1y, n1z );
				glTexCoord3f( w+1, l, tabR[3] );
				glVertex3i( w+1, tabH[3], l );
				glNormal3f( n2x, n2y, n2z );
				glTexCoord3f( w+1, l+1, tabR[2] );
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
			glTexCoord3f( w+1, l, tabR[0] );
			glVertex3i( w+1, tabH[0], l );
			glNormal3f( n2x, n2y, n2z );
			glTexCoord3f( w+1, l+1, tabR[1] );
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
	glEndList();

displayterrain_return:
	glCallList( _uiTerrainList );
}


   /*=====================================================================*/
void
Renderer::_DisplayWater() const
{
#define WATER_HEIGHT	-.15

	static bool initialized = false;

// IF the display list is already initialized THEN
	if (initialized)
		goto displaywater_return;
	else
		initialized = true;

	static GLfloat ax, ay, az;
	static GLfloat bx, by, bz;
	static GLfloat n1x, n1y, n1z;		// normal 1 coordinates
	static GLfloat n2x, n2y, n2z;		// normal 2 coordinates
	static int l, w;					// WARNING: yes, we use INT not UINT

// Reserve a new display list for the terrain
	glNewList( _uiWaterList, GL_COMPILE );

// WARNING: this is used to calculated the final textured fragment.
	glColor4f( .3, .25, .2, 1. );

// Enable terrain texturing
	glPushAttrib( GL_ENABLE_BIT );
	glEnable( GL_BLEND );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, _uiWaterTex );
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

	for (l = 0; l < (int)_uiCityLength; l++) {
	// IF we draw the squares from left to right THEN
		if (l % 2 == 0) {
		// Get the 4 heights of the current square
			w = 0;
		// calculate the new normal 1 (the cross product)
			ax = 0.;   ay = WATER_HEIGHT; az = 1.;
			bx = 1.;   by = WATER_HEIGHT; bz = .0;
			n1x = -by; n1y = 1.; n1z = -ay;
		// calculate the new normal 2 (the cross product)
			ax = 1.;   ay = WATER_HEIGHT; az = .0;
			bx = .0;   by = WATER_HEIGHT; bz = -1.;
			n2x = -ay; n2y = 1.; n2z = by;

		// Set the first normal and the first pair of vertices
			glNormal3f( n1x, n1y, n1z );
			glTexCoord2i( w, l );
			glVertex3f( w, WATER_HEIGHT, l );
			glTexCoord2i( w, l+1 );
			glVertex3f( w, WATER_HEIGHT, l+1 );

			for (w = 1; w < (int)_uiCityWidth; w++) {
			// draw the stuff
				glNormal3f( n1x, n1y, n1z );
				glTexCoord2i( w, l );
				glVertex3f( w, WATER_HEIGHT, l );
				glNormal3f( n2x, n2y, n2z );
				glTexCoord2i( w, l+1 );
				glVertex3f( w, WATER_HEIGHT, l+1 );

			// calculate the new normal 1 (the cross product)
				ax = 0.;   ay = WATER_HEIGHT; az = 1.;
				bx = 1.;   by = WATER_HEIGHT; bz = .0;
				n1x = -by; n1y = 1.; n1z = -ay;
			// calculate the new normal 2 (the cross product)
				ax = 1.;   ay = WATER_HEIGHT; az = .0;
				bx = .0;   by = WATER_HEIGHT; bz = -1.;
				n2x = -ay; n2y = 1.; n2z = by;
			} // for

		// Draw the last edge
			glNormal3f( n1x, n1y, n1z );
			glTexCoord2i( w, l );
			glVertex3f( w, WATER_HEIGHT, l );
			glNormal3f( n2x, n2y, n2z );
			glTexCoord2i( w, l+1 );
			glVertex3f( w, WATER_HEIGHT, l+1 );
		// Then prepare the triangles for the next line
			glVertex3f( w, WATER_HEIGHT, l+1 );
		}
		else {
		// WARNING: repeated codes as above ================================
		// We draw the square from right to left
		// Get the 4 heights of the current square
			w = _uiCityWidth-1;

		// calculate the new normal 1 (the cross product)
			ax = -1.;  ay = WATER_HEIGHT; az = .0;
			bx =  .0;  by = WATER_HEIGHT; bz = 1.;
			n1x = ay;  n1y = 1.; n1z = -by;
		// calculate the new normal 2 (the cross product)
			ax = 1.;   ay = WATER_HEIGHT; az = .0;
			bx = .0;   by = WATER_HEIGHT; bz = -1.;
			n2x = -ay; n2y = 1.; n2z = by;

		// Set the first normal and the first pair of vertices
			glNormal3f( n1x, n1y, n1z );
			glTexCoord2i( w+1, l );
			glVertex3f( w+1, WATER_HEIGHT, l );
			glTexCoord2i( w+1, l+1 );
			glVertex3f( w+1, WATER_HEIGHT, l+1 );

			for (w = _uiCityWidth-2; w >= 0; w--) {
		// draw the stuff
				glNormal3f( n1x, n1y, n1z );
				glTexCoord2i( w+1, l );
				glVertex3f( w+1, WATER_HEIGHT, l );
				glNormal3f( n2x, n2y, n2z );
				glTexCoord2i( w+1, l+1 );
				glVertex3f( w+1, WATER_HEIGHT, l+1 );

			// calculate the new normal 1 (the cross product)
				ax = -1.;  ay = WATER_HEIGHT; az = .0;
				bx =  .0;  by = WATER_HEIGHT; bz = 1.;
				n1x = ay;  n1y = 1.; n1z = -by;
			// calculate the new normal 2 (the cross product)
				ax = 1.;   ay = WATER_HEIGHT; az = .0;
				bx = .0;   by = WATER_HEIGHT; bz = -1.;
				n2x = -ay; n2y = 1.; n2z = by;
			}

		// Draw the last edge
			glNormal3f( n1x, n1y, n1z );
			glTexCoord2i( w+1, l );
			glVertex3f( w+1, WATER_HEIGHT, l );
			glNormal3f( n2x, n2y, n2z );
			glTexCoord2i( w+1, l+1 );
			glVertex3f( w+1, WATER_HEIGHT, l+1 );
		// Then prepare the triangles for the next line
			glVertex3f( w+1, WATER_HEIGHT, l+1 );
		}
	}
// then restore the default normal
	glNormal3f( 0., 0., 1. );
	glEnd();
// END: Draw the terrain by using GL_TRIANGLE_STRIP

// Restore old attribs
	glPopAttrib();
	glEndList();

displaywater_return:
	glCallList( _uiWaterList );
}


   /*=====================================================================*/
void
Renderer::_DisplayMapGrid( const Map* pcmap )
{
	if (bHeightChange == false)
		goto displaymapgrid_return;

	uint linear;
	uint w, l;

// Create a new display list for the grid
	glNewList( _uiGridList, GL_COMPILE );

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
	for (l = 0; l <= _uiCityLength; l++) {
		glBegin( GL_LINE_STRIP );
		for (w = 0; w <= _uiCityWidth; w++) {
			glVertex3s( w, pcmap->GetLinearHeight( linear++ ), l );
		}
		glEnd();
	}

//--- vertical lines ---
	for (w = 0; w <= _uiCityWidth; w++) {
		glBegin( GL_LINE_STRIP );
		linear = w;
		for (l = 0; l <= _uiCityLength; l++) {
			glVertex3s( w, pcmap->GetLinearHeight( linear ), l );
			linear += _uiCityWidth + 1;
		}
		glEnd();
	}

// Restore the old matrix and attribs
	glPopMatrix();
	glPopAttrib();
	glEndList();

displaymapgrid_return:
	glCallList( _uiGridList );
}


   /*=====================================================================*/
void
Renderer::_DisplayCompass() const
{
	glPushAttrib( GL_ENABLE_BIT );
	glDisable( GL_LIGHTING );

// Display the compass
	glMatrixMode( GL_MODELVIEW );
	glTranslatef( _iWinWidth/2 + 223, 32, 0 );
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

// Display the minimap
	glEnable( GL_BLEND );
	glEnable( GL_TEXTURE_2D );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glBindTexture( GL_TEXTURE_2D, _uiMinimapTex );
	glLoadIdentity();
	glTranslatef( _iWinWidth/2 + 195, 3, 0 );

// Display the textured quad
	glBegin( GL_QUADS );
	glTexCoord2i( 0, 0 ); glVertex2i( 0,   0 );
	glTexCoord2i( 0, 1 ); glVertex2i( 0,  56 );
	glTexCoord2i( 1, 1 ); glVertex2i( 56, 56 );
	glTexCoord2i( 1, 0 ); glVertex2i( 56,  0 );
	glEnd();

// Restore the old values
	glLoadIdentity();
	glMatrixMode( GL_PROJECTION );
	glPopAttrib();
}


   /*=====================================================================*/
void
Renderer::_DisplayStatusBar() const
{
// We save the modelview matrix
	glPushMatrix();
	glLoadIdentity();

// Save the old projection matrix before processing
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( 0, _iWinWidth-1, 0, _iWinHeight-1 );

// Draw the compass
	if (_bDisplayCompass) {
		_DisplayCompass();
	}

// Enable alpha blending
	glPushAttrib( GL_ENABLE_BIT );
	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );

// Draw the blended status rectangle
	glColor4f( .1, .1, .1, .8 );
	glBegin( GL_QUADS );
		glVertex2i( 0, _iWinHeight );
		glVertex2i( 0, _iWinHeight-20 );
		glVertex2i( _iWinWidth, _iWinHeight-20 );
		glVertex2i( _iWinWidth, _iWinHeight );
	glEnd();

// Restore the attributes
	glPopAttrib();

// Restore the projection matrix
	glPopMatrix();

// Restore the modelview matrix
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
}


   /*=====================================================================*/
void
Renderer::_PrepareView()
{
// Clear the ModelView matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

// Translate & rotate the map to create an view angle
	if ( ubProjectionType == OC_PERSPECTIVE ) {
		glRotated( 35, 1., .0, .0 );
		glTranslated( .0, -18.0, -24.0 );
	}
	else {
		glRotated( 45, 1., .0, .0 );
		glTranslated( _iWinWidth / 2, -5.0, -_iWinHeight / 2 );
		glScalef( 24., 24., 24. );
	}
	float ratio = fScaleRatio / 10;
	glScalef( ratio, ratio, ratio);


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

// Calculate the culling grid if it's requested
	if (_bCalculateCulling) {
		OPENCITY_DEBUG( "Calculate culling" );
		_CalculateCulledGrid( 0, 0, _uiCityWidth, _uiCityLength, true);
	 	_bCalculateCulling = false;
	}
}


   /*=====================================================================*/
void
Renderer::_CalculateCulledGrid
(
	uint w1, uint l1,
	uint w2, uint l2,
	bool init
)
{
#define MIN_SUBDIVISION_SIZE		4
#define WINDOW_DELTA				0
#define IS_CULLED( x, y )			(x > x1) and (x < x2) and (y > y1) and (y < y2)

	static GLdouble daModelViewMatrix[16], daProjectionMatrix[16];
	static GLint iaViewport[4];
	static int x1, y1, x2, y2;

// Get the OpenGL matrices if it's requested
	if (init) {
		glGetDoublev( GL_MODELVIEW_MATRIX, daModelViewMatrix );
		glGetDoublev( GL_PROJECTION_MATRIX, daProjectionMatrix );
		glGetIntegerv( GL_VIEWPORT, iaViewport);
		x1 = -WINDOW_DELTA;
		y1 = -WINDOW_DELTA;
		x2 = _iWinWidth + WINDOW_DELTA;
		y2 = _iWinHeight + WINDOW_DELTA;
	}

	double x, y, z;				// Window coordinates
	bool a, b, c, d;			// True if the corresponding part is culled (selected)
	bool culled;

// First part A
	gluProject( w1, 0, l1, daModelViewMatrix, daProjectionMatrix, iaViewport, &x, &y, &z );
	a = IS_CULLED( x, y );
//	OPENCITY_DEBUG( "a/x/y/z " << a << " / " << x << "/" << y << "/" << z );
// Second part B
	gluProject( w1, 0, l2, daModelViewMatrix, daProjectionMatrix, iaViewport, &x, &y, &z );
	b = IS_CULLED( x, y );
//	OPENCITY_DEBUG( "b/x/y/z " << b << " / " << x << "/" << y << "/" << z );
// Third part C
	gluProject( w2, 0, l2, daModelViewMatrix, daProjectionMatrix, iaViewport, &x, &y, &z );
	c = IS_CULLED( x, y );
//	OPENCITY_DEBUG( "c/x/y/z " << c << " / " << x << "/" << y << "/" << z );
// Fourth part D
	gluProject( w2, 0, l1, daModelViewMatrix, daProjectionMatrix, iaViewport, &x, &y, &z );
	d = IS_CULLED( x, y );
//	OPENCITY_DEBUG( "d/x/y/z " << d << " / " << x << "/" << y << "/" << z );

// IF the working surface is small enough THEN
	uint linear;				// Array linear index
	if ((w2-w1 <= MIN_SUBDIVISION_SIZE) || (l2-l1 <= MIN_SUBDIVISION_SIZE)) {
		culled = a or b or c or d;
//		OPENCITY_DEBUG( "culled? w1/l1_w2/l2: " << culled << " " << w1 << "/" << l1 << " _ " << w2 << "/" << l2 );
		for (uint l = l1; l < l2; l++) {
			linear = l*_uiCityWidth + w1;
			for (uint w = w1; w < w2; w++, linear++)
				_baCulledModel[linear] = culled;
		}
	}
	else {
	// IF all culled (selected) THEN
		culled = a and b and c and d;
		if (culled) {
//			OPENCITY_DEBUG( "culled: w1/l1_w2/l2: " << w1 << "/" << l1 << " _ " << w2 << "/" << l2 );
			for (uint l = l1; l < l2; l++) {
				linear = l*_uiCityWidth + w1;
				for (uint w = w1; w < w2; w++, linear++)
					_baCulledModel[linear] = true;
			}
		} else
	// IF all not culled (not selected) THEN divide the grid into four parts
		{
//			OPENCITY_DEBUG( "unknown: w1/l1_w2/l2: " << w1 << "/" << l1 << " _ " << w2 << "/" << l2 );
			uint mw = (w2-w1)/2;
			uint ml = (l2-l1)/2;
			_CalculateCulledGrid(w1,    l1,    w1+mw, l1+ml);
			_CalculateCulledGrid(w1,    l1+ml, w1+mw, l2);
			_CalculateCulledGrid(w1+mw, l1+ml, w2,    l2);
			_CalculateCulledGrid(w1+mw, l1,    w2,    l1+ml);
		}
	}
}


   /*=====================================================================*/
void
Renderer::_SetLOD() const
{
	if (gVars.gpGraphicMgr == NULL)
		return;

	if (this->fScaleRatio >= OC_INITIAL_SCALE)
		gVars.gpGraphicMgr->SetLOD(OC_LOD_HIGH);
	else if (this->fScaleRatio >= OC_INITIAL_SCALE - 8)
		gVars.gpGraphicMgr->SetLOD(OC_LOD_MEDIUM);
	else if (this->fScaleRatio >= OC_INITIAL_SCALE - 10)
		gVars.gpGraphicMgr->SetLOD(OC_LOD_LOW);
}























