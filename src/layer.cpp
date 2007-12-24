/***************************************************************************
						layer.cpp  -  description
							-------------------
	begin                : september 20th, 2003
	copyright            : (C) 2003-2007 by Duong-Khang NGUYEN
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

// Useful enumerations
#include "opencity_structure_type.h"

// OpenCity headers
#include "layer.h"
#include "structure.h"
#include "guicontainer.h"	// for the "Query" function
#include "guibutton.h"

// Static variables
uint Layer::uiNumberLayer = 0;
GUIButton* Layer::pbtnQW;
GUIButton* Layer::pbtnQE;
GUIButton* Layer::pbtnQG;
GUIButton* Layer::pbtnQR;
GUIButton* Layer::pbtnQC;
GUIButton* Layer::pbtnQI;


Layer::Layer(){
	OPENCITY_DEBUG( "ctor" );

// If called first time, create all the static variables
// we do it here, because we need an initialized OpenGL context
// if we do it on the declaration line, there will be runtime errors !
	if ( Layer::uiNumberLayer++ == 0) {
		Layer::pbtnQW = new GUIButton( 19, 33, 30, 30, ocDataDirPrefix( "graphism/gui/query_water" ));
		Layer::pbtnQE = new GUIButton( 55, 13, 30, 30, ocDataDirPrefix( "graphism/gui/query_electricity" ));
		Layer::pbtnQG = new GUIButton( 91, 33, 30, 30, ocDataDirPrefix( "graphism/gui/query_gas" ));
		Layer::pbtnQR = new GUIButton( 19, 73, 30, 30, ocDataDirPrefix( "graphism/gui/query_residential" ));
		Layer::pbtnQC = new GUIButton( 55, 93, 30, 30, ocDataDirPrefix( "graphism/gui/query_commercial" ));
		Layer::pbtnQI = new GUIButton( 91, 73, 30, 30, ocDataDirPrefix( "graphism/gui/query_industrial" ));

	// Disable the default alpha blending behaviour
		Layer::pbtnQW->Unset( OC_GUIMAIN_BLENDING );
		Layer::pbtnQE->Unset( OC_GUIMAIN_BLENDING );
		Layer::pbtnQG->Unset( OC_GUIMAIN_BLENDING );
		Layer::pbtnQR->Unset( OC_GUIMAIN_BLENDING );
		Layer::pbtnQC->Unset( OC_GUIMAIN_BLENDING );
		Layer::pbtnQI->Unset( OC_GUIMAIN_BLENDING );
	}
}


   /*=====================================================================*/
Layer::~Layer(){
	OPENCITY_DEBUG( "dtor" );

// called for the last derived class ?
// yes, then destroy all the static stuff
	if ( Layer::uiNumberLayer-- == 1) {
		delete Layer::pbtnQW;
		delete Layer::pbtnQE;
		delete Layer::pbtnQG;
		delete Layer::pbtnQR;
		delete Layer::pbtnQC;
		delete Layer::pbtnQI;
	}
}


   /*=====================================================================*/
const uint
Layer::GetMaxLinear() const
{
	return _uiLayerWidth * _uiLayerLength - 1;
}


   /*=====================================================================*/
void
Layer::GetLayerSize(
	uint & w,
	uint & l) const
{
	w = _uiLayerWidth;
	l = _uiLayerLength;
}






























