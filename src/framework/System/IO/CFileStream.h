/***************************************************************************
                      CFileStream.h  -  description
							-------------------
	begin                : January 11th, 2009
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

#ifndef _OPENCITY_FRAMEWORK_SYSTEM_IO_CFILESTREAM_H_
#define _OPENCITY_FRAMEWORK_SYSTEM_IO_CFILESTREAM_H_ 1

// Framework headers
#include "CStream.h"			// System::IO::Stream class

SPF_NAMESPACE_BEGIN(System)

// Forward System::String class declaration
class String;

SPF_NAMESPACE_BEGIN(IO)

/**
	Exposes a Stream around a file.
	\see System::IO::Stream class
*/
class FileStream : public Stream
{
	public:
		FileStream();
		virtual ~FileStream();

		virtual String ToString() const;

}; // class System::IO::FileStream

// namespace System::IO
SPF_NAMESPACE_END

// namespace System
SPF_NAMESPACE_END

#endif
