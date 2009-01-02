/***************************************************************************
                     SharpPlusFramework.h  -  description
							-------------------
	begin                : January 2nd, 2009
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

#ifndef _OPENCITY_FRAMEWORK_SHARPPLUSFRAMEWORK_H_
#define _OPENCITY_FRAMEWORK_SHARPPLUSFRAMEWORK_H_ 1

#ifndef __GXX_RTTI
	#error "GCC RTTI support required."
#endif

#ifndef __EXCEPTIONS
	#error "C++ exception support required."
#endif

#define SPF_NAMESPACE_SEPARATOR				"::"
#define SPF_NAMESPACE_BEGIN(X)				namespace X {
#define SPF_NAMESPACE_END					}
#define SPF_NAMESPACE_NESTED_BEGIN(X, Y)	namespace X { namespace Y {
#define SPF_NAMESPACE_NESTED_END			} }

#endif
