/***************************************************************************
                      CodingStyle.cpp    -  description
         $Id: CodingStyle.cpp,v 1.2 2006/03/12 23:14:18 neoneurone Exp $
                             -------------------
    begin                : nov 14th 2005
    copyright            : (C) 2005 by Duong-Khang NGUYEN
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


   /*=====================================================================*/
CodingStyle::CodingStyle
(
	const bool & rcUseDL = true
)
{
	_boolABool = rcUseDL;
	OPENCITY_DEBUG( "ctor: some debug value:" << _boolABool );
}


   /*=====================================================================*/
CodingStyle::~CodingStyle()
{
	OPENCITY_DEBUG( "dtor" );
}


   /*=====================================================================*/
bool
CodingStyle::GetBool() const
{
	return _boolABool;
}


   /*=====================================================================*/
   /*                        PRIVATE     METHODS                          */
   /*=====================================================================*/
void
CodingStyle::_PrivateMethod()
{
}


   /*=====================================================================*/
void
CodingStyle::_PrivateMethod2()
{
}



















