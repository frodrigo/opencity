/***************************************************************************
						propertymanager.cpp  -  description
							-------------------
	begin                : feb 6th, 2004
	copyright            : (C) 2004-2010 by Duong Khang NGUYEN
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
#include "opencity_structure_type.h"

// OpenCity headers
#include "propertymanager.h"
#include "structure.h"

// Standard headers
#include <algorithm>		// We use STL algorithms
#include <sstream>			// We use stringstream or data conversion


   /*=====================================================================*/
PropertyManager::PropertyManager():
_vpConf(OC_GRAPHIC_CODE_MAX, NULL)
{
	OPENCITY_DEBUG( "ctor" );

	string strAc = string(".ac");
	string strConf = string(".conf");

// Read the main graphism configuration file
	Conf* pConf = new Conf();
	string fileName = ocConfigDirPrefix(OC_GRAPHISM_FILE_FILENAME);
	if (pConf->Open( fileName ) != OC_ERR_FREE) {
		cerr << "WARNING: Error opening graphism config file: " << fileName << endl;
		delete pConf;
		pConf = NULL;
		abort();
	}

// Initialize the Conf* vector
	string strValue;
	string::size_type pos;
	std::stringstream ss;
	Conf* pconfFile;
	for (uint i = 0; i < OC_GRAPHIC_CODE_MAX; i++ ) {
		ss << i;
		strValue = pConf->GetValue(ss.str(), "");
		ss.str("");

	// IF the key is not defined in the config file THEN
		if (strValue == "")
			continue;

	// Reverse search for the ".ac" file extension
		pos = strValue.rfind( strAc );
		if (pos != strValue.npos ) {
		// Replace the file extension ".ac" by ".conf"
			strValue.replace( pos, strConf.size(), strConf );
		}

		pconfFile = new Conf();
		fileName = ocDataDirPrefix(strValue);
		if (pconfFile->Open( fileName ) == OC_ERR_FREE) {
			_vpConf[i] = pconfFile;
		}
		else {
			delete pconfFile;
			pconfFile = NULL;
		}
	} // for

	pConf->Close();
	delete pConf;
	pConf = NULL;
}


   /*=====================================================================*/
PropertyManager::~PropertyManager()
{
	OPENCITY_DEBUG( "dtor" );

// delete the Conf* vector
	for (int i = 0; i < OC_GRAPHIC_CODE_MAX; i++ ) {
		if (_vpConf[i] != NULL) {
			_vpConf[i]->Close();		// Close the file
			delete _vpConf[i];			// NOTE: delete NULL pointer has no effect
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
		case OC_TOOL_COST:
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
				case OC_STRUCTURE_EPLANT_NUCLEAR:
					value = 5000; break;
				case OC_STRUCTURE_FIREDEPT:
				case OC_STRUCTURE_POLICEDEPT:
				case OC_STRUCTURE_HOSPITALDEPT:
				case OC_STRUCTURE_EDUCATIONDEPT:
					value = 1500; break;
				case OC_STRUCTURE_ROAD:
				case OC_STRUCTURE_FLORA:
					value = 5; break;

				case OC_STRUCTURE_TEST:			// Development test feature
					value = 0; break;
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
				case OC_STRUCTURE_FLORA:
					value = 4; break;

				case OC_STRUCTURE_PART:
				case OC_STRUCTURE_EPLANT_COAL:
				case OC_STRUCTURE_FIREDEPT:
				case OC_STRUCTURE_POLICEDEPT:
				case OC_STRUCTURE_HOSPITALDEPT:
				case OC_STRUCTURE_EDUCATIONDEPT:
					value = 200; break;

				case OC_STRUCTURE_EPLANT_NUCLEAR:
					value = 800; break;

				case OC_STRUCTURE_TEST:			// Development test feature
					value = 0; break;
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
				case OC_STRUCTURE_EPLANT_NUCLEAR:
					value = 80; break;
				case OC_STRUCTURE_FIREDEPT:
				case OC_STRUCTURE_POLICEDEPT:
				case OC_STRUCTURE_HOSPITALDEPT:
				case OC_STRUCTURE_EDUCATIONDEPT:
					value = 15; break;
				case OC_STRUCTURE_ROAD:
				case OC_STRUCTURE_FLORA:
					value = 1; break;

				case OC_STRUCTURE_TEST:			// Development test feature
					value = 0; break;
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
		case OC_STRUCTURE_FLORA:
			gcode = OC_TREE_BEGIN;
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
			gcode = OC_EPLANT_COAL;
			break;
		case OC_STRUCTURE_EPLANT_NUCLEAR:
			gcode = OC_EPLANT_NUCLEAR;
			break;
		case OC_STRUCTURE_FIREDEPT:
			gcode = OC_FIRE_DEPT;
			break;
		case OC_STRUCTURE_POLICEDEPT:
			gcode = OC_POLICE_DEPT;
			break;
		case OC_STRUCTURE_HOSPITALDEPT:
			gcode = OC_HOSPITAL_DEPT;
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
			tcode = OC_TYPE_RESIDENCE; break;
		case OC_STRUCTURE_COM:
			tcode = OC_TYPE_COMMERCE; break;
		case OC_STRUCTURE_IND:
			tcode = OC_TYPE_INDUSTRY; break;

		case OC_STRUCTURE_EPLANT_COAL:
		case OC_STRUCTURE_EPLANT_NUCLEAR:
			tcode = OC_TYPE_ELECTRICITY; break;

		case OC_STRUCTURE_ROAD:
		case OC_STRUCTURE_ELINE:
			tcode = OC_TYPE_PATH; break;

		case OC_STRUCTURE_FIREDEPT:
		case OC_STRUCTURE_POLICEDEPT:
		case OC_STRUCTURE_HOSPITALDEPT:
		case OC_STRUCTURE_MILITARYDEPT:
		case OC_STRUCTURE_EDUCATIONDEPT:
			tcode = OC_TYPE_GOVERNMENT; break;

		case OC_STRUCTURE_FLORA:
			tcode = OC_TYPE_TREE; break;

		case OC_STRUCTURE_PART:
			tcode = OC_TYPE_UNDEFINED; break;

	// TODO
		case OC_STRUCTURE_UNDEFINED:
		case OC_STRUCTURE_ANY:
		case OC_STRUCTURE_ELECTRIC:
			tcode = OC_TYPE_UNDEFINED; assert( 0 ); break;

		case OC_STRUCTURE_TEST:			// Development test feature
			tcode = OC_TYPE_UNDEFINED; break;
		default:
			tcode = OC_TYPE_UNDEFINED;
			OPENCITY_DEBUG( "WARNING: game design error" );
			assert( 0 );
	}

	return tcode;
}


   /*=====================================================================*/
void
PropertyManager::GetWLH(
	const OPENCITY_GRAPHIC_CODE gcode,
	uint& w, const uint defw,
	uint& l, const uint defl,
	uint& h, const uint defh)
{
	static OC_LINT width, length, height;

	propertymanagerGetLint( gcode, "width", width, defw );
	w = (uint)width;
	propertymanagerGetLint( gcode, "length", length, defl );
	l = (uint)length;
	propertymanagerGetLint( gcode, "height", height, defh );
	h = (uint)height;

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
	const OPENCITY_GRAPHIC_CODE gcode,
	const string& key,
	const string& defaultValue)
{
	return "";
}


   /*=====================================================================*/
const OPENCITY_ERR_CODE
PropertyManager::propertymanagerGetLint(
	const OPENCITY_GRAPHIC_CODE gcode,
	const string& key,
	OC_LINT& rlint,
	const OC_LINT defaultValue)
{
// Preconditions
	assert( (uint)gcode < _vpConf.size() );

// Get the configuration pointer.
	Conf* pConf = _vpConf[gcode];
	assert( pConf != NULL );

	return pConf->GetLint( key, rlint, defaultValue );
}
