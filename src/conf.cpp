/***************************************************************************
						conf.cpp  -  description
							-------------------
	begin                : august 1st, 2004
	copyright            : (C) 2004-2007 by Duong-Khang NGUYEN
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

#include "conf.h"

#include <fstream>		// for file IO
#include <cerrno>
#include <cctype>		// for isspace()
#include <cstring>		// for C string functions


   /*=====================================================================*/
const OPENCITY_ERR_CODE
Conf::Open(
	const string& fname )
{
	std::ifstream inFile( fname.c_str() );

// is the file opened ?
	if ( inFile == NULL ) {
		OPENCITY_DEBUG( "WARNING: File open error, see below: " );
		OPENCITY_DEBUG( fname.c_str() );
		return OC_ERR_FILE;
	}

	OC_CHAR strTemp[ OC_MAX_CONF_LINE ];
	OC_CHAR* strNew;
	OC_CHAR* strFirst;
	OC_CHAR* strSecond;
	OC_CHAR* strEmpty = "";

// read the first line
	inFile.getline( strTemp, OC_MAX_CONF_LINE );
	if ( !inFile.good() ) {
		inFile.close();
		OPENCITY_DEBUG( "WARNING: File read error" );
		return OC_ERR_FILE;
	}

// now process the file
	while ( inFile.good() ) {
		if ( (strlen(strTemp) != 0)
		  && (strTemp[0] != '#') ) {
			strNew = strTemp;
		// get the first token, it's the name of the parameter
			strFirst = strtok( strNew, "=" );
		// get the second token, it's the value of the parameter
			strSecond = strtok( NULL, "=" );
		// Trim out spaces from the datas if applicable
			(strFirst != NULL) ? strFirst = Conf::RTrim( strFirst ) : strFirst = strEmpty;
			(strSecond != NULL) ? strSecond = Conf::LTrim( strSecond ) : strSecond = strEmpty;

		// Add the pair to the _mapData if the key is not empty
			if (strlen(strFirst) > 0)
				_mapData[ strFirst ] = strSecond;

//debug cout << "StrFirst/StrSecond: " << strFirst << "/" << strSecond << endl;
//debug
/*
__gnu_cxx::hash<const char *> H;
cout << "'" << (string) strFirst << "' = '" << (string) strSecond << "'" << endl;
cout << "hash : " << H(strFirst) << " / " << H(strSecond) << endl;
cout << (long) strFirst << "=" << (long) strSecond << endl;
cout << "new" << (long) strNew << endl;
			delete strNew;
*/
		}
	// read the next line
		inFile.getline( strTemp, OC_MAX_CONF_LINE );
	}

	if (inFile.eof()) {
		inFile.close();
		return OC_ERR_FREE;
	}
	else {
		inFile.close();
		OPENCITY_DEBUG("FATAL: out of buffer ?");
		assert( 0 );
		return OC_ERR_SOMETHING;
	}
}


   /*=====================================================================*/
void
Conf::Close()
{
//debug
/*
__gnu_cxx::hash_map<string, string, myHash>::iterator iter;
for ( iter = _mapData.begin(); iter != _mapData.end(); iter++ ) {
	cout << "first '" << iter->first << "', second '" << iter->second << "'" << endl;
}
*/
	_mapData.clear();
}


   /*=====================================================================*/
const string &
Conf::GetValue(
	const string& key,
	const string& def )
{
//debug
/*cout << "key is : '" << key << "', data is : '" << _mapData[ key ] << "'" << endl;
	__gnu_cxx::hash_map<string, string, myHash>::iterator
		iter = _mapData.find( key );
	return iter != _mapData.end() ? iter->second : "";
*/

// IF the key is not in the hash_map THEN return the default value
	if (_mapData.find( key ) == _mapData.end())
		return def;
	else
		return _mapData[ key ];
}


   /*=====================================================================*/
const OPENCITY_ERR_CODE
Conf::GetBool(
	const string & key,
	bool & rbool,
	const bool def )
{
// IF the key is not in the hash_map THEN return the default value
	if (_mapData.find( key ) == _mapData.end()) {
		rbool = def;
		return OC_ERR_FREE;
	}

	if (_mapData[key] == "") {
		return OC_ERR_INVALID;
	}

	if ((strcasecmp(_mapData[key].c_str(), "no") == 0)
	 || (strcasecmp(_mapData[key].c_str(), "n") == 0)
	 || (strcasecmp(_mapData[key].c_str(), "false") == 0)
	 || (strcasecmp(_mapData[key].c_str(), "off") == 0)
	 || (strcasecmp(_mapData[key].c_str(), "0") == 0)) {
		rbool = false;
	}
	else {
		rbool = true;
	}

	return OC_ERR_FREE;
}


   /*=====================================================================*/
const OPENCITY_ERR_CODE
Conf::GetLint(
	const string & key,
	OC_LINT & rlint,
	const OC_LINT def )
{
/* debug
for (__gnu_cxx::hash_map<string, string, myHash>::iterator i = _mapData.begin();
i != _mapData.end(); i++) {
	cout << "Map key: " << i->first << "/ value: " << i->second << endl;
}
*/

// IF the key is not in the hash_map THEN return the default value
	if (_mapData.find( key ) == _mapData.end()) {
//debug cout << "key: " << key << "/ default: " << def << endl;
		rlint = def;
		return OC_ERR_FREE;
	}

	rlint = strtol(_mapData[key].c_str(), NULL, 0);

//debug cout << __PRETTY_FUNCTION__ << "read: " << (long int)rlint << endl;

/* FIXME: better check
	int myerr = errno;
	if (myerr != 0) {
//debug cout << "Errno: " << myerr << "/ Str: " << strerror(myerr) << endl;
		assert(0);
		return OC_ERR_INVALID;
	}
	else {
		return OC_ERR_FREE;
	}
*/

	return OC_ERR_FREE;
}


   /*=====================================================================*/
const OPENCITY_ERR_CODE
Conf::GetFloat(
	const string& key,
	float& rfloat,
	const float def )
{
// IF the key is not in the hash_map THEN return the default value
	if (_mapData.find( key ) == _mapData.end()) {
		rfloat = def;
		return OC_ERR_FREE;
	}

	rfloat = strtof(_mapData[key].c_str(), NULL);

	return OC_ERR_FREE;
}


   /*=====================================================================*/
   /*                             STATIC     METHOD                       */
   /*=====================================================================*/
OC_CHAR* const
Conf::RTrim( OC_CHAR* const str )
{
	OC_CHAR* strSpace = NULL;

	if (str != NULL) {
		strSpace = str + strlen( str ) - 1;
		while ((strSpace >= str) && (isspace(*strSpace) != 0))
			*strSpace-- = '\0';
	}

	return str;
}


   /*=====================================================================*/
OC_CHAR* const
Conf::LTrim( OC_CHAR* const str )
{
	OC_CHAR* strSpace = NULL;
	OC_CHAR* strEnd = NULL;

	if (str != NULL) {
		strSpace = str;
		strEnd = str;
		strEnd = strEnd + strlen( str );
		while ((strSpace < strEnd) && (isspace(*strSpace) != 0))
			*strSpace++ = '\0';
	}

	return strSpace;
}













