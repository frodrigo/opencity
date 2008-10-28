/***************************************************************************
					extensionmanager.h    -    description
							-------------------
	begin                : august 15th, 2008
	copyright            : (C) 2006-2008 by Duong Khang NGUYEN
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

#ifndef _OPENCITY_EXTENSIONMANAGER_H_
#define _OPENCITY_EXTENSIONMANAGER_H_ 1

#include "main.h"

//========================================================================
/** Handles OpenGL extensions
*/
class ExtensionManager {
public:
	ExtensionManager();
	~ExtensionManager();

	bool Load(void);

//========================================================================
// OpengGL 1.2 extensions
//========================================================================
	PFNGLTEXIMAGE3DEXTPROC glTexImage3D;

private:
	bool _bInitialized;
};

#endif
