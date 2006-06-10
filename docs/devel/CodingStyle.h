/***************************************************************************
                          CodingStyle.h    -  description
         $Id: CodingStyle.h,v 1.5 2006/03/12 23:14:18 neoneurone Exp $

                             -------------------
    begin                : nov 10th 2005
    copyright            : (C) 2005-2006 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_CODINGSTYLE_H_
#define _OPENCITY_CODINGSTYLE_ 1

#include "main.h"

#include <vector>

#define OC_CODINGSTYLE_SOMECONST	1

class GUIContainer;		///< Forward class declaration


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
		const bool& rcboolUseDL = true );

	~CodingStyle(  );

	bool GetBool() const;			///< One line for short declaration
	OC_UINT GetSize() const;
	bool IsBig() const;

	int
	GetLongLongProperty(
		const int& rciA,
		const int& rciB) const;		///< Multi-line for long declaration


private:
	bool _boolABool;		///< _ (underscore) means something private
	string _strAString;		///< A member std::string
	int _iAnInt;
	OC_UINT _uiAnUInt;
	uint _uiAnotherUInt;


   /*=====================================================================*/
   /*                        PRIVATE     METHODS                          */
   /*=====================================================================*/

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
};

#endif



















