/***************************************************************************
						texture.cpp  -  description
							-------------------
	begin                : july 2nd, 2004
	copyright            : (C) 2004-2008 by Duong-Khang NGUYEN
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
#include "texture.h"
#include "buildinglayer.h"
#include "structure.h"

// Global settings
#include "globalvar.h"
extern GlobalVar gVars;

// Libraries headers
#include "SDL_image.h"


map<string, GLuint> Texture::mapTexture;		// Automanaged texture cache


   /*=====================================================================*/
Texture::Texture():
uiWidth( 0 ),
uiHeight( 0 ),
uiTexture( 0 )
{
	OPENCITY_DEBUG("ctor");
}


   /*=====================================================================*/
Texture::Texture( const string & rcFile )
{
	OPENCITY_DEBUG("param ctor");
	this->uiTexture = Texture::Load( rcFile, this->uiWidth, this->uiHeight );
}


   /*=====================================================================*/
Texture::~Texture()
{
	OPENCITY_DEBUG("dtor");

// Silently ignore invalid texture
	glDeleteTextures( 1, &this->uiTexture );
}


   //========================================================================
   //                       PRIVATE STATIC METHODS
   //========================================================================
const GLuint
Texture::Load( const string& rcFile )
{
	uint w, h;
	GLuint tex = 0;
	string filename = Texture::_ResolveRelativePath( rcFile );

// IF the requested texture is not already in the cache
// OR it's no more a valid texture THEN
	if ((Texture::mapTexture.find( filename ) == Texture::mapTexture.end())
		or (glIsTexture( Texture::mapTexture[ filename ] ) == GL_FALSE) )
	{
		tex = Texture::Load( filename, w, h );
		Texture::mapTexture[ filename ] = tex;
	}
	else {
//		OPENCITY_DEBUG( "Texture cache hit for: " << filename );
		tex = Texture::mapTexture[ filename ];
	}

	return tex;
}


   /*=====================================================================*/
const GLuint
Texture::Load3D( const string& rcFile )
{
	uint w, h;
	GLuint tex = 0;
	string filename = Texture::_ResolveRelativePath( rcFile );

// IF the requested texture is not already in the cache
// OR it's no more a valid texture THEN
	if ((Texture::mapTexture.find( filename ) == Texture::mapTexture.end())
		or (glIsTexture( Texture::mapTexture[ filename ] ) == GL_FALSE) )
	{
		tex = Texture::Load3D( filename, w, h );
		Texture::mapTexture[ filename ] = tex;
	}
	else {
//		OPENCITY_DEBUG( "3D texture cache hit for: " << filename );
		tex = Texture::mapTexture[ filename ];
	}

	return tex;
}


   /*=====================================================================*/
const GLuint
Texture::Load
(
	const string & rcFile,
	uint & ruiW,
	uint & ruiH
)
{
	SDL_Surface* pImage = NULL;
	SDL_Surface* pMirror = NULL;
	GLuint uiTexture = 0;	// tell glIsTexture that this is not a texture name

	OPENCITY_DEBUG( "Loading texture file: " << rcFile.c_str() );

// TODO: error checking
// load the image to the SDL surface
	pImage = IMG_Load( rcFile.c_str() );
	assert( pImage != NULL );

// Return the width and height
	ruiW = pImage->w;
	ruiH = pImage->h;

// Check the bytes per pixel
//	cout << "Bytes per pixel: " << (int)pImage->format->BytesPerPixel << endl;
	assert( pImage->format->BytesPerPixel == 4 );

// note: the texture coordinates are flipped in the vertical direction
//       because the SDL_image library reads the image's pixels
//       from the upper left corner, left to right, to the bottom right
//       corner whereas the glTexImage2D builds the texture from
//       the bottom left corner, left to right, to the upper right corner
// Take the horizontal mirror of current image
//	pMirror = Texture::HorizontalMirror( pConverted );
	pMirror = Texture::HorizontalMirror( pImage );

// convert the surface to texture
// create the new texture if needed
	Texture::Surface2Texture( pMirror, uiTexture );
	SDL_FreeSurface( pImage );
	SDL_FreeSurface( pMirror );

	return uiTexture;
}


   /*=====================================================================*/
const GLuint
Texture::Load3D
(
	const string & rcFile,
	uint & ruiW,
	uint & ruiH
)
{
	OPENCITY_DEBUG( rcFile.c_str() );

// Load the image to the SDL surface
	SDL_Surface* pImage = IMG_Load( rcFile.c_str() );
	assert( pImage != NULL );

// Return the width and height
	ruiW = pImage->w;
	ruiH = pImage->h;

// Load the texture
	GLuint uiTexture = 0;
	uint bpp = pImage->format->BytesPerPixel;
	if (bpp == 3) {
		OPENCITY_DEBUG( "3D texture format: GL_RGB" );
		Texture::Surface2Texture3D( pImage, uiTexture, GL_RGB );
	}
	else if (bpp == 4) {
		OPENCITY_DEBUG( "3D texture format: GL_RGBA" );
		Texture::Surface2Texture3D( pImage, uiTexture, GL_RGBA );
	}

	SDL_FreeSurface( pImage );

	assert( uiTexture != 0 );
	return uiTexture;
}


   /*=====================================================================*/
SDL_Surface*
Texture::HorizontalMirror(
	const SDL_Surface* const psurface
)
{
	SDL_Surface* psNew = NULL;
	int h, iBytesPerRow;
	char* pPixelDest;
	char* pPixelSrc;

// IF the surface needs locking THEN
	if (SDL_MUSTLOCK( psurface )) {
		SDL_LockSurface( const_cast<SDL_Surface*>(psurface) );
	}

// Create the new surface, then lock it
	psNew = SDL_CreateRGBSurface(
		SDL_SWSURFACE,
		psurface->w,
		psurface->h,
		psurface->format->BitsPerPixel,
		psurface->format->Rmask,
		psurface->format->Gmask,
		psurface->format->Bmask,
		psurface->format->Amask );
	assert( psNew != NULL );
	SDL_LockSurface( psNew );

// Variables initialization
	iBytesPerRow = psurface->w * psurface->format->BytesPerPixel;
	pPixelDest = (char*)psNew->pixels;
	pPixelSrc = (char*)psurface->pixels + ((psurface->h-1) * iBytesPerRow);
//debug cout << "BPR: " << iBytesPerRow << endl;

// Copy the pixels
	for ( h = psurface->h-1; h >= 0; h-- ) {
		memcpy( pPixelDest, pPixelSrc, iBytesPerRow );
		pPixelDest += iBytesPerRow;
		pPixelSrc -= iBytesPerRow;
//debug cout << "bla: " << h << endl;
	}

	SDL_UnlockSurface( psNew );

// IF the surface needs locking THEN
	if (SDL_MUSTLOCK( psurface )) {
		SDL_UnlockSurface( const_cast<SDL_Surface*>(psurface) );
	}

	return psNew;
}


   /*=====================================================================*/
void
Texture::Surface2Texture(
	const SDL_Surface* const psurface,
	GLuint & ruiTexture )
{
// Delete the existing texture
	if (glIsTexture( ruiTexture ) == GL_TRUE)
		glDeleteTextures( 1, &ruiTexture );

// Create the texture to hold the image
	glGenTextures( 1, &ruiTexture );
	glBindTexture( GL_TEXTURE_2D, ruiTexture );
//	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	SDL_LockSurface( const_cast<SDL_Surface*>(psurface) );

// If the image doesn't have the correct size then scale it before converting
	uint glW, glH;
	if (Texture::_GetCorrectSize( psurface->w, psurface->h, glW, glH )) {
		OPENCITY_DEBUG("Texture's size has been fixed");
		void* pPixels = malloc( glW * glH * psurface->format->BytesPerPixel );
		assert( pPixels != NULL );

	// Scale the input image
		(void)gluScaleImage(
			GL_RGBA,			// Image format
			psurface->w,		// The image's size
			psurface->h,
			GL_UNSIGNED_BYTE,	// The data type of the input pixel
			psurface->pixels,	// The pixels' data
			glW,				// The wanted scaled image's size
			glH,
			GL_UNSIGNED_BYTE,	// The data type of the output pixel
			pPixels				// The pointer which will holds the output data
		);

	// Convert the scaled image to texture
		glTexImage2D(
			GL_TEXTURE_2D,		// texture 2D
			0,					// base image
			GL_RGBA,			// internal format
			glW,				// texture width, must be 2n >= 64
			glH,				// texture height, must be 2n >= 64
			0,					// no border
			GL_RGBA,			// pixel format
			GL_UNSIGNED_BYTE,	// pixel data format
			pPixels				// point to the pixels' data
			);

		free( pPixels );
		pPixels = NULL;			// Safe
	}
	else {
	// Convert the surface to the OpenGL texture
		glTexImage2D(
			GL_TEXTURE_2D,		// texture 2D
			0,					// base image
			GL_RGBA,			// internal format
			psurface->w,		// texture width, must be 2n >= 64
			psurface->h,		// texture height, must be 2n >= 64
			0,					// no border
			GL_RGBA,			// pixel format
			GL_UNSIGNED_BYTE,	// pixel data format
			psurface->pixels	// point to the pixels' data
			);
	}

	SDL_UnlockSurface( const_cast<SDL_Surface*>(psurface) );

// GL error checking
	GLenum glError = glGetError();
	if (glError != GL_NO_ERROR) {
		GLint maxTexSize;
		glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxTexSize );

		OPENCITY_DEBUG( "CRITIC GL ERROR while loading texture" );
		cout << "Max texture size is: " << maxTexSize << endl;
		cout << "GL error code: 0x";
		cout.flags( cout.hex );
		cout << glError << endl;
		cout.flags( cout.dec );
	}
}


   /*=====================================================================*/
void
Texture::Surface2Texture3D
(
	const SDL_Surface* const psurface,
	GLuint & ruiTexture,
	GLint format
)
{
/*
// Dynamically look up the glTexImage3D extension
	PFNGLTEXIMAGE3DEXTPROC glTexImage3D = (PFNGLTEXIMAGE3DEXTPROC) SDL_GL_GetProcAddress("glTexImage3DEXT");
	if (glTexImage3D == NULL) {
		OPENCITY_FATAL( "glTexImage3D function not found" );
		abort();
	}
*/

// Delete the existing texture
	if (glIsTexture( ruiTexture ) == GL_TRUE)
		glDeleteTextures( 1, &ruiTexture );

// Create the texture to hold the image
	glGenTextures( 1, &ruiTexture );
	glBindTexture( GL_TEXTURE_3D, ruiTexture );
	SDL_LockSurface( const_cast<SDL_Surface*>(psurface) );

// If the image doesn't have the correct size then scale it before converting
// Convert the surface to the OpenGL texture
// NOTE: the texture internal format is the same as the pixels' one
	gVars.gpExtensionMgr->glTexImage3D(
		GL_TEXTURE_3D,		// texture 3D
		0,					// base image
		format,				// internal format
		psurface->w,		// texture width, must be 2n >= 64
		psurface->w,		// texture width, must be 2n >= 64		// The 3D texture is 64x64x64 RGBA
		psurface->h / psurface->w,				// Depth
		0,					// no border
		format,				// pixel format
		GL_UNSIGNED_BYTE,	// pixel data format
		psurface->pixels	// point to the pixels' data
		);

	SDL_UnlockSurface( const_cast<SDL_Surface*>(psurface) );

// GL error checking
	GLenum glError = glGetError();
	if (glError != GL_NO_ERROR) {
		GLint maxTexSize;
		glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxTexSize );

		OPENCITY_DEBUG( "CRITIC GL ERROR while loading texture" );
		cout << "Max texture size is: " << maxTexSize << endl;
		cout << "GL error code: 0x";
		cout.flags( cout.hex );
		cout << glError << endl;
		cout.flags( cout.dec );
	}
}


   /*=====================================================================*/
void
Texture::Building2Texture
(
	const BuildingLayer* const pLayer,
	GLuint& ruiTexture
)
{
#define BYTE_PER_PIXEL	4

	assert( pLayer != NULL );

// Delete the existing texture
	if (glIsTexture( ruiTexture ) == GL_TRUE)
		glDeleteTextures( 1, &ruiTexture );

// Create the texture to hold the image
	glGenTextures( 1, &ruiTexture );
	glBindTexture( GL_TEXTURE_2D, ruiTexture );
//	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

// Create the minimap data
	uint i = 0;
	Structure* pStruct = NULL;
	OC_UBYTE* pData = (OC_UBYTE*)malloc( gVars.guiCityWidth * gVars.guiCityLength * BYTE_PER_PIXEL );
	for (int l = gVars.guiCityLength-1; l >= 0; l--) {
		for (uint w = 0; w < gVars.guiCityWidth; w++, i+=BYTE_PER_PIXEL) {
			pStruct = pLayer->GetStructure( w, l );
			if (pStruct == NULL) {
				pData[i] = 0x50; pData[i+1] = 0xA0; pData[i+2] = 0x50; pData[i+3] = 0xE5;
				continue;
			}

			switch (pStruct->GetCode()) {
				case OC_STRUCTURE_RES:
					pData[i] = 0x50; pData[i+1] = 0xFF; pData[i+2] = 0x50; pData[i+3] = 0xE5;
					break;
				case OC_STRUCTURE_COM:
					pData[i] = 0x50; pData[i+1] = 0x50; pData[i+2] = 0xFF; pData[i+3] = 0xE5;
					break;
				case OC_STRUCTURE_IND:
					pData[i] = 0xFF; pData[i+1] = 0xFF; pData[i+2] = 0x50; pData[i+3] = 0xE5;
					break;
				default:
					pData[i] = 0x50; pData[i+1] = 0x50; pData[i+2] = 0x50; pData[i+3] = 0xE5;
					break;
			}
		}
	}

// If the image doesn't have the correct size then scale it before converting
	uint glW, glH;
	if (Texture::_GetCorrectSize( gVars.guiCityWidth, gVars.guiCityLength, glW, glH )) {
		OPENCITY_DEBUG("Texture's size has been fixed");
		void* pPixels = malloc( glW * glH * BYTE_PER_PIXEL );
		assert( pPixels != NULL );

	// Scale the input image
		(void)gluScaleImage(
			GL_RGBA,			// Image format
			gVars.guiCityWidth,	// The minimap size
			gVars.guiCityLength,
			GL_UNSIGNED_BYTE,	// The data type of the input pixel
			pData,				// The minimap data
			glW,				// The expected scaled image's size
			glH,
			GL_UNSIGNED_BYTE,	// The data type of the output pixel
			pPixels				// The pointer which will holds the output data
		);

	// Convert the scaled image to texture
		glTexImage2D(
			GL_TEXTURE_2D,		// Texture 2D
			0,					// Base image
			GL_RGBA,			// Internal format
			glW,				// Texture width, must be 2n >= 64
			glH,				// Texture height, must be 2n >= 64
			0,					// No border
			GL_RGBA,			// Pixel format
			GL_UNSIGNED_BYTE,	// Pixel data format
			pPixels				// point to the pixels' data
			);

		free( pPixels );
		pPixels = NULL;			// Safe
	}
	else {
	// Convert directly the minimap data to the OpenGL texture
		glTexImage2D(
			GL_TEXTURE_2D,		// Texture 2D
			0,					// Base image
			GL_RGBA,			// Internal format
			gVars.guiCityWidth,	// Texture width, must be 2n >= 64
			gVars.guiCityLength,// Texture height, must be 2n >= 64
			0,					// No border
			GL_RGBA,			// Pixel format
			GL_UNSIGNED_BYTE,	// Pixel data format
			pData				// point to the pixels' data
			);
	}

	free( pData );
	pData = NULL;
}


   /*=====================================================================*/
bool
Texture::_GetCorrectSize(
	const uint w, const uint h,
	uint & rW,    uint & rH )
{
	rW = 64;
	while (rW < w) rW = rW << 1;

	rH = 64;
	while (rH < h) rH = rH << 1;

	return (w != rW || h != rH);
}


   /*=====================================================================*/
string
Texture::_ResolveRelativePath(const string& rcsPath)
{
	string output = rcsPath;

// Search for "/../"
	string::size_type start, end = 0;
	while ((end = output.find("/../")) != output.npos) {
		if (end <= 0)
			break;

	// Search for the first "/" before "/../"
		start = output.rfind("/", end-1);
		if (start == output.npos)
			break;

	// Resolve the relative path
		output.erase(start, end+3-start);
	}

	return output;
}



























