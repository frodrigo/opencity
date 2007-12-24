/***************************************************************************
						macros.h  -  description
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

#ifndef _OPENCITY_MACROS_H_
#define _OPENCITY_MACROS_H_ 1

#include <iostream>
#include <cassert>			// for assert() => ifdef NDEBUG: do nothing
using std::cout;
using std::endl;
using std::cerr;

// Debug message macro
	#ifndef NDEBUG
		#define OPENCITY_DEBUG( msg ) { \
			cout << "<DEBUG> " << __FILE__ << " " << __LINE__ << " : " << msg << endl; \
		}
	#else
		#define OPENCITY_DEBUG(msg)
	#endif

// Information message macro
	#define OPENCITY_INFO( msg ) { \
		cout << "<INFO> " << msg << endl; \
	}

// Error message macro
	#define OPENCITY_ERROR( msg ) { \
		cerr << "<ERROR> " << msg << endl; \
	}

// Fatal message macro
	#define OPENCITY_FATAL( msg ) { \
		cerr << "<FATAL> " << __FILE__ << " " << __LINE__ << " : " << msg << endl; \
	}
	
// Swap two variables so that a <= b
	#define OPENCITY_SWAP( a, b, type ) \
	{\
		if (a > b) {   \
			type c;\
			c = a; a = b; b = c;\
		}\
	}
	
// We use "uint" for "unsigned int"
	#ifndef uint
		typedef unsigned int uint;
	#endif

// Define some missing magical macros for Visual Studio 2005
	#ifdef __WIN32__
		#if !defined(__PRETTY_FUNCTION__)
			#define __PRETTY_FUNCTION__ __FUNCTION__
		#endif

	// Include the text operator "and"
		#if !defined(and)
			#include <ciso646>
		#endif
	// strcasecmp workaround
		#if !defined(strcasecmp)
			#define strcasecmp _stricmp
		#endif
	/*
	// log2 workaround
		#if !defined(log2)
			#define log2(value) log((double)value)/log(2.0)
		#endif
	*/
	#endif // #if defined(__WIN32__)
#endif

