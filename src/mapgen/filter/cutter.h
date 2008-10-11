/***************************************************************************
						flattern.h  -  description
							-------------------
	begin                : july 16th, 2007
	copyright            : (C) 2007 by Frédéric RODRIGO
	email                : f.rodrigo free.fr
	
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

#ifndef _OPENCITY_CUTTER_H_
#define _OPENCITY_CUTTER_H_ 1

#include "filter.h"

namespace MapGen
{

class CutterFunctor {
public:
	virtual bool operator()( const float value );
};

//========================================================================
/** Set value to default where functor is true
*/
class Cutter: public Filter {
public:
	Cutter( CutterFunctor* cutterfunctor, const float defaultValue=0 );
	~Cutter();

	void apply( Map* map );

private:
	CutterFunctor*	_cutterfunctor;
	float	_defaultValue;

};

// Reusability ...

class CutterFunctorConstant : public CutterFunctor {
public:
	CutterFunctorConstant( const float value, const bool higher=false );

	virtual bool operator()( const float value );

private:
	float	_value;
	bool	_higher;

};

class CutterFunctorOnlyPositive : public CutterFunctorConstant {
public:
	CutterFunctorOnlyPositive();
};


}

#endif
