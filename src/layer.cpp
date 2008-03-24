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
uint Layer::_uiNumberLayer = 0;
GUIContainer* Layer::pctrQ;								///< The query container
GUIButton* Layer::pbtnQRo, * Layer::pbtnQRf;			///< Residential query button
GUIButton* Layer::pbtnQCo, * Layer::pbtnQCf;			///< Commercial query button
GUIButton* Layer::pbtnQIo, * Layer::pbtnQIf;			///< Industrial query button
GUIButton* Layer::pbtnQWo, * Layer::pbtnQWf;			///< Water query button
GUIButton* Layer::pbtnQEo, * Layer::pbtnQEf;			///< Electricity query button
GUIButton* Layer::pbtnQGo, * Layer::pbtnQGf;			///< Gas query button


Layer::Layer(){
	OPENCITY_DEBUG( "ctor" );

// IF called first time THEN create all the static variables
// we do it here, because we need an initialized OpenGL context
// if we do it on the declaration line, there will be runtime errors !
	if ( Layer::_uiNumberLayer++ > 0)
		return;

	_CreateQueryContainer();
}


   /*=====================================================================*/
Layer::~Layer(){
	OPENCITY_DEBUG( "dtor" );

// IF not called for the last derived class THEN return
	if ( Layer::_uiNumberLayer-- > 1)
		return;

	_DeleteQueryContainer();
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


   /*=====================================================================*/
void
Layer::_CreateQueryContainer()
{
// On buttons
	Layer::pbtnQRo =
		new GUIButton( GUIBUTTON_POSITION_1, ocDataDirPrefix("graphism/gui/query_residential_on"), 1);
	Layer::pbtnQWo =
		new GUIButton( GUIBUTTON_POSITION_2, ocDataDirPrefix("graphism/gui/query_water_on"), 1);
	Layer::pbtnQEo =
		new GUIButton( GUIBUTTON_POSITION_3, ocDataDirPrefix("graphism/gui/query_electricity_on"), 1);
	Layer::pbtnQGo =
		new GUIButton( GUIBUTTON_POSITION_4, ocDataDirPrefix("graphism/gui/query_gas_on"), 1);
	Layer::pbtnQIo =
		new GUIButton( GUIBUTTON_POSITION_5, ocDataDirPrefix("graphism/gui/query_industrial_on"), 1);
	Layer::pbtnQCo =
		new GUIButton( GUIBUTTON_POSITION_6, ocDataDirPrefix("graphism/gui/query_commercial_on"), 1);

// Off buttons
	Layer::pbtnQRf =
		new GUIButton( GUIBUTTON_POSITION_1, ocDataDirPrefix("graphism/gui/query_residential_off"), 1);
	Layer::pbtnQWf =
		new GUIButton( GUIBUTTON_POSITION_2, ocDataDirPrefix("graphism/gui/query_water_off"), 1);
	Layer::pbtnQEf =
		new GUIButton( GUIBUTTON_POSITION_3, ocDataDirPrefix("graphism/gui/query_electricity_off"), 1);
	Layer::pbtnQGf =
		new GUIButton( GUIBUTTON_POSITION_4, ocDataDirPrefix("graphism/gui/query_gas_off"), 1);
	Layer::pbtnQIf =
		new GUIButton( GUIBUTTON_POSITION_5, ocDataDirPrefix("graphism/gui/query_industrial_off"), 1);
	Layer::pbtnQCf =
		new GUIButton( GUIBUTTON_POSITION_6, ocDataDirPrefix("graphism/gui/query_commercial_off"), 1);

// Query container
	Layer::pctrQ =
		new GUIContainer( 100, 100, 140, 140, ocDataDirPrefix( "graphism/gui/querycircle_bg.png" ) );

// Add all the controls with their ON state
	Layer::pctrQ->Add( Layer::pbtnQRo );
	Layer::pctrQ->Add( Layer::pbtnQWo );
	Layer::pctrQ->Add( Layer::pbtnQEo );
	Layer::pctrQ->Add( Layer::pbtnQGo );
	Layer::pctrQ->Add( Layer::pbtnQIo );
	Layer::pctrQ->Add( Layer::pbtnQCo );

// Add all the controls with their OFF state
	Layer::pctrQ->Add( Layer::pbtnQRf );
	Layer::pctrQ->Add( Layer::pbtnQWf );
	Layer::pctrQ->Add( Layer::pbtnQEf );
	Layer::pctrQ->Add( Layer::pbtnQGf );
	Layer::pctrQ->Add( Layer::pbtnQIf );
	Layer::pctrQ->Add( Layer::pbtnQCf );

// Disable unused query icons
	Layer::pbtnQWo->Unset( OC_GUIMAIN_VISIBLE );
	Layer::pbtnQGo->Unset( OC_GUIMAIN_VISIBLE );
}


   /*=====================================================================*/
void
Layer::_DeleteQueryContainer()
{
// Destroy all the static stuff when called by the last derived class
	delete Layer::pctrQ;

	delete Layer::pbtnQCf;
	delete Layer::pbtnQIf;
	delete Layer::pbtnQGf;
	delete Layer::pbtnQEf;
	delete Layer::pbtnQWf;
	delete Layer::pbtnQRf;

	delete Layer::pbtnQCo;
	delete Layer::pbtnQIo;
	delete Layer::pbtnQGo;
	delete Layer::pbtnQEo;
	delete Layer::pbtnQWo;
	delete Layer::pbtnQRo;
}


























