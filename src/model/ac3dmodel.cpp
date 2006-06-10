/***************************************************************************
                          AC3DModel.cpp  -  description
          $Id: ac3dmodel.cpp,v 1.5 2006/03/12 23:14:18 neoneurone Exp $
                             -------------------
    begin                : mer juin 29 05
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

#include "ac3dmodel.h"
#include "ac3dobject.h"

#include "macros.h"			// For debugging macros
#include "ac3dmacros.h"		// Our macros

#include <fstream>
#include <cstring>
#include <sstream>

using std::ifstream;
using std::stringstream;

namespace AC3D {

   /*=====================================================================*/
AC3DModel::AC3DModel
(
	string fileName
):
strFileName(fileName),
pObject(NULL)
{
	OPENCITY_DEBUG("ctor");
	this->bGood = this->ac3dmodelParse();
}


   /*=====================================================================*/
AC3DModel::~AC3DModel()
{
	OPENCITY_DEBUG("dtor");
	
	delete this->pObject;
	this->pObject = NULL;
	this->bGood = false;
}


   /*=====================================================================*/
const vector<AC3DMaterial> &
AC3DModel::GetVMaterial() const
{
	return this->vMaterial;
}


   /*=====================================================================*/
const AC3DObject* const
AC3DModel::GetPObject() const
{
	return this->pObject;
}


   /*=====================================================================*/
bool
AC3DModel::IsGood() const
{
	return this->bGood;
}


   /*=====================================================================*/
string
AC3DModel::ToStr() const
{
	string str = this->strHeader + "\n";
	vector<AC3DMaterial>::size_type pos, size;

// Output all materials
	size = this->vMaterial.size();
	for (pos = 0; pos < size; pos++)
		str += this->vMaterial[pos].ToStr();

// Output all objects
	if (this->pObject != NULL)
		str += this->pObject->ToStr();

	return str;	
}


   /*=====================================================================*/
   /*                        PRIVATE      METHODS                         */
   /*=====================================================================*/
bool
AC3DModel::ac3dmodelParse()
{
	ifstream in;
	stringstream ss;
	
	char line[AC3D_MAX_LINE_LENGTH];
	bool endBlock;

	AC3DObject* pNewObj = NULL;
	AC3DObject* pCurrentObj = NULL;
	vector<AC3DObject*> vpObj;				// Stack used for recursive object loading


	in.open(this->strFileName.c_str());

	if (!in.good()) {
		OPENCITY_DEBUG("File open error");
		OPENCITY_DEBUG( this->strFileName.c_str() );
		return false;
	}

// Get the header. Ex: "AC3Db"
// TODO: version check
	in.getline( line, AC3D_MAX_LINE_LENGTH );
	if (in.good())
		strHeader = line;

// Parse the file
	in.getline( line, AC3D_MAX_LINE_LENGTH );
	while (in.good()) {
	// Looks for "MATERIAL" token
		if ( strncmp( line, AC3D_TOKEN_MATERIAL, AC3D_TOKEN_MATERIAL_L ) == 0 ) {
		// Create new MATERIAL
			this->vMaterial.push_back( AC3DMaterial( line ) );
			in.getline( line, AC3D_MAX_LINE_LENGTH );
		} else
		if( strncmp( line, AC3D_TOKEN_OBJECT, AC3D_TOKEN_OBJECT_L ) == 0 ) {
			ss.str("");
			//debug cout << "New line: " << line << endl;
			ss << line << endl;

		// Get all the OBJECT block
			in.getline( line, AC3D_MAX_LINE_LENGTH );
			endBlock = false;
			while (in.good() && !endBlock) {
			// IF this is a new OBJECT command then we reach the end of the block
				if( strncmp( line, AC3D_TOKEN_OBJECT, AC3D_TOKEN_OBJECT_L ) == 0 ) {
					endBlock = true;
				}
				else {
					//debug cout << "New line: " << line << endl;
					ss << line << endl;
					in.getline( line, AC3D_MAX_LINE_LENGTH );
				}
			} // while

		// Create new OBJECT from the block of data
			pNewObj = new AC3DObject( ss );

		// IF this is the first object THEN
			if (this->pObject == NULL) {
				this->pObject = pNewObj;
				pCurrentObj = pNewObj;
			} 
			else {
				assert( pCurrentObj != NULL );
				pCurrentObj->AddKid( pNewObj );

			// IF this object has any kids THEN
				if (pNewObj->GetNumberKid() > 0) {
				// Save the current level on the stack
					vpObj.push_back( pCurrentObj );
					pCurrentObj = pNewObj;
				}
				else {
				// IF all the kids have been loaded THEN
					while ((pCurrentObj->GetVPObject().size() == pCurrentObj->GetNumberKid())
						&& (vpObj.empty() == false)) {
					// Restore the previous level
						pCurrentObj = vpObj.back();
						vpObj.pop_back();
					}
				}
			} // IF first object

		} // new OBJECT
	} // while good
	
	if (in.is_open())
		in.close();

	return true;			// everything is OK
}


}

























