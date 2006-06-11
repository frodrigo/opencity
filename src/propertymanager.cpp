/***************************************************************************
                          propertymanager.cpp  -  description
      $Id$
                             -------------------
    begin                : feb 6th, 2004
    copyright            : (C) 2004-2006 by Duong-Khang NGUYEN
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

#include "propertymanager.h"

#include "structure.h"

#include <algorithm>		// We use STL algorithms
#include <sstream>			// We use stringstream or data conversion


   /*=====================================================================*/
PropertyManager::PropertyManager():
vpConf(OC_GRAPHIC_CODE_MAX, NULL)
{
	uint i;
	string str;
	string::size_type pos;
	string strAc = string(".ac");
	string strConf = string(".conf");
	std::stringstream ss;
	Conf* pconfFile;

	OPENCITY_DEBUG( "ctor" );

// read the main config file
	Conf* pConf = new Conf();
	if (pConf->Open( ocHomeDirPrefix(OC_GRAPHISM_FILE_FILENAME) ) == OC_ERR_FREE) {
	// initialize the Conf* vector
		for (i = 0; i < OC_GRAPHIC_CODE_MAX; i++ ) {
			ss << i;
//debugcout << "Mub: " << ss.str() << " / i: " << i << endl;
			str = pConf->GetValue(ss.str(), "");
			ss.str("");

		// IF the key is not defined in the config file THEN
			if (str != "") {
			// Reverse search for the ".ac" file extension
				pos = str.rfind( strAc );
				if (pos != str.npos ) {
				// Replace ".ac" by ".conf"
					str.replace( pos, strConf.size(), strConf );
				}

				pconfFile = new Conf();
				if (pconfFile->Open( ocHomeDirPrefix(str) ) == OC_ERR_FREE) {
//debug cout << "Loading: " << str.c_str() << " / at: " << i << endl;
					this->vpConf[i] = pconfFile;
				}
				else {
					delete pconfFile;
				}
			} // if (str=="")
		} // for

		pConf->Close();
	}
	else {
		cerr << "WARNING: Error opening graphism config file." << endl;
		abort();
	}
	delete pConf;
}


   /*=====================================================================*/
PropertyManager::~PropertyManager()
{
	OPENCITY_DEBUG( "dtor" );

// delete the Conf* vector
	for (int i = 0; i < OC_GRAPHIC_CODE_MAX; i++ ) {
		if (this->vpConf[i] != NULL) {
			this->vpConf[i]->Close();		// Close the file
			delete this->vpConf[i];			// NOTE: delete NULL pointer has no effect
		}
	}
}


   /*=====================================================================*/
const int
PropertyManager::Get(
	const OPENCITY_PROPERTY_CODE & pCode,
	const OPENCITY_STRUCTURE_CODE & sCode,
	const Structure* pcStruct ) const
{
	static int value;

	switch (pCode) {
		case OC_BUILD_COST:
			switch (sCode) {
				case OC_STRUCTURE_RES:
				case OC_STRUCTURE_COM:
					value = 5; break;
				case OC_STRUCTURE_IND:
					value = 8; break;
				case OC_STRUCTURE_PARK:
					value = 100; break;
				case OC_STRUCTURE_ELINE:
					value = 3; break;
				case OC_STRUCTURE_EPLANT_COAL:
					value = 2000; break;
				case OC_STRUCTURE_FIREDEPT:
				case OC_STRUCTURE_POLICEDEPT:
				case OC_STRUCTURE_EDUCATIONDEPT:
					value = 1500; break;
				case OC_STRUCTURE_ROAD:
					value = 5; break;

				default:
					value = 0;
					OPENCITY_DEBUG("WARNING: game design error");
					assert( 0 );
			}
			break;

		case OC_DESTROY_COST:
			assert( pcStruct != NULL );
			switch (sCode) {
				case OC_STRUCTURE_RES:
				case OC_STRUCTURE_COM:
					value = 2*(pcStruct->GetLevel() + 1); break;
				case OC_STRUCTURE_IND:
					value = 3*(pcStruct->GetLevel() + 1); break;

				case OC_STRUCTURE_PARK:
					value = 10; break;
				case OC_STRUCTURE_ELINE:
					value = 1; break;
				case OC_STRUCTURE_ROAD:
					value = 4; break;

				case OC_STRUCTURE_PART:
				case OC_STRUCTURE_EPLANT_COAL:
				case OC_STRUCTURE_FIREDEPT:
				case OC_STRUCTURE_POLICEDEPT:
				case OC_STRUCTURE_EDUCATIONDEPT:
					value = 200; break;

				default:
					value = 0;
					OPENCITY_DEBUG("WARNING: game design error");
					assert( 0 );
			}
			break;

		case OC_MAINTENANCE_COST:
			switch (sCode) {
				case OC_STRUCTURE_RES:
				case OC_STRUCTURE_COM:
				case OC_STRUCTURE_IND:
				case OC_STRUCTURE_PART:
					value = 0; break;
				case OC_STRUCTURE_PARK:
					value = 1; break;
				case OC_STRUCTURE_ELINE:
					value = 1; break;
				case OC_STRUCTURE_EPLANT_COAL:
					value = 20; break;
				case OC_STRUCTURE_FIREDEPT:
				case OC_STRUCTURE_POLICEDEPT:
				case OC_STRUCTURE_EDUCATIONDEPT:
					value = 15; break;
				case OC_STRUCTURE_ROAD:
					value = 1; break;

				default:
					value = 0;
					OPENCITY_DEBUG( "WARNING: game design error" );
					assert( 0 );
			}
			break;

		default:
			value = 0;
			OPENCITY_DEBUG("WARNING: game design error");
			assert( 0 );
	}

/*
#ifndef NDEBUG
// Debug code
	if ( value == 0 ) {
		cerr << "pCode: " << pCode << "/ sCode: " << sCode 
			 << "/ pcStruct: " << pcStruct << endl;
		assert( 0 );
	}
#endif
*/

	return value;
}


   /*=====================================================================*/
const OPENCITY_GRAPHIC_CODE
PropertyManager::GetGC(
	const OPENCITY_STRUCTURE_CODE scode ) const
{
// For better readability and performance (use of "static" keyword)
	static OPENCITY_GRAPHIC_CODE gcode;

	switch (scode) {
		case OC_STRUCTURE_UNDEFINED:
			gcode = OC_EMPTY;
			break;

	// Multi level graphic code
		case OC_STRUCTURE_RES:
			gcode = OC_RES_ZONE0;
			break;
		case OC_STRUCTURE_COM:
			gcode = OC_COM_ZONE0;
			break;
		case OC_STRUCTURE_IND:
			gcode = OC_IND_ZONE0;
			break;
		case OC_STRUCTURE_PARK:
			gcode = OC_PARK0;
			break;

	// Special path type structure
		case OC_STRUCTURE_ROAD:
			gcode = OC_ROAD_O_N;
			break;
		case OC_STRUCTURE_ELINE:
			gcode = OC_ELINE_O_N;
			break;

	// Single level graphic code
		case OC_STRUCTURE_TEST:
			gcode = OC_TEST_BUILDING;
			break;
		case OC_STRUCTURE_PART:
			gcode = OC_EMPTY;
			break;
		case OC_STRUCTURE_EPLANT_COAL:
			gcode = OC_EPLANT_COAL_BOX;
			break;
		case OC_STRUCTURE_FIREDEPT:
			gcode = OC_FIRE_DEPT;
			break;
		case OC_STRUCTURE_POLICEDEPT:
			gcode = OC_POLICE_DEPT;
			break;
		case OC_STRUCTURE_EDUCATIONDEPT:
			gcode = OC_EDUCATION_DEPT;
			break;

		default:
			gcode = OC_EMPTY;
			OPENCITY_DEBUG( "WARNING: game design error, unknown structure !" );
			assert( 0 );
			break;
	}

	return gcode;
}


   /*=====================================================================*/
const OPENCITY_STRUCTURE_TYPE
PropertyManager::GetST(
	const OPENCITY_STRUCTURE_CODE scode ) const
{
// For better readability and performance (use of "static" keyword)
	static OPENCITY_STRUCTURE_TYPE tcode;

	switch (scode) {
		case OC_STRUCTURE_RES:
		case OC_STRUCTURE_PARK:
			tcode = OC_STRUCTURE_RESIDENCE; break;
		case OC_STRUCTURE_COM:
			tcode = OC_STRUCTURE_COMMERCE; break;
		case OC_STRUCTURE_IND:
			tcode = OC_STRUCTURE_INDUSTRY; break;

		case OC_STRUCTURE_EPLANT_COAL:
			tcode = OC_STRUCTURE_ELECTRICITY; break;

		case OC_STRUCTURE_ROAD:
		case OC_STRUCTURE_ELINE:
			tcode = OC_STRUCTURE_PATH; break;

		case OC_STRUCTURE_FIREDEPT:
		case OC_STRUCTURE_POLICEDEPT:
		case OC_STRUCTURE_HOSPITALDEPT:
		case OC_STRUCTURE_MILITARYDEPT:
		case OC_STRUCTURE_EDUCATIONDEPT:
			tcode = OC_STRUCTURE_GOVERNMENT; break;

		case OC_STRUCTURE_PART:
			tcode = OC_STRUCTURE_UNUSEDTYPE; break;

	// TODO
		case OC_STRUCTURE_UNDEFINED:
		case OC_STRUCTURE_TEST:
		case OC_STRUCTURE_ANY:
		case OC_STRUCTURE_ELECTRIC:
			tcode = OC_STRUCTURE_UNUSEDTYPE; assert( 0 ); break;

		default:
			tcode = OC_STRUCTURE_UNUSEDTYPE;
			OPENCITY_DEBUG( "WARNING: game design error" );
			assert( 0 );
	}

	return tcode;
}


   /*=====================================================================*/
void
PropertyManager::GetWLH(
	const OPENCITY_GRAPHIC_CODE & gcode,
	uint & w, const uint defw,
	uint & l, const uint defl,
	uint & h, const uint defh)
{
	static OC_LINT lint;

	propertymanagerGetLint( gcode, "width", lint, defw );
	w = lint;
	propertymanagerGetLint( gcode, "length", lint, defl );
	l = lint;
	propertymanagerGetLint( gcode, "height", lint, defh );
	h = lint;

/* debug
	cout << "code: " << gcode
		 << "/ w: " << w
		 << "/ l: " << l
		 << "/ h: " << h << endl;
*/
}





   /*=====================================================================*/
   /*                           PRIVATE METHODS                           */
   /*=====================================================================*/
const string
PropertyManager::propertymanagerGetStr(
	const OPENCITY_GRAPHIC_CODE & gcode,
	const string & key,
	const string & def)
{
	return "";
}


   /*=====================================================================*/
const OPENCITY_ERR_CODE
PropertyManager::propertymanagerGetLint(
	const OPENCITY_GRAPHIC_CODE & gcode,
	const string & key,
	OC_LINT & rlint,
	const OC_LINT & def)
{
// Avoid developers' mistakes
	assert( (uint)gcode < this->vpConf.size() );
	assert( this->vpConf[gcode] != NULL );

	return this->vpConf[gcode]->GetLint( key, rlint, def );
}




























