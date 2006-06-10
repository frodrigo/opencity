/***************************************************************************
                          macros.h  -  description
      $Id: macros.h,v 1.14 2006/06/05 09:58:06 neoneurone Exp $
                             -------------------
    begin                : dim sep 21 2003
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
	#endif

// Information message macro
	#define OPENCITY_INFO( msg ) { \
		cout << "<INFO> " << msg << endl; \
	}

// Error message macro
	#define OPENCITY_ERROR( msg ) { \
		cout << "<ERROR> " << msg << endl; \
	}

// Fatal message macro
	#define OPENCITY_FATAL( msg ) { \
		cerr << "<FATAL> " << msg << endl; \
	}
	
// Swap two variables so that a <= b
	#define OPENCITY_SWAP( a, b, type ) \
	{\
		if (a > b) {   \
			type c;\
			c = a; a = b; b = c;\
		}\
	}


#endif

