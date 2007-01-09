/***************************************************************************
						AC3DModel.h  -  description
							-------------------
	begin                : june 29th, 05
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

#ifndef _OPENCITY_AC3DAC3DMODEL_H_
#define _OPENCITY_AC3DAC3DMODEL_H_ 1

#include "ac3dmaterial.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace AC3D {

class AC3DObject;

//========================================================================
/** An independant AC3DModel loader
*/
class AC3DModel{
public:
    AC3DModel(string fileName);

    ~AC3DModel();

	const vector<AC3DMaterial> &
	GetVMaterial() const;

	const AC3DObject* const
	GetPObject() const;

	bool IsGood() const;
	string ToStr() const;		// debug method

private:
	bool bGood;
	string strHeader;			///< "AC3Dx"
	string strFileName;	
	vector<AC3DMaterial> vMaterial;
	AC3DObject* pObject;		///< The "world" object

	bool ac3dmodelParse();
};

}

#endif






























