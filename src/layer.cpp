/***************************************************************************
                          layer.cpp  -  description
          $Id$
                             -------------------
    begin                : september 20th, 2003
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
//GUIButton* Layer::pbtnQP;


Layer::Layer(){
	OPENCITY_DEBUG( "ctor" );

// If called first time, create all the static variables
// we do it here, because we need an initialized OpenGL context
// if we do it on the declaration line, there will be runtime errors !
	if ( Layer::uiNumberLayer++ == 0) {
		Layer::pbtnQW = new GUIButton( 20,  20,  30, 30, ocHomeDirPrefix( "graphism/gui/query_water" ));
		Layer::pbtnQE = new GUIButton( 60,  0,   30, 30, ocHomeDirPrefix( "graphism/gui/query_electricity" ));
		Layer::pbtnQG = new GUIButton( 100, 20,  30, 30, ocHomeDirPrefix( "graphism/gui/query_gas" ));
		Layer::pbtnQR = new GUIButton( 20,  80,  30, 30, ocHomeDirPrefix( "graphism/gui/query_residential" ));
		Layer::pbtnQC = new GUIButton( 60,  100, 30, 30, ocHomeDirPrefix( "graphism/gui/query_commercial" ));
		Layer::pbtnQI = new GUIButton( 100, 80,  30, 30, ocHomeDirPrefix( "graphism/gui/query_industrial" ));

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
































