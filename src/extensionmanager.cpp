/***************************************************************************
					extensionmanager.cpp  -  description
							-------------------
	begin                : august 15th, 2008
	copyright            : (C) 2008 by Duong Khang NGUYEN
	email                : neoneurone @ gmail com

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

#include "extensionmanager.h"

   /*====================================================================*/
ExtensionManager::ExtensionManager():
_bInitialized(false)
{
	OPENCITY_DEBUG("ctor");
}

   /*====================================================================*/
ExtensionManager::~ExtensionManager()
{
	OPENCITY_DEBUG("dtor");
}

   /*====================================================================*/
bool
ExtensionManager::Load()
{
	bool ok = true;

	// Warning: ISO C++ forbids casting between pointer-to-function and pointer-to-object
	// There's no other solution at the time of writing.

	// Look for the glTexImage3DEXT function.
	this->glTexImage3D = (PFNGLTEXIMAGE3DEXTPROC)SDL_GL_GetProcAddress("glTexImage3DEXT");

	// Look for the glTexImage3D function instead.
	if (this->glTexImage3D == NULL) {
		this->glTexImage3D = (PFNGLTEXIMAGE3DEXTPROC)SDL_GL_GetProcAddress("glTexImage3D");
	}

	if (this->glTexImage3D == NULL) {
		OPENCITY_ERROR( "GL_EXT_texture3D extension unsupported by your video driver" );
		ok = false;
	}
	else {
		OPENCITY_INFO( "GL_EXT_texture3D supported" );
	}

	_bInitialized = ok;
	return ok;
}
