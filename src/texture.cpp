/***************************************************************************
                          texture.cpp    -  description
       $Id$
                             -------------------
    begin                : ven 2 jui 2004
    copyright            : (C) 2004-2005 by Duong-Khang NGUYEN
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

#include "texture.h"

#include "SDL_image.h"

Texture::Texture():
uiWidth( 0 ),
uiHeight( 0 ),
uiTexture( 0 )
{
	OPENCITY_DEBUG("ctor");
}


Texture::Texture( const string & rcFile )
{
	OPENCITY_DEBUG("param ctor");
	this->uiTexture = Texture::Load( rcFile, this->uiWidth, this->uiHeight );
}


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
Texture::Load( const string & rcFile )
{
	uint w, h;
	return Texture::Load( rcFile, w, h );
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

	OPENCITY_DEBUG( rcFile.c_str() );

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
SDL_Surface*
Texture::HorizontalMirror(
	const SDL_Surface* const psurface
)
{
	SDL_Surface* psNew = NULL;
	int h, iBytesPerRow;
	OC_CHAR* pPixelDest;
	OC_CHAR* pPixelSrc;

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
	pPixelDest = (OC_CHAR*)psNew->pixels;
	pPixelSrc = (OC_CHAR*)psurface->pixels + ((psurface->h-1) * iBytesPerRow);
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
	uint glW, glH;
	void* pPixels = NULL;

// delete the existing texture
	if (glIsTexture( ruiTexture ) == GL_TRUE)
		glDeleteTextures( 1, &ruiTexture );

// create the texture to hold the image
	glGenTextures( 1, &ruiTexture );
	glBindTexture( GL_TEXTURE_2D, ruiTexture );
//	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

//debug cout << "Tex W: " << psurface->w << " / H: " << psurface->h << endl;

	SDL_LockSurface( const_cast<SDL_Surface*>(psurface) );

// Fix the image size
	Texture::GetCorrectSize( psurface->w, psurface->h, glW, glH );

// If the image doesn't have the correct size then scale it before converting
	if ((psurface->w != (int)glW) || (psurface->h != (int)glH)) {
		OPENCITY_DEBUG("Texture's size has been fixed");
		pPixels = malloc( glW * glH * psurface->format->BytesPerPixel );

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
Texture::GetCorrectSize(
	const uint w, const uint h,
	uint & rW,    uint & rH )
{
	rW = 64;
	while (rW < w) rW = rW << 1;

	rH = 64;
	while (rH < h) rH = rH << 1;
}





























