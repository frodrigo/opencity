/***************************************************************************
						pathstructure.cpp  -  description
							-------------------
	begin                : june 17th, 2003
	copyright            : (C) 2003-2007 by Duong Khang NGUYEN
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

// Useful enumerations
#include "opencity_direction.h"
#include "opencity_structure_type.h"

// OpenCity header
#include "pathstructure.h"

// Global settings
#include "globalvar.h"
extern GlobalVar gVars;


   /*======================================================================*/
PathStructure::PathStructure():
Structure(),
ubNumberNeighbour( 0 ),
ubTraffic( 0 )
{
	OPENCITY_DEBUG( "ctor" );
}


   /*======================================================================*/
PathStructure::PathStructure
(
	const OPENCITY_STRUCTURE_CODE & enumStructCode
):
Structure( enumStructCode ),
ubNumberNeighbour( 0 ),
ubTraffic( 0 )
{
	OPENCITY_DEBUG( "ctor param" );

	_eGC = gVars.gpPropertyMgr->GetGC( enumStructCode );
	_eType = gVars.gpPropertyMgr->GetST( enumStructCode );

//debug
//cout << "struct: " << (int)enumStructCode
//     << "/ graphic: " << (int)_eGC << endl;

}


   /*======================================================================*/
PathStructure::~PathStructure()
{
	OPENCITY_DEBUG( "dtor" );
}


   /*======================================================================*/
void
PathStructure::SaveTo( std::fstream& rfs )
{
	Structure::SaveTo( rfs );

	rfs << (uint)ubNumberNeighbour << std::ends;
	rfs << (uint)ubTraffic << std::ends;
}


   /*======================================================================*/
void
PathStructure::LoadFrom( std::fstream& rfs )
{
	Structure::LoadFrom( rfs );
	uint temp = 0;

	rfs >> temp; rfs.ignore(); ubNumberNeighbour = (OC_BYTE)temp;
	rfs >> temp; rfs.ignore(); ubTraffic		 = (OC_BYTE)temp;
}


   /*======================================================================*/
void
PathStructure::AddNeighbour(
	const PathStructure & rcNeighbour,
	const OPENCITY_DIRECTION & enumDir )
{
	OPENCITY_GRAPHIC_CODE baseGC = OC_EMPTY;
	uint newGC;

//debug cout << "adding 1 neighbour" << endl;

	switch (_eSC) {
		case OC_STRUCTURE_ROAD:
			baseGC = OC_ROAD_O_N;
			break;
		case OC_STRUCTURE_ELINE:
			baseGC = OC_ELINE_O_N;
			break;
		default:
			OPENCITY_DEBUG( "Game design ERROR" );
			break;
	}

// We will update the _eGC with the newGC value
// when we return, so we keep with this
	newGC = _eGC;

	if (ubNumberNeighbour == 0) {
	// The current _eGC is already OC_ROAD_O_N we don't process further in such case
		if (enumDir == OC_DIR_O_E)
			newGC = baseGC + 1;
		else if (enumDir == OC_DIR_O_S)
			newGC = baseGC + 2;
		else if (enumDir == OC_DIR_O_W)
			newGC = baseGC + 3;
	} else

		/*-----------------------------------------*/
	if (ubNumberNeighbour == 1) {
//debug cout << "number neighbour == 1" << endl;
		if (enumDir == OC_DIR_O_N) {
		// We don't process the case OC_ROAD_O_N
			if (_eGC == baseGC + 1)
				newGC = baseGC + 6;
			else if (_eGC == baseGC + 2)
				newGC = baseGC + 4;
			else if (_eGC == baseGC + 3)
				newGC = baseGC + 7;
			else
				return;
		} else

		if (enumDir == OC_DIR_O_E) {
		   // we don't process the case OC_ROAD_O_E
			if (_eGC == baseGC)
				newGC = baseGC + 6;
			else if (_eGC == baseGC + 2)
				newGC = baseGC + 8;
			else if (_eGC == baseGC + 3)
				newGC = baseGC + 5;
			else
				return;
		} else

		if (enumDir == OC_DIR_O_S) {
		   // we don't process the case OC_ROAD_O_S
			if (_eGC == baseGC)
				newGC = baseGC + 4;
			else if (_eGC == baseGC + 1)
				newGC = baseGC + 8;
			else if (_eGC == baseGC + 3)
				newGC = baseGC + 9;
			else
				return;
		} else

		if (enumDir == OC_DIR_O_W) {
		   // we don't process the case OC_ROAD_O_W
			if (_eGC == baseGC)
				newGC = baseGC + 7;
			else if (_eGC == baseGC + 1)
				newGC = baseGC + 5;
			else if (_eGC == baseGC + 2)
				newGC = baseGC + 9;
			else
				return;
		}
	} else

		//-----------------------------------------
	if (ubNumberNeighbour == 2) {
		if (enumDir == OC_DIR_O_N) {
		// We don't process the case OC_ROAD_S_N, N_E, N_W
			if (_eGC == baseGC + 8)			// S_E
				newGC = baseGC + 10;		// S_N_E
			else if (_eGC == baseGC + 9)	// S_W
				newGC = baseGC + 12;		// S_N_W
			else if (_eGC == baseGC + 5)	// W_E
				newGC = baseGC + 13;		// N_W_E
			else
				return;
		} else

		if (enumDir == OC_DIR_O_E) {
		// We don't process the case OC_ROAD_W_E, N_E, S_E
			if (_eGC == baseGC + 9)
				newGC = baseGC + 11;
			else if (_eGC == baseGC + 7)
				newGC = baseGC + 13;
			else if (_eGC == baseGC + 4)
				newGC = baseGC + 10;
			else
				return;
		} else

		if (enumDir == OC_DIR_O_S) {
		// We don't process the case OC_ROAD_S_N, S_E, S_W
			if (_eGC == baseGC + 6)
				newGC = baseGC + 10;
			else if (_eGC == baseGC + 7)
				newGC = baseGC + 12;
			else if (_eGC == baseGC + 5)
				newGC = baseGC + 11;
			else
				return;
		} else

		if (enumDir == OC_DIR_O_W) {
		// We don't process the case OC_ROAD_W_E, N_W, S_W
			if (_eGC == baseGC + 8)
				newGC = baseGC + 11;
			else if (_eGC == baseGC + 6)
				newGC = baseGC + 13;
			else if (_eGC == baseGC + 4)
				newGC = baseGC + 12;
			else
				return;
		}
	} else

		//-----------------------------------------
	if (ubNumberNeighbour == 3) {
		if (enumDir == OC_DIR_O_N) {
			if (_eGC == baseGC + 11)
				newGC = baseGC + 14;
			else
				return;
		} else
		if (enumDir == OC_DIR_O_E) {
			if (_eGC == baseGC + 12)
				newGC = baseGC + 14;
			else
				return;
		} else
		if (enumDir == OC_DIR_O_S) {
			if (_eGC == baseGC + 13)
				newGC = baseGC + 14;
			else
				return;
		} else
		if (enumDir == OC_DIR_O_W) {
			if (_eGC == baseGC + 10)
				newGC = baseGC + 14;
			else
				return;
		}
	}
	else {
	// impossible case !
		OPENCITY_FATAL( "Internal game design error !" );
		exit(-1);
		return;
	}

	_eGC = (OPENCITY_GRAPHIC_CODE)newGC;
	ubNumberNeighbour++;

//debug
//cout << "newGC: " << (int) newGC
//     << " / nNeighbours: " << (int)ubNumberNeighbour << endl;
//debug end
}


   /*=====================================================================*/
void
PathStructure::RemoveNeighbour(
	const PathStructure & rcNeighbour,
	const OPENCITY_DIRECTION & enumDir )
{
	OPENCITY_GRAPHIC_CODE baseGC = OC_EMPTY;
	uint newGC;

   // note: we do nothing when number of neighbours is 0
	if (ubNumberNeighbour == 0) {
		return;
	}

	switch (_eSC) {
		case OC_STRUCTURE_ROAD:
			baseGC = OC_ROAD_O_N;
			break;
		case OC_STRUCTURE_ELINE:
			baseGC = OC_ELINE_O_N;
			break;
		default:
			cout << "WARNING: game design error" << endl;
			break;
	}

   // we will update the _eGC with the newGC value
   // when we return, so we keep with this
	newGC = _eGC;

	if (ubNumberNeighbour == 1) {
	   // we have no neighbour so the default path structure
	   // look to the NORTH
		newGC = baseGC;
	} else

		//-----------------------------------------

	if (ubNumberNeighbour == 2) {
//debug cout << "number neighbour == 1" << endl;
		if (enumDir == OC_DIR_O_N) {
			if (_eGC == baseGC + 4)
				newGC = baseGC + 2;
			else if (_eGC == baseGC + 6)
				newGC = baseGC + 1;
			else if (_eGC == baseGC + 7)
				newGC = baseGC + 3;
			else
				return;
		} else

		if (enumDir == OC_DIR_O_E) {
			if (_eGC == baseGC + 6)
				newGC = baseGC;
			else if (_eGC == baseGC + 8)
				newGC = baseGC + 2;
			else if (_eGC == baseGC + 5)
				newGC = baseGC + 3;
			else
				return;
		} else

		if (enumDir == OC_DIR_O_S) {
			if (_eGC == baseGC + 4)
				newGC = baseGC;
			else if (_eGC == baseGC + 8)
				newGC = baseGC + 1;
			else if (_eGC == baseGC + 9)
				newGC = baseGC + 3;
			else
				return;
		} else

		if (enumDir == OC_DIR_O_W) {
			if (_eGC == baseGC + 7)
				newGC = baseGC;
			else if (_eGC == baseGC + 5)
				newGC = baseGC + 1;
			else if (_eGC == baseGC + 9)
				newGC = baseGC + 2;
			else
				return;
		}
	} else

		//-----------------------------------------
	if (ubNumberNeighbour == 3) {
		if (enumDir == OC_DIR_O_N) {
			if (_eGC == baseGC + 10)
				newGC = baseGC + 8;
			else if (_eGC == baseGC + 12)
				newGC = baseGC + 9;
			else if (_eGC == baseGC + 13)
				newGC = baseGC + 5;
			else
				return;
		} else

		if (enumDir == OC_DIR_O_E) {
			if (_eGC == baseGC + 11)
				newGC = baseGC + 9;
			else if (_eGC == baseGC + 13)
				newGC = baseGC + 7;
			else if (_eGC == baseGC + 10)
				newGC = baseGC + 4;
			else
				return;
		} else

		if (enumDir == OC_DIR_O_S) {
			if (_eGC == baseGC + 10)
				newGC = baseGC + 6;
			else if (_eGC == baseGC + 12)
				newGC = baseGC + 7;
			else if (_eGC == baseGC + 11)
				newGC = baseGC + 5;
			else
				return;
		} else

		if (enumDir == OC_DIR_O_W) {
			if (_eGC == baseGC + 11)
				newGC = baseGC + 8;
			else if (_eGC == baseGC + 13)
				newGC = baseGC + 6;
			else if (_eGC == baseGC + 12)
				newGC = baseGC + 4;
			else
				return;
		}
	} else

		//-----------------------------------------
	if (ubNumberNeighbour == 4) {
		if (enumDir == OC_DIR_O_N) {
			newGC = baseGC + 11;
		} else
		if (enumDir == OC_DIR_O_E) {
			newGC = baseGC + 12;
		} else
		if (enumDir == OC_DIR_O_S) {
			newGC = baseGC + 13;
		} else
		if (enumDir == OC_DIR_O_W) {
			newGC = baseGC + 10;
		}
	}

	_eGC = (OPENCITY_GRAPHIC_CODE)newGC;
	ubNumberNeighbour--;
}


   /*======================================================================*/
// Old code, kept for future reference
/*
void
pathstructureAddNeighbour2(
	const PathStructure & rcNeighbour,
	const OPENCITY_DIRECTION & enumDir )
{
//debug
cout << "adding 1 neighbour" << endl;

	if (ubNumberNeighbour == 0) {
	   // the current _eGC is already OC_ROAD_O_N
	   // we don't process further in such case
		if (enumDir == OC_DIR_O_E)
			_eGC = OC_ROAD_O_E;
		else if (enumDir == OC_DIR_O_S)
			_eGC = OC_ROAD_O_S;
		else if (enumDir == OC_DIR_O_W)
			_eGC = OC_ROAD_O_W;
	} else

		*-----------------------------------------*
	if (ubNumberNeighbour == 1) {
//debug
cout << "number neighbour == 1" << endl;
		if (enumDir == OC_DIR_O_N) {
		   // we don't process the case OC_ROAD_O_N
			if (_eGC == OC_ROAD_O_E)
				_eGC = OC_ROAD_N_E;
			else if (_eGC == OC_ROAD_O_S)
				_eGC = OC_ROAD_S_N;
			else if (_eGC == OC_ROAD_O_W)
				_eGC = OC_ROAD_N_W;
			else
				return;
		} else

		if (enumDir == OC_DIR_O_E) {
		   // we don't process the case OC_ROAD_O_E
			if (_eGC == OC_ROAD_O_N)
				_eGC = OC_ROAD_N_E;
			else if (_eGC == OC_ROAD_O_S)
				_eGC = OC_ROAD_S_E;
			else if (_eGC == OC_ROAD_O_W)
				_eGC = OC_ROAD_W_E;
			else
				return;
		} else

		if (enumDir == OC_DIR_O_S) {
		   // we don't process the case OC_ROAD_O_S
			if (_eGC == OC_ROAD_O_N)
				_eGC = OC_ROAD_S_N;
			else if (_eGC == OC_ROAD_O_E)
				_eGC = OC_ROAD_S_E;
			else if (_eGC == OC_ROAD_O_W)
				_eGC = OC_ROAD_S_W;
			else
				return;
		} else

		if (enumDir == OC_DIR_O_W) {
		   // we don't process the case OC_ROAD_O_W
			if (_eGC == OC_ROAD_O_N)
				_eGC = OC_ROAD_N_W;
			else if (_eGC == OC_ROAD_O_E)
				_eGC = OC_ROAD_W_E;
			else if (_eGC == OC_ROAD_O_S)
				_eGC = OC_ROAD_S_W;
			else
				return;
		}
	} else

		*-----------------------------------------*
	if (ubNumberNeighbour == 2) {
		if (enumDir == OC_DIR_O_N) {
		   // we don't process the case OC_ROAD_S_N, N_E, N_W
			if (_eGC == OC_ROAD_S_E)
				_eGC = OC_ROAD_S_N_E;
			else if (_eGC == OC_ROAD_S_W)
				_eGC = OC_ROAD_S_W_E;
			else if (_eGC == OC_ROAD_W_E)
				_eGC = OC_ROAD_N_W_E;
			else
				return;
		} else

		if (enumDir == OC_DIR_O_E) {
		   // we don't process the case OC_ROAD_W_E, N_E, S_E
			if (_eGC == OC_ROAD_S_W)
				_eGC = OC_ROAD_S_W_E;
			else if (_eGC == OC_ROAD_N_W)
				_eGC = OC_ROAD_N_W_E;
			else if (_eGC == OC_ROAD_S_N)
				_eGC = OC_ROAD_S_N_E;
			else
				return;
		} else

		if (enumDir == OC_DIR_O_S) {
		   // we don't process the case OC_ROAD_S_N, S_E, S_W
			if (_eGC == OC_ROAD_N_E)
				_eGC = OC_ROAD_S_N_E;
			else if (_eGC == OC_ROAD_N_W)
				_eGC = OC_ROAD_S_N_W;
			else if (_eGC == OC_ROAD_W_E)
				_eGC = OC_ROAD_S_W_E;
			else
				return;
		} else

		if (enumDir == OC_DIR_O_W) {
		   // we don't process the case OC_ROAD_W_E, N_W, S_W
			if (_eGC == OC_ROAD_S_E)
				_eGC = OC_ROAD_S_W_E;
			else if (_eGC == OC_ROAD_N_E)
				_eGC = OC_ROAD_N_W_E;
			else if (_eGC == OC_ROAD_S_N)
				_eGC = OC_ROAD_S_N_W;
			else
				return;
		}
	} else

		*-----------------------------------------*
	if (ubNumberNeighbour == 3) {
		if (enumDir == OC_DIR_O_N) {
			if (_eGC == OC_ROAD_S_W_E)
				_eGC = OC_ROAD_S_N_W_E;
			else
				return;
		} else
		if (enumDir == OC_DIR_O_E) {
			if (_eGC == OC_ROAD_S_N_W)
				_eGC = OC_ROAD_S_N_W_E;
			else
				return;
		} else
		if (enumDir == OC_DIR_O_S) {
			if (_eGC == OC_ROAD_N_W_E)
				_eGC = OC_ROAD_S_N_W_E;
			else
				return;
		} else
		if (enumDir == OC_DIR_O_W) {
			if (_eGC == OC_ROAD_S_N_E)
				_eGC = OC_ROAD_S_N_W_E;
			else
				return;
		}
	}
	else {
	   // impossible case !
		cout << "WARNING: internal game design error !" << endl;
		return;
	}

	ubNumberNeighbour++;
}
*/


   /*======================================================================*/
const OC_BYTE &
PathStructure::GetTraffic() const
{
	return this->ubTraffic;
}


   /*======================================================================*/
void
PathStructure::SetTraffic(
	const OC_BYTE & rubTraffic )
{
	this->ubTraffic = rubTraffic;
}


   /*======================================================================*/
const uint &
PathStructure::GetLength() const
{
	return this->uiLength;
}


   /*======================================================================*/
void
PathStructure::SetLength(
	const uint & rcuiValue )
{
	this->uiLength = rcuiValue;
}
