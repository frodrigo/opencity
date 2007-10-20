/***************************************************************************
						persistence.h  -  description
							-------------------
	begin                : june 6th, 2003
	copyright            : (C) 2003-2006 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_PERSISTENCE_H_
#define _OPENCITY_PERSISTENCE_H_ 1

#include "main.h"

#include <fstream>

/**
	Any class which needs to save their data in the external memory for 
future use should implement this interface.
*/
class Persistence {
public:
	Persistence();
	virtual ~Persistence(  );


//========================================================================
/** Save the data to the specified fstream
	\param rfs A reference to a file stream which is ready for writing
*/
	virtual void
	SaveTo( std::fstream& rfs ) = 0;


//========================================================================
/** Load the data from the specified stream
	\param rfs A reference to a file stream which is ready for reading
*/
	virtual void
	LoadFrom( std::fstream& rfs ) = 0;
};

#endif





























