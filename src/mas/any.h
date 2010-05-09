/***************************************************************************
                           any.h  -  description
							-------------------
	begin                : nov 29th 2005
	copyright            : (C) 2005-2010 by Duong Khang NGUYEN
	email                : neoneurone @ gmail com
	author               : Victor STINNER

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


#ifndef _OPENCITY_MAS_ANY_H_
#define _OPENCITY_MAS_ANY_H_ 1

#include <string>


typedef enum {
	ANY_INT = 0,
	ANY_UINT,
	ANY_DOUBLE,
	ANY_STRING
} MAS_ANY_TYPE;


   /*=====================================================================*/
/** Any class is used to serialize data in order to send message from
an agent to another
	@author Victor STINNER
*/
class Any
{
public:
	explicit Any(int value);
	explicit Any(unsigned int value);
	explicit Any(double value);
	explicit Any(const std::string& value);

	MAS_ANY_TYPE getType() const;
	int getInt() const;
	unsigned int getUInt() const;
	double getDouble() const;
	const std::string& getString() const;

	friend std::ostream& operator<<(std::ostream& os, const Any& any);

private:
	MAS_ANY_TYPE _type;
	int _intValue;
	unsigned int _uintValue;
	double _doubleValue;
	std::string _stringValue;
};

#endif
