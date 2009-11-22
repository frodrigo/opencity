/***************************************************************************
                        CSdl.cpp  -  description
							-------------------
	begin                : November 1st, 2009
	copyright            : (C) 2009 by Duong Khang NGUYEN
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

// Framework headers
#include "CSdl.h"				// System::Sdl class
#include "System/CString.h"		// System::String class

// libsdl headers
#include "SDL.h"				// SDL_Version, SDL_Linked_Version, SDL_VERSION macro

SPF_NAMESPACE_BEGIN(System)
SPF_NAMESPACE_NESTED_BEGIN(Information, Software)


   /*=====================================================================*/
Sdl::Sdl() {}


Sdl::~Sdl() {}


String Sdl::ToString() const
{
	return String("System::Information::Software::Sdl");
}


   /*=====================================================================*/
const String Sdl::GetCompiletimeVersion()
{
	SDL_version lsVersion;
	SDL_VERSION(&lsVersion);

	String lsStringVersion;
	lsStringVersion << (short)lsVersion.major << "." << (short)lsVersion.minor << "." << (short)lsVersion.patch;

	return lsStringVersion;
}


const String Sdl::GetRuntimeVersion()
{
	const SDL_version* lpVersion = SDL_Linked_Version();
	//assert(lpVersion != NULL);

	String lsStringVersion;
	lsStringVersion << (short)lpVersion->major << "." << (short)lpVersion->minor << "." << (short)lpVersion->patch;

	return lsStringVersion;
}


   /*=====================================================================*/


SPF_NAMESPACE_NESTED_END
SPF_NAMESPACE_END
