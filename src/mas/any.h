/***************************************************************************
                           any.h  -  description
         $Id$
                             -------------------
    begin                : nov 29th 2005
    copyright            : (C) 2005 by Duong-Khang NGUYEN
    email                : neoneurone @ users sourceforge net
    author               : Victor STINNER
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/


#ifndef _OPENCITY_ANY_H_
#define _OPENCITY_ANY_H_ 1

#include <string>

typedef enum {
	ANY_INT=0,
	ANY_UINT,
	ANY_DOUBLE,
	ANY_STRING
} Any_t;


   /*=====================================================================*/
/**
	@author Victor STINNER
*/
class Any
{
public:
	explicit Any(int value);
	explicit Any(unsigned int value);
	explicit Any(double value);
	explicit Any(const std::string& value);

	Any_t getType() const;
	int getInt() const;
	unsigned int getUInt() const;
	double getDouble() const;	
	const std::string& getString() const;

	friend std::ostream& operator<<(std::ostream& os, const Any& any);

private:
	Any_t m_type;
	int m_int_value;
	unsigned int m_uint_value;
	double m_double_value;
	std::string m_string_value;
};

#endif































