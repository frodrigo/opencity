/***************************************************************************
                          pathstructure.cpp  -  description
      $Id$
                             -------------------
    begin                : june 17th, 2003
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

#include "pathstructure.h"
#include "propertymanager.h"


extern PropertyManager* gpPropertyMgr;	// global property manager


   /*======================================================================*/
PathStructure::PathStructure():
Structure( OC_STRUCTURE_UNDEFINED ),
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

	this->enumGraphicCode = gpPropertyMgr->GetGC( enumStructCode );
	_enumType = gpPropertyMgr->GetST( enumStructCode );

//debug
//cout << "struct: " << (int)enumStructCode
//     << "/ graphic: " << (int)enumGraphicCode << endl;

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

	rfs << ubNumberNeighbour << std::endl;
	rfs << ubTraffic << std::endl;
}


   /*======================================================================*/
void
PathStructure::LoadFrom( std::fstream& rfs )
{
	Structure::LoadFrom( rfs );

	rfs >> ubNumberNeighbour; rfs.ignore();
	rfs >> ubTraffic; rfs.ignore();
}


   /*======================================================================*/
void
PathStructure::AddNeighbour(
	const PathStructure & rcNeighbour,
	const OPENCITY_DIRECTION & enumDir )
{
	OPENCITY_GRAPHIC_CODE baseGC;
	uint newGC;

//debug cout << "adding 1 neighbour" << endl;

	switch (enumStructureCode) {
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

   // we will update the enumGraphicCode with the newGC value
   // when we return, so we keep with this
	newGC = enumGraphicCode;

	if (ubNumberNeighbour == 0) {
	   // the current enumGraphicCode is already OC_ROAD_O_N
	   // we don't process further in such case
		if (enumDir == OC_DIR_E)
			newGC = baseGC + 1;
		else if (enumDir == OC_DIR_S)
			newGC = baseGC + 2;
		else if (enumDir == OC_DIR_W)
			newGC = baseGC + 3;
	} else

		/*-----------------------------------------*/
	if (ubNumberNeighbour == 1) {
//debug cout << "number neighbour == 1" << endl;
		if (enumDir == OC_DIR_N) {
		   // we don't process the case OC_ROAD_O_N
			if (enumGraphicCode == baseGC + 1)
				newGC = baseGC + 6;
			else if (enumGraphicCode == baseGC + 2)
				newGC = baseGC + 4;
			else if (enumGraphicCode == baseGC + 3)
				newGC = baseGC + 7;
			else
				return;
		} else

		if (enumDir == OC_DIR_E) {
		   // we don't process the case OC_ROAD_O_E
			if (enumGraphicCode == baseGC)
				newGC = baseGC + 6;
			else if (enumGraphicCode == baseGC + 2)
				newGC = baseGC + 8;
			else if (enumGraphicCode == baseGC + 3)
				newGC = baseGC + 5;
			else
				return;
		} else

		if (enumDir == OC_DIR_S) {
		   // we don't process the case OC_ROAD_O_S
			if (enumGraphicCode == baseGC)
				newGC = baseGC + 4;
			else if (enumGraphicCode == baseGC + 1)
				newGC = baseGC + 8;
			else if (enumGraphicCode == baseGC + 3)
				newGC = baseGC + 9;
			else
				return;
		} else

		if (enumDir == OC_DIR_W) {
		   // we don't process the case OC_ROAD_O_W
			if (enumGraphicCode == baseGC)
				newGC = baseGC + 7;
			else if (enumGraphicCode == baseGC + 1)
				newGC = baseGC + 5;
			else if (enumGraphicCode == baseGC + 2)
				newGC = baseGC + 9;
			else
				return;
		}
	} else

		//-----------------------------------------
	if (ubNumberNeighbour == 2) {
		if (enumDir == OC_DIR_N) {
		   // we don't process the case OC_ROAD_S_N, N_E, N_W
			if (enumGraphicCode == baseGC + 8)
				newGC = baseGC + 10;
			else if (enumGraphicCode == baseGC + 9)
				newGC = baseGC + 11;
			else if (enumGraphicCode == baseGC + 5)
				newGC = baseGC + 13;
			else
				return;
		} else

		if (enumDir == OC_DIR_E) {
		   // we don't process the case OC_ROAD_W_E, N_E, S_E
			if (enumGraphicCode == baseGC + 9)
				newGC = baseGC + 11;
			else if (enumGraphicCode == baseGC + 7)
				newGC = baseGC + 13;
			else if (enumGraphicCode == baseGC + 4)
				newGC = baseGC + 10;
			else
				return;
		} else

		if (enumDir == OC_DIR_S) {
		   // we don't process the case OC_ROAD_S_N, S_E, S_W
			if (enumGraphicCode == baseGC + 6)
				newGC = baseGC + 10;
			else if (enumGraphicCode == baseGC + 7)
				newGC = baseGC + 12;
			else if (enumGraphicCode == baseGC + 5)
				newGC = baseGC + 11;
			else
				return;
		} else

		if (enumDir == OC_DIR_W) {
		   // we don't process the case OC_ROAD_W_E, N_W, S_W
			if (enumGraphicCode == baseGC + 8)
				newGC = baseGC + 11;
			else if (enumGraphicCode == baseGC + 6)
				newGC = baseGC + 13;
			else if (enumGraphicCode == baseGC + 4)
				newGC = baseGC + 12;
			else
				return;
		}
	} else

		//-----------------------------------------
	if (ubNumberNeighbour == 3) {
		if (enumDir == OC_DIR_N) {
			if (enumGraphicCode == baseGC + 11)
				newGC = baseGC + 14;
			else
				return;
		} else
		if (enumDir == OC_DIR_E) {
			if (enumGraphicCode == baseGC + 12)
				newGC = baseGC + 14;
			else
				return;
		} else
		if (enumDir == OC_DIR_S) {
			if (enumGraphicCode == baseGC + 13)
				newGC = baseGC + 14;
			else
				return;
		} else
		if (enumDir == OC_DIR_W) {
			if (enumGraphicCode == baseGC + 10)
				newGC = baseGC + 14;
			else
				return;
		}
	}
	else {
	   // impossible case !
		cout << "WARNING: internal game design error !" << endl;
		return;
	}

	enumGraphicCode = (OPENCITY_GRAPHIC_CODE)newGC;
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
	OPENCITY_GRAPHIC_CODE baseGC;
	uint newGC;

   // note: we do nothing when number of neighbours is 0
	if (ubNumberNeighbour == 0) {
		return;
	}

	switch (this->enumStructureCode) {
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

   // we will update the enumGraphicCode with the newGC value
   // when we return, so we keep with this
	newGC = enumGraphicCode;

	if (ubNumberNeighbour == 1) {
	   // we have no neighbour so the default path structure
	   // look to the NORTH
		newGC = baseGC;
	} else

		//-----------------------------------------

	if (ubNumberNeighbour == 2) {
//debug cout << "number neighbour == 1" << endl;
		if (enumDir == OC_DIR_N) {
			if (enumGraphicCode == baseGC + 4)
				newGC = baseGC + 2;
			else if (enumGraphicCode == baseGC + 6)
				newGC = baseGC + 1;
			else if (enumGraphicCode == baseGC + 7)
				newGC = baseGC + 3;
			else
				return;
		} else

		if (enumDir == OC_DIR_E) {
			if (enumGraphicCode == baseGC + 6)
				newGC = baseGC;
			else if (enumGraphicCode == baseGC + 8)
				newGC = baseGC + 2;
			else if (enumGraphicCode == baseGC + 5)
				newGC = baseGC + 3;
			else
				return;
		} else

		if (enumDir == OC_DIR_S) {
			if (enumGraphicCode == baseGC + 4)
				newGC = baseGC;
			else if (enumGraphicCode == baseGC + 8)
				newGC = baseGC + 1;
			else if (enumGraphicCode == baseGC + 9)
				newGC = baseGC + 3;
			else
				return;
		} else

		if (enumDir == OC_DIR_W) {
			if (enumGraphicCode == baseGC + 7)
				newGC = baseGC;
			else if (enumGraphicCode == baseGC + 5)
				newGC = baseGC + 1;
			else if (enumGraphicCode == baseGC + 9)
				newGC = baseGC + 2;
			else
				return;
		}
	} else

		//-----------------------------------------
	if (ubNumberNeighbour == 3) {
		if (enumDir == OC_DIR_N) {
			if (enumGraphicCode == baseGC + 10)
				newGC = baseGC + 8;
			else if (enumGraphicCode == baseGC + 11)
				newGC = baseGC + 9;
			else if (enumGraphicCode == baseGC + 13)
				newGC = baseGC + 5;
			else
				return;
		} else

		if (enumDir == OC_DIR_E) {
			if (enumGraphicCode == baseGC + 11)
				newGC = baseGC + 9;
			else if (enumGraphicCode == baseGC + 13)
				newGC = baseGC + 7;
			else if (enumGraphicCode == baseGC + 10)
				newGC = baseGC + 4;
			else
				return;
		} else

		if (enumDir == OC_DIR_S) {
			if (enumGraphicCode == baseGC + 10)
				newGC = baseGC + 6;
			else if (enumGraphicCode == baseGC + 12)
				newGC = baseGC + 7;
			else if (enumGraphicCode == baseGC + 11)
				newGC = baseGC + 5;
			else
				return;
		} else

		if (enumDir == OC_DIR_W) {
			if (enumGraphicCode == baseGC + 11)
				newGC = baseGC + 8;
			else if (enumGraphicCode == baseGC + 13)
				newGC = baseGC + 6;
			else if (enumGraphicCode == baseGC + 12)
				newGC = baseGC + 4;
			else
				return;
		}
	} else

		//-----------------------------------------
	if (ubNumberNeighbour == 4) {
		if (enumDir == OC_DIR_N) {
			newGC = baseGC + 11;
		} else
		if (enumDir == OC_DIR_E) {
			newGC = baseGC + 12;
		} else
		if (enumDir == OC_DIR_S) {
			newGC = baseGC + 13;
		} else
		if (enumDir == OC_DIR_W) {
			newGC = baseGC + 10;
		}
	}

	enumGraphicCode = (OPENCITY_GRAPHIC_CODE)newGC;
	ubNumberNeighbour--;
}


   /*======================================================================*/
//FOR FUTURE REFERENCE
/*
void
pathstructureAddNeighbour2(
	const PathStructure & rcNeighbour,
	const OPENCITY_DIRECTION & enumDir )
{
//debug
cout << "adding 1 neighbour" << endl;

	if (ubNumberNeighbour == 0) {
	   // the current enumGraphicCode is already OC_ROAD_O_N
	   // we don't process further in such case
		if (enumDir == OC_DIR_E)
			enumGraphicCode = OC_ROAD_O_E;
		else if (enumDir == OC_DIR_S)
			enumGraphicCode = OC_ROAD_O_S;
		else if (enumDir == OC_DIR_W)
			enumGraphicCode = OC_ROAD_O_W;
	} else

		*-----------------------------------------*
	if (ubNumberNeighbour == 1) {
//debug
cout << "number neighbour == 1" << endl;
		if (enumDir == OC_DIR_N) {
		   // we don't process the case OC_ROAD_O_N
			if (enumGraphicCode == OC_ROAD_O_E)
				enumGraphicCode = OC_ROAD_N_E;
			else if (enumGraphicCode == OC_ROAD_O_S)
				enumGraphicCode = OC_ROAD_S_N;
			else if (enumGraphicCode == OC_ROAD_O_W)
				enumGraphicCode = OC_ROAD_N_W;
			else
				return;
		} else

		if (enumDir == OC_DIR_E) {
		   // we don't process the case OC_ROAD_O_E
			if (enumGraphicCode == OC_ROAD_O_N)
				enumGraphicCode = OC_ROAD_N_E;
			else if (enumGraphicCode == OC_ROAD_O_S)
				enumGraphicCode = OC_ROAD_S_E;
			else if (enumGraphicCode == OC_ROAD_O_W)
				enumGraphicCode = OC_ROAD_W_E;
			else
				return;
		} else

		if (enumDir == OC_DIR_S) {
		   // we don't process the case OC_ROAD_O_S
			if (enumGraphicCode == OC_ROAD_O_N)
				enumGraphicCode = OC_ROAD_S_N;
			else if (enumGraphicCode == OC_ROAD_O_E)
				enumGraphicCode = OC_ROAD_S_E;
			else if (enumGraphicCode == OC_ROAD_O_W)
				enumGraphicCode = OC_ROAD_S_W;
			else
				return;
		} else

		if (enumDir == OC_DIR_W) {
		   // we don't process the case OC_ROAD_O_W
			if (enumGraphicCode == OC_ROAD_O_N)
				enumGraphicCode = OC_ROAD_N_W;
			else if (enumGraphicCode == OC_ROAD_O_E)
				enumGraphicCode = OC_ROAD_W_E;
			else if (enumGraphicCode == OC_ROAD_O_S)
				enumGraphicCode = OC_ROAD_S_W;
			else
				return;
		}
	} else

		*-----------------------------------------*
	if (ubNumberNeighbour == 2) {
		if (enumDir == OC_DIR_N) {
		   // we don't process the case OC_ROAD_S_N, N_E, N_W
			if (enumGraphicCode == OC_ROAD_S_E)
				enumGraphicCode = OC_ROAD_S_N_E;
			else if (enumGraphicCode == OC_ROAD_S_W)
				enumGraphicCode = OC_ROAD_S_W_E;
			else if (enumGraphicCode == OC_ROAD_W_E)
				enumGraphicCode = OC_ROAD_N_W_E;
			else
				return;
		} else

		if (enumDir == OC_DIR_E) {
		   // we don't process the case OC_ROAD_W_E, N_E, S_E
			if (enumGraphicCode == OC_ROAD_S_W)
				enumGraphicCode = OC_ROAD_S_W_E;
			else if (enumGraphicCode == OC_ROAD_N_W)
				enumGraphicCode = OC_ROAD_N_W_E;
			else if (enumGraphicCode == OC_ROAD_S_N)
				enumGraphicCode = OC_ROAD_S_N_E;
			else
				return;
		} else

		if (enumDir == OC_DIR_S) {
		   // we don't process the case OC_ROAD_S_N, S_E, S_W
			if (enumGraphicCode == OC_ROAD_N_E)
				enumGraphicCode = OC_ROAD_S_N_E;
			else if (enumGraphicCode == OC_ROAD_N_W)
				enumGraphicCode = OC_ROAD_S_N_W;
			else if (enumGraphicCode == OC_ROAD_W_E)
				enumGraphicCode = OC_ROAD_S_W_E;
			else
				return;
		} else

		if (enumDir == OC_DIR_W) {
		   // we don't process the case OC_ROAD_W_E, N_W, S_W
			if (enumGraphicCode == OC_ROAD_S_E)
				enumGraphicCode = OC_ROAD_S_W_E;
			else if (enumGraphicCode == OC_ROAD_N_E)
				enumGraphicCode = OC_ROAD_N_W_E;
			else if (enumGraphicCode == OC_ROAD_S_N)
				enumGraphicCode = OC_ROAD_S_N_W;
			else
				return;
		}
	} else

		*-----------------------------------------*
	if (ubNumberNeighbour == 3) {
		if (enumDir == OC_DIR_N) {
			if (enumGraphicCode == OC_ROAD_S_W_E)
				enumGraphicCode = OC_ROAD_S_N_W_E;
			else
				return;
		} else
		if (enumDir == OC_DIR_E) {
			if (enumGraphicCode == OC_ROAD_S_N_W)
				enumGraphicCode = OC_ROAD_S_N_W_E;
			else
				return;
		} else
		if (enumDir == OC_DIR_S) {
			if (enumGraphicCode == OC_ROAD_N_W_E)
				enumGraphicCode = OC_ROAD_S_N_W_E;
			else
				return;
		} else
		if (enumDir == OC_DIR_W) {
			if (enumGraphicCode == OC_ROAD_S_N_E)
				enumGraphicCode = OC_ROAD_S_N_W_E;
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
const OC_UBYTE &
PathStructure::GetTraffic() const
{
	return this->ubTraffic;
}


   /*======================================================================*/
void
PathStructure::SetTraffic(
	const OC_UBYTE & rubTraffic )
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
































