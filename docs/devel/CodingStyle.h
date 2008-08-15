/***************************************************************************
						CodingStyle.h    -  description
							-------------------
	begin                : november 10th, 2005
	copyright            : (C) 2005-2007 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_CODINGSTYLE_H_
#define _OPENCITY_CODINGSTYLE_H_ 1

#include "main.h"

#include <vector>					///< Standard headers come after our headers

#define OC_CODINGSTYLE_SOMECONST	1

class GUIContainer;					///< Forward class declaration


//========================================================================
/** Here is the description of the class.
This is the second line of the description.
	\author Duong-Khang NGUYEN
*/
class CodingStyle
{
public:
	enum OPENCITY_ENUM {
		OC_CS_SOMETHING1 = 0,
		OC_CS_SOMETHING2
	};

	CodingStyle(
		const bool& rcboolUseDL = true );		///< One parameter, two lines version method

	~CodingStyle(  );

	bool GetBool() const;				///< One line for short declaration
	uint GetSize() const;
	bool IsBig() const;
	long GetLong( uint uiX ) const;		///< One parameter, one line version method


//========================================================================
/** This line describes the method's role and goal.
The second line of description goes here.
	\param rciA A constant integer reference
	\param rciB Another constant integer reference
	\return An int
*/
	int
	GetLongLongProperty(
		const int& rciA,
		const int& rciB ) const;		///< Multi-line for long declaration


//========================================================================
// Static attributes
//========================================================================
	static uint uiAttr;


//========================================================================
// Static methods
//========================================================================
	static void
	DoStaticThing();


private:
/** Variable prefixes:
	_:		private member
	g:		global variable

	a:		array
	b:		boolean
	c:		constant
	d:		double
	dq:		deque (double queue)
	e:		enumeration
	i:		integer
	f:		float
	l:		long
	p:		pointer
	q:		std::queue
	r:		reference
	s:		structure
	str:	std::string
	ui:		unsigned integer
*/
	bool	_bABool;					///< _ (underscore) means something private
	string	_strAString;				///< A member std::string
	int		_iAnInt;
	uint	_uiAnotherUInt;
	double	_daADoubleArray[2];
	MyStruct		_sSomething;
	MyClass*		_pClassOne;			///< Use this
	MyClass			*_pWrongClassOne;	///< Not this
	OPENCITY_ENUM	_eAnEnum;


//========================================================================
// Private methods
//========================================================================

//========================================================================
/** This line describes the method's role and goal.
The second line of description goes here.
	\sa _PrivateMethod2()
*/
	void _PrivateMethod();


//========================================================================
/** Describe what it does
	\param x The amount of something
	\return True if something is true, false otherwise
	The return's description continues on the second line
*/
	bool _PrivateMethod2( int x );


	uint _PrivateMethod3(
		uint a,
		float b );
};

#endif



















