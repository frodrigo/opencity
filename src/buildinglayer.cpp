/***************************************************************************
                          buildinglayer.cpp  -  description
      $Id: buildinglayer.cpp,v 1.39 2006/06/05 09:58:06 neoneurone Exp $
                             -------------------
    begin                : sam sep 20 2003
    copyright            : (C) 2003-2005 by Duong-Khang NGUYEN
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

#include "buildinglayer.h"

#include "city.h"
#include "map.h"

#include "rcistructure.h"
#include "wegstructure.h"
#include "pathstructure.h"

#include "guicontainer.h"
#include "guibutton.h"

#include "propertymanager.h"


extern PropertyManager* gpPropertyMgr;	// global property manager
extern Map* gpMapMgr;					// global map height manager


   /*=====================================================================*/
BuildingLayer::BuildingLayer
(
	const City & rcCity
)
{
	OPENCITY_DEBUG( "ctor" );
// initialization of width and height
	_uiLayerWidth = rcCity.cityGetWidth();
	_uiLayerHeight = rcCity.cityGetHeight();

// initialization of the table of pointer of Structure
	uint citySurface = _uiLayerHeight * _uiLayerWidth;
	_tabpStructure = new Structure*[citySurface];
	for ( uint counter = 0; counter < citySurface; counter++ ) {
		_tabpStructure[ counter ] = NULL;
	}

}


   /*=====================================================================*/
BuildingLayer::~BuildingLayer()
{
	OPENCITY_DEBUG("dtor");

	uint citySurface = _uiLayerHeight * _uiLayerWidth;
	for ( uint counter = 0; counter < citySurface; counter++ ) {
	// delete NULL pointer is allowed ?
	// delete seems to have no effects on NULL
		if (_tabpStructure[counter] != NULL)
			delete _tabpStructure[counter];
	}

	delete [] _tabpStructure;
}


   /*=====================================================================*/
/* TOKILL, reference, old version
void
BuildingLayer::SaveTo( std::fstream& rfs )
{
	OPENCITY_DEBUG( __PRETTY_FUNCTION__ << "saving" );
	Structure* p = NULL, * m = NULL;
	bool found = false;
	uint dw1, dl1, dw2, dl2;
	uint tempW, tempL;
	uint sw, sl, sh;				// Structure's width, length and height

	rfs << _uiLayerWidth << std::endl;
	rfs << _uiLayerHeight << std::endl;

	uint citySurface = _uiLayerHeight * _uiLayerWidth;
	for ( uint linear = 0; linear < citySurface; linear++ ) {
		p = _tabpStructure[linear];
		if (p != NULL) {
		// Save the main structure coordinates
			m = p->GetMain();
			found = false;
			if (m != NULL) {
				dw1 = dw2 = linear % _uiLayerWidth;
				dl1 = dl2 = linear / _uiLayerWidth;
				assert ( dw1 == dw2 );
				assert ( dl1 == dl2 );

			// Get the width, length and height of the main structure
			// WARNING: invalid default values given.
				gpPropertyMgr->GetWLH( m->GetGraphicCode(), sw, 0, sl, 0, sh, 0 );
				assert( (sw != 0) && (sl != 0) );
				sw--; sl--;			// We calculate the range
	
			// Calculate the possible area to limit cpu usage
				gpMapMgr->GetPossibleWH( dw1, dl1, -sw, -sl );
				gpMapMgr->GetPossibleWH( dw2, dl2,  sw,  sl );

			// Now search for all STRUCTURE_PART and the main structure, destroy em
				tempL = dl1;
				while ( tempL <= dl2 ) {
					tempW = dw1;
					while ( tempW <= dw2 ) {
						if ( GetStructure(tempW, tempL) == m ) {
							found = true;
							goto save_structure;
						}
						tempW++;
					}
					tempL++;
				}
			} // if (m != NULL)
		}

	save_structure:
		rfs << p << std::endl;
		if (p != NULL) {
			rfs << p->GetType() << std::endl;
			p->SaveTo( rfs );
			rfs << m << std::endl;
			if (found) {
				rfs << tempW << std::endl;
				rfs << tempL << std::endl;
				assert( found and m != NULL );
			}
		}
	} // for
}
*/


   /*=====================================================================*/
/* TOKILL, reference, old version
void
BuildingLayer::LoadFrom( std::fstream& rfs )
{
	OPENCITY_DEBUG( __PRETTY_FUNCTION__ << "loading" );
	Structure* p = NULL;
	void* t = NULL;
	uint w, l;
	uint type;		///< Structure's object type

// Remove the old structures
	uint citySurface = _uiLayerHeight * _uiLayerWidth;
	for ( uint linear = 0; linear < citySurface; linear++ ) {
		delete _tabpStructure[linear];
	}
	delete [] _tabpStructure;

// Reset the counter
	Structure::SetNumber( 0 );

// Read the saved size
	rfs >> _uiLayerWidth; rfs.ignore();
	rfs >> _uiLayerHeight; rfs.ignore();

// initialization of the table of pointer of Structure
	citySurface = _uiLayerHeight * _uiLayerWidth;
	_tabpStructure = new Structure*[citySurface];

	for ( uint linear = 0; linear < citySurface; linear++ ) {
	// Load the structure
		rfs >> t; rfs.ignore();
		p = NULL;
		if (t != NULL) {
			rfs >> type; rfs.ignore();
			switch (type) {
				case OC_STRUCTURE_R:
				case OC_STRUCTURE_C:
				case OC_STRUCTURE_I:
					p = new RCIStructure();
					break;

				case OC_STRUCTURE_W:
				case OC_STRUCTURE_E:
				case OC_STRUCTURE_G:
					p = new WEGStructure();
					break;

				case OC_STRUCTURE_P:
					p = new PathStructure();
					break;

				default:
					OPENCITY_DEBUG( "Unknown structure's type: " << type );
					assert( 0 );
			}
			p->LoadFrom( rfs );

		// Get the main structure information eventually
			rfs >> t; rfs.ignore();
			if (t != NULL) {
				rfs >> w; rfs.ignore();
				rfs >> l; rfs.ignore();
			// WARNING: this is not safe. It supposes 
			// that the main structure is created before its parts
				if ( GetStructure(w,l) != NULL ) {
					OPENCITY_DEBUG( "linear: " << linear << " /w: " << w << " /l: " << l );
					assert( 0 );
				}
				p->SetMain(GetStructure(w,l));
			}
		}
		_tabpStructure[linear] = p;
	}
}
*/


   /*=====================================================================*/
void
BuildingLayer::SaveTo( std::fstream& rfs )
{
	OPENCITY_DEBUG( __PRETTY_FUNCTION__ << "saving" );
	Structure* p = NULL;

	rfs << _uiLayerWidth << std::endl;
	rfs << _uiLayerHeight << std::endl;

	uint citySurface = _uiLayerHeight * _uiLayerWidth;
	for ( uint linear = 0; linear < citySurface; linear++ ) {
		p = _tabpStructure[linear];

	// We save only extra information for main structures
		if ((p != NULL) && (p->GetMain() != NULL)) {
			p = NULL;
		}
		rfs << p << std::endl;

		if (p != NULL) {
			rfs << p->GetType() << std::endl;
			p->SaveTo( rfs );
		}
	} // for
}


   /*=====================================================================*/
void
BuildingLayer::LoadFrom( std::fstream& rfs )
{
	OPENCITY_DEBUG( __PRETTY_FUNCTION__ << "loading" );
	Structure* p = NULL;
	void* t = NULL;
	uint w, l, rCost, anUint, linear;
	OPENCITY_STRUCTURE_TYPE type;		///< Structure's object type
	OPENCITY_ERR_CODE errCode;

// Remove the old structures
	uint citySurface = _uiLayerHeight * _uiLayerWidth;
	for ( linear = 0; linear < citySurface; linear++ ) {
		delete _tabpStructure[linear];
	}
	delete [] _tabpStructure;

// Reset the counter
	Structure::SetNumber( 0 );

// Read the saved size
	rfs >> _uiLayerWidth; rfs.ignore();
	rfs >> _uiLayerHeight; rfs.ignore();

// initialization of the table of pointer of Structure
	citySurface = _uiLayerHeight * _uiLayerWidth;
	_tabpStructure = new Structure*[citySurface];
	for ( linear = 0; linear < citySurface; linear++ ) {
		_tabpStructure[linear] = NULL;
	}

// Load the structures
	for ( l = 0; l < _uiLayerHeight; l++ )
	for ( w = 0; w < _uiLayerWidth; w++ ) {
		rfs >> t; rfs.ignore();
		if (t != NULL) {
			rfs >> anUint; rfs.ignore(); type = (OPENCITY_STRUCTURE_TYPE)anUint;
			switch (type) {
				case OC_STRUCTURE_RESIDENCE:
				case OC_STRUCTURE_COMMERCE:
				case OC_STRUCTURE_INDUSTRY:
					p = new RCIStructure();
					p->LoadFrom( rfs );
					errCode = _BuildRCIStructure( w, l, w, l, p->GetCode(), rCost );
					break;

				case OC_STRUCTURE_WATER:
				case OC_STRUCTURE_ELECTRICITY:
				case OC_STRUCTURE_GAS:
				case OC_STRUCTURE_GOVERNMENT:		// hack
					p = new WEGStructure();
					p->LoadFrom( rfs );
					OPENCITY_DEBUG( "code: " << p->GetCode() );
					errCode = _BuildWEGStructure( w, l, p->GetCode(), rCost );
					break;

				case OC_STRUCTURE_PATH:
					p = new PathStructure();
					p->LoadFrom( rfs );
					errCode = _BuildPathStructure( w, l, w, l, p->GetCode(), rCost );
					break;

				default:
					OPENCITY_DEBUG( "Unknown structure's type: " << type );
					assert( 0 );
			}

			delete p;
		} // if (p != NULL)
	} // for
}


   /*=====================================================================*/
void
BuildingLayer::uiKeyboard( const SDL_KeyboardEvent & rcsSDLKeyboardEvent )
{}

void
BuildingLayer::uiMouseMotion( const SDL_MouseMotionEvent &
				rcsSDLMouseMotionEvent )
{}

void
BuildingLayer::uiMouseButton( const SDL_MouseButtonEvent & 
				rcsSDLMouseButtonEvent )
{}

void
BuildingLayer::uiExpose( const SDL_ExposeEvent & rcsSDLExposeEvent )
{}

void
BuildingLayer::uiResize( const SDL_ResizeEvent & rcsSDLResizeEvent )
{}


   /*=====================================================================*/
const bool
BuildingLayer::IsConstructive(
	uint W1, uint L1,
	uint W2, uint L2,
	const OPENCITY_STRUCTURE_CODE & enumStructCode ) const
{
	uint linearIndex;
	uint w;

// Out of map ?
	if ((L2 >= _uiLayerHeight) || (W2 >= _uiLayerWidth))
		return false;

// cheking for the availability of a free room
	while (L1 <= L2) {
		linearIndex = (L1*_uiLayerWidth) + W1;
		w = W1;
		while (w <= W2) {
			switch (enumStructCode) {
				case OC_STRUCTURE_RES:
				case OC_STRUCTURE_IND:
				case OC_STRUCTURE_COM:
				case OC_STRUCTURE_PARK:
				case OC_STRUCTURE_EPLANT_COAL:
				case OC_STRUCTURE_FIREDEPT:
				case OC_STRUCTURE_POLICEDEPT:
				case OC_STRUCTURE_EDUCATIONDEPT:
				case OC_STRUCTURE_TEST:
			// IF there's a structure on the square
			// OR the square is not plane
			// then the square is not constructive
				if ((this->GetLinearStructure(linearIndex) != NULL)
				  ||(gpMapMgr->IsSquarePlane(w, L1) != true ))
					return false;
				break;

				case OC_STRUCTURE_ROAD:
				case OC_STRUCTURE_ELINE:
			// The case of "road structure"
			// OR "eline" is
			// processed in a seperate method
				if (_IsPathConstructive(w, L1, enumStructCode) != true)
					return false;
				break;

				default:
					OPENCITY_DEBUG( "Unknown structure" );
					assert(0);
				break;

			}  // switch
			w++;
			linearIndex++;
		}  // while w
		L1++;
	}

	return true;
}


   /*=====================================================================*/
const OPENCITY_ERR_CODE
BuildingLayer::BuildPreview(
	const uint & W,
	const uint & L,
	const OPENCITY_STRUCTURE_CODE & enumStructCode,
	OPENCITY_GRAPHIC_CODE & enumGraphicCode ) const
{
//	OPENCITY_DEBUG( "I'm previewing a building" );
	static uint W1, L1, W2, L2;
	static uint sw, sl, sh;			// Structure's width, length and height


// FIXME: Work for WEG structure only !
// Get the graphic code of the structure
	enumGraphicCode = gpPropertyMgr->GetGC( enumStructCode );
	if ( enumGraphicCode == OC_EMPTY ) {
		OPENCITY_DEBUG( "WARNING: not implemented" );
		assert(0);
	}

// Get the corresponding WLH and calculate the range
// An coal electric plant is 4x4 size by default
	gpPropertyMgr->GetWLH( enumGraphicCode, sw, 4, sl, 4, sh, 1 );
	W1 = W;
	L1 = L;
	W2 = W + sw - 1;
	L2 = L + sl - 1;

// Isn't there enough space ?
// OR is there already something on the surface ?
	if (IsConstructive(W1, L1, W2, L2, enumStructCode) == false) {
		return OC_ERR_SOMETHING;
	}

	return OC_ERR_FREE;
}


   /*=====================================================================*/
const OPENCITY_ERR_CODE
BuildingLayer::BuildStructure(
	const uint & W1,
	const uint & L1,
	const uint & W2,
	const uint & L2,
	const OPENCITY_STRUCTURE_CODE & enumStructCode,
	uint& rCost )
{
	OPENCITY_DEBUG( "I'm building some structures" );

// For more obvious coding
	OPENCITY_ERR_CODE errCode;

	switch (enumStructCode) {
		case OC_STRUCTURE_RES:
		case OC_STRUCTURE_COM:
		case OC_STRUCTURE_IND:
		case OC_STRUCTURE_PARK:
		case OC_STRUCTURE_TEST:
			errCode = _BuildRCIStructure( W1, L1, W2, L2, enumStructCode, rCost );
			break;

		case OC_STRUCTURE_EDUCATIONDEPT:
		case OC_STRUCTURE_FIREDEPT:
		case OC_STRUCTURE_POLICEDEPT:
		case OC_STRUCTURE_EPLANT_COAL:
			errCode = _BuildWEGStructure( W1, L1, enumStructCode, rCost );
			break;

		case OC_STRUCTURE_ROAD:
		case OC_STRUCTURE_ELINE:
			errCode = _BuildPathStructure( W1, L1, W2, L2, enumStructCode, rCost );
			break;

		default:
			OPENCITY_DEBUG( "WARNING: Unknown structure to build" );
			errCode = OC_ERR_SOMETHING;
			assert(0);
			break;
	} // switch

	return errCode;
}


   /*=====================================================================*/
const OPENCITY_ERR_CODE
BuildingLayer::ResizeStructure(
	const uint & w,
	const uint & l,
	const OPENCITY_GRAPHIC_CODE & oldGC )
{
	uint linearIndex = (l*_uiLayerWidth) + w;
	Structure* pStruct = NULL, *pTemp = NULL;
	uint ow = 0, ol = 0, oh = 0;
	uint nw = 0, nl = 0, nh = 0;
	uint dw, dl;						// Used in the "for loop"
	uint uiCost;
	OPENCITY_STRUCTURE_CODE structCode;

// Get the pointer to the structure that we need to modify
	pStruct = _tabpStructure[ linearIndex ];
	assert( pStruct != NULL );
	if ( pStruct == NULL ) {
		assert( 0 );
		return OC_ERR_SOMETHING;
	}

// IF the graphic code has not changed THEN it's okay
	if ( oldGC == pStruct->GetGraphicCode() )
		return OC_ERR_FREE;
	structCode = pStruct->GetCode();

// Get the old WLH
	gpPropertyMgr->GetWLH(
		oldGC,
		ow, 0,
		ol, 0,
		oh, 0 );
	assert((ow != 0) && (ol != 0));		// not used yet: && (oh != 0) 

// Get the new WLH
	gpPropertyMgr->GetWLH(
		pStruct->GetGraphicCode(),
		nw, 0,
		nl, 0,
		nh, 0 );
	assert((nw != 0) && (nl != 0));		// not used yet: && (nh != 0) 

// Remove all existing marks on the old surface used by the structure
	for (dl = l; dl < l + ol; dl++) {
		linearIndex = (dl*_uiLayerWidth) + w;
		for (dw = w; dw < w + ow; dw++) {
			pTemp = _tabpStructure[ linearIndex++ ];
		// NOTE: the following "if" is unnecessary, since pTemp is never NULL
			assert( pTemp != NULL );
			if (pTemp != NULL)
				pTemp->Unset( OC_STRUCTURE_MARK );
		}
	}

// Build the eventual missing parts of the new structure
	for (dl = l; dl < l + nl; dl++) {
		linearIndex = (dl*_uiLayerWidth) + w;
		for (dw = w; dw < w + nw; dw++) {
			pTemp = _tabpStructure[ linearIndex ];
			if (pTemp != pStruct) {
			// IF there is something AND it's not the structure we're working on THEN
				if (pTemp != NULL) {
				// IF this structure is not part of the resized structure
				// THEN remove it and build a new part
					if (pTemp->GetMain() != pStruct) {
						_DestroyStructure( dw, dl, uiCost );
						pTemp = new RCIStructure( OC_STRUCTURE_PART, pStruct );
						_tabpStructure[ linearIndex ] = pTemp;
					}
				}
				else {
				// There is nothing here, let's build a new structure's part
					pTemp = new RCIStructure( OC_STRUCTURE_PART, pStruct );
					_tabpStructure[ linearIndex ] = pTemp;
				}
			}
			pTemp->Set( OC_STRUCTURE_MARK );
			linearIndex++;
		}
	}

// Remove the unmarked structures on the old surface 
// then build the first level structures on it
	for (dl = l; dl < l + ol; dl++) {
		linearIndex = (dl*_uiLayerWidth) + w;
		for (dw = w; dw < w + ow; dw++) {
			pTemp = _tabpStructure[ linearIndex ];
// debug
			assert( pTemp != NULL );
			if (pTemp->IsSet(OC_STRUCTURE_MARK) == false) {
				_DestroyStructure( dw, dl, uiCost );
				_tabpStructure[ linearIndex ] = new RCIStructure( structCode );
			}
			linearIndex++;
		} // for dw
	} // for dl

	return OC_ERR_FREE;
}


   /*=====================================================================*/
const OPENCITY_ERR_CODE
BuildingLayer::DestroyStructure(
	uint W1, uint L1,
	uint W2, uint L2,
	uint& rCost )
{
	uint w, l;
	uint costPerSquare;

	OPENCITY_SWAP( W1, W2, uint );
	OPENCITY_SWAP( L1, L2, uint );
	rCost = 0;

	for (w = W1; w <= W2; w++)
		for (l = L1; l <= L2; l++) {
		if (_DestroyStructure( w, l, costPerSquare ) == OC_ERR_FREE)
			rCost += costPerSquare;
	}

	return OC_ERR_FREE;
}


   /*=====================================================================*/
Structure *
BuildingLayer::GetLinearStructure( const uint & rcuiLinearIndex ) const
{
	return _tabpStructure[ rcuiLinearIndex ];
}


   /*=====================================================================*/
Structure*
BuildingLayer::GetStructure(
	const uint & w,
	const uint & h ) const
{
	return _tabpStructure[ h*_uiLayerWidth + w ];
}


   /*=====================================================================*/
Structure*
BuildingLayer::GetRandomStructure(
	uint & w,
	uint & h,
	const OPENCITY_STRUCTURE_CODE & enumStructCode ) const
{
	if (enumStructCode == OC_STRUCTURE_UNDEFINED)
		return NULL;

// nothing can be static because we can be called by more than 1 thread
	uint linear;
	uint counter;
	Structure* pstruct;

// we try 100 times before giving up
// OC_CHANCE_COUNTER_MAX = 100
	counter = 0;
	do {
		linear = rand() % (_uiLayerWidth*_uiLayerHeight);
		pstruct = GetLinearStructure(linear);
		if (pstruct != NULL)
			if ((pstruct->GetCode() == enumStructCode)
			 || (enumStructCode == OC_STRUCTURE_ANY)) {
				h = linear / _uiLayerWidth;
				w = linear % _uiLayerWidth;
				return pstruct;
			}

		counter++;
	} while (counter < OC_CHANCE_COUNTER_MAX);

//debug	cout << "coco: " << linear << endl;
	return NULL;
}


   /*=====================================================================*/
const uint
BuildingLayer::GetNumberStructure() const
{
	cout << "WARNING: not implemented" << endl;
	assert(0);

	return 0;
}


   /*=====================================================================*/
void
BuildingLayer::StructureSet(
	const OC_BYTE & status )
{
	uint citySurface = _uiLayerHeight * _uiLayerWidth;
	for ( uint counter = 0; counter < citySurface; counter++ ) {
		if (_tabpStructure[ counter ] != NULL)
			_tabpStructure[ counter ]->Set( status );
	}
}


   /*=====================================================================*/
void
BuildingLayer::StructureUnset(
	const OC_BYTE & status )
{
	uint citySurface = _uiLayerHeight * _uiLayerWidth;
	for ( uint counter = 0; counter < citySurface; counter++ ) {
		if (_tabpStructure[ counter ] != NULL)
			_tabpStructure[ counter ]->Unset( status );
	}
}


   /*=====================================================================*/
const bool
BuildingLayer::ContainStructure(
	const uint & W1,
	const uint & L1,
	const uint & W2,
	const uint & L2,
	const OPENCITY_STRUCTURE_CODE & enumStructCode ) const
{
	uint w, h, linear;
	Structure* pstruct;

	switch (enumStructCode) {
		case OC_STRUCTURE_UNDEFINED:
			return false;

//TODO: better checking for each type
		case OC_STRUCTURE_ANY:
		case OC_STRUCTURE_PART:
		case OC_STRUCTURE_RES:
		case OC_STRUCTURE_COM:
		case OC_STRUCTURE_IND:
		case OC_STRUCTURE_PARK:
		case OC_STRUCTURE_ROAD:
		case OC_STRUCTURE_ELINE:
		case OC_STRUCTURE_EPLANT_COAL:
		case OC_STRUCTURE_FIREDEPT:
		case OC_STRUCTURE_POLICEDEPT:
		case OC_STRUCTURE_EDUCATIONDEPT:
		case OC_STRUCTURE_TEST:
			for (h = L1; h <= L2; h++) {
				linear = h*_uiLayerWidth + W1;
				for (w = W1; w <= W2; w++, linear++) {
					pstruct = GetLinearStructure( linear );
					if (pstruct != NULL) {
						if ((pstruct->GetCode() == enumStructCode)
						  ||(enumStructCode == OC_STRUCTURE_ANY))
							return true;
					}
				}  // while w
			}  // while h
			break;

		case OC_STRUCTURE_ELECTRIC:
			for (h = L1; h <= L2; h++) {
				linear = h*_uiLayerWidth + W1;
				for (w = W1; w <= W2; w++, linear++) {
					pstruct = GetLinearStructure( linear );
					if (pstruct != NULL) {
					// return true if this structure is
					// connected to the electric system
						if (pstruct->IsSet( OC_STRUCTURE_E ) == true)
							return true;
					}
				}  // while w
			}  // while h
			break;

		default:
			cerr << "WARNING: unknown structure code" << endl;
			assert(0);
			break;			// keep the compiler happy
	}  // switch

	return false;
}


   /*=====================================================================*/
const bool
BuildingLayer::ContainStructureOnly(
	const uint & W1,
	const uint & L1,
	const uint & W2,
	const uint & L2,
	const OPENCITY_STRUCTURE_CODE & enumStructCode ) const
{
	uint w, h, linear;
	Structure* pstruct;

	switch (enumStructCode) {
		case OC_STRUCTURE_UNDEFINED:
			return false;

		case OC_STRUCTURE_ANY:
			return true;

		case OC_STRUCTURE_PART:
		case OC_STRUCTURE_RES:
		case OC_STRUCTURE_COM:
		case OC_STRUCTURE_IND:
		case OC_STRUCTURE_PARK:
		case OC_STRUCTURE_ROAD:
		case OC_STRUCTURE_ELINE:
		case OC_STRUCTURE_EPLANT_COAL:
		case OC_STRUCTURE_FIREDEPT:
		case OC_STRUCTURE_POLICEDEPT:
		case OC_STRUCTURE_EDUCATIONDEPT:
		case OC_STRUCTURE_TEST:
			for (h = L1; h <= L2; h++) {
				linear = h*_uiLayerWidth + W1;
				for (w = W1; w <= W2; w++, linear++) {
					pstruct = GetLinearStructure( linear );
					if (pstruct != NULL) {
						if (pstruct->GetCode() != enumStructCode)
							return false;
					}
				}  // while w
			}  // while h
			break;

		case OC_STRUCTURE_ELECTRIC:
		// Return true if all the contained structures are electrified
			for (h = L1; h <= L2; h++) {
				linear = h*_uiLayerWidth + W1;
				for (w = W1; w <= W2; w++, linear++) {
					pstruct = GetLinearStructure( linear );
					if (pstruct != NULL) {
					// Return true if this structure is
					// connected to the electric system
						if (pstruct->IsSet( OC_STRUCTURE_E ) == false)
							return false;
					}
				}  // while w
			}  // while h
			break;

		default:
			cerr << "WARNING: unknown structure code" << endl;
			assert(0);
			break;			// keep the compiler happy
	}  // switch

	return true;
}


   /*=====================================================================*/
GUIContainer*
BuildingLayer::QueryStructure(
	const uint & w,
	const uint & h ) const
{
	GUIContainer* pcontainer = new GUIContainer( 100, 100, 50, 50 );
	Structure* pstruct = _tabpStructure[ h*_uiLayerWidth + w ];
	OPENCITY_COLOR red = { 255, 0, 0, 255 };
	OPENCITY_COLOR green = { 0, 255, 0, 255 };

// everything is RED :D
	Layer::pbtnQW->SetBackground( red );
	Layer::pbtnQE->SetBackground( red );
	Layer::pbtnQG->SetBackground( red );
	Layer::pbtnQR->SetBackground( red );
	Layer::pbtnQC->SetBackground( red );
	Layer::pbtnQI->SetBackground( red );

// look for the RCI structures around
// and WEG properties
	if ( pstruct != NULL ) {
	// we check only the E bit, WG are NOT implemented yet
		if ( pstruct->IsSet( OC_STRUCTURE_E ) )
			Layer::pbtnQE->SetBackground( green );

	// check for RCI
		if ( pstruct->IsSet( OC_STRUCTURE_R ) )
			Layer::pbtnQR->SetBackground( green );
		if ( pstruct->IsSet( OC_STRUCTURE_C ) )
			Layer::pbtnQC->SetBackground( green );
		if ( pstruct->IsSet( OC_STRUCTURE_I ) )
			Layer::pbtnQI->SetBackground( green );
	}

// add all the controls
	pcontainer->Add( Layer::pbtnQW );
	pcontainer->Add( Layer::pbtnQE );
	pcontainer->Add( Layer::pbtnQG );
	pcontainer->Add( Layer::pbtnQR );
	pcontainer->Add( Layer::pbtnQC );
	pcontainer->Add( Layer::pbtnQI );

	return pcontainer;
}




   /*=====================================================================*/
   /*                        PRIVATE     METHODS                          */
   /*=====================================================================*/
const bool
BuildingLayer::_IsPathConstructive(
	const uint& w,
	const uint& h,
	const OPENCITY_STRUCTURE_CODE& enumStructCode ) const
{
	OC_BYTE tabH[4];
	OC_BYTE minH = 127;
	OC_BYTE maxH = -127;
	OC_BYTE deltaH = 0;
	uint uiIndex = 0;
	OC_BYTE minHNb = 0;
	Structure* pStruct = NULL;

// Return immediatly if there's already 
// something different than enumStructCode built on it
	pStruct = this->GetStructure(w,h);
	if ((pStruct != NULL) && (pStruct->GetCode() != enumStructCode))
		return false;

// Calculate the min and the max heights
	gpMapMgr->GetSquareHeight( w, h, tabH );
	for (uiIndex = 0; uiIndex < 4; uiIndex++) {
		if (tabH[uiIndex] < minH)
			minH = tabH[uiIndex];
		if (tabH[uiIndex] > maxH)
			maxH = tabH[uiIndex];
	}

// Calculate the absolute difference
	deltaH = maxH - minH;
	if (deltaH > 1)
	// we can not build road if the difference is too big
		return false;

// Else, calculate the number of minH points
// minHNb is already initialized
	for (uiIndex = 0; uiIndex < 4; uiIndex++) {
		if (tabH[uiIndex] == minH)
			minHNb++;
	}

// The number of minH points should be the multiple of 2
	if ((minHNb % 2) != 0)
		return false;

// Here, the square is "road constructive"
	return true;
}


   /*=====================================================================*/
const OPENCITY_ERR_CODE
BuildingLayer::_BuildPathStructure(
	uint W1, uint L1,
	uint W2, uint L2,
	const OPENCITY_STRUCTURE_CODE & enumStructCode,
	uint& rCost )
{
	OPENCITY_DEBUG( "I'm building some path structures" );

	uint w, h;
	uint cost;

	OPENCITY_SWAP( W1, W2, uint );
	OPENCITY_SWAP( L1, L2, uint );

	cost = 0;
	rCost = 0;

// is the area road constructive ?
// which is the largest side ?
	if (W2 - W1 >= L2 - L1) {
	// we test the largest side first
		for (w = W1; w < W2; w++)
			if (_IsPathConstructive( w, L1, enumStructCode ) == false)
				return OC_ERR_SOMETHING;
	// we test the rest
		for (h = L1; h <= L2; h++)
			if (_IsPathConstructive( W2, h, enumStructCode ) == false)
				return OC_ERR_SOMETHING;
	}
	else {
	// we test the largest side first
		for (h = L1; h < L2; h++)
			if (_IsPathConstructive( W1, h, enumStructCode ) == false)
				return OC_ERR_SOMETHING;
	// we test the rest
		for (w = W1; w <= W2; w++)
			if (_IsPathConstructive( w, L2, enumStructCode ) == false)
				return OC_ERR_SOMETHING;
	}



// it's OK, let's GO !
	uint linearIndex;

	if (W2 - W1 >= L2 - L1) {
	// we build the largest side first
		linearIndex = (L1*_uiLayerWidth) + W1;
		for (w = W1; w < W2; w++, linearIndex++) {
			_BuildPathStructure( w, L1, linearIndex, enumStructCode, cost );
			rCost += cost;
		}

	// then we build the rest
// this is already calculated by the previous "for"
//		linearIndex = (L1*_uiLayerWidth) + W2;
		for (h = L1; h <= L2; h++, linearIndex += _uiLayerWidth) {
			_BuildPathStructure( W2, h, linearIndex, enumStructCode, cost );
			rCost += cost;
		}
	}
	else {
	// we build the largest side first
		linearIndex = (L1*_uiLayerWidth) + W1;
		for (h = L1; h < L2; h++, linearIndex += _uiLayerWidth) {
			_BuildPathStructure( W1, h, linearIndex, enumStructCode, cost );
			rCost += cost;
		}

	// then we build the rest
// this is already calculated by the previous "for"
//		linearIndex = (L2*_uiLayerWidth) + W1;
		for (w = W1; w <= W2; w++, linearIndex++) {
			_BuildPathStructure( w, L2, linearIndex, enumStructCode, cost );
			rCost += cost;
		}
	}

	return OC_ERR_FREE;
}


   /*=====================================================================*/
void
BuildingLayer::_BuildPathStructure(
	const uint & w,
	const uint & h,
	const uint & linearIndex,
	const OPENCITY_STRUCTURE_CODE & enumStructCode,
	uint& rCost )
{
	Structure* pstructNeighbour = NULL;
	PathStructure* ppathstructNeighbour;
	PathStructure* pNewStructure;
	uint nW, nH;


// IF there's already something, THEN we return
	if (_tabpStructure[linearIndex] != NULL) {
		rCost = 0;
		return;
	}

// Build the new road
	pNewStructure = new PathStructure(enumStructCode);
	rCost = gpPropertyMgr->Get( OC_BUILD_COST, enumStructCode );
	_tabpStructure[ linearIndex ] = pNewStructure;

// Get the neighbour in the North
	if (gpMapMgr->GetNeighbourWH( w, h, nW, nH, OC_DIR_N ) == true) {
		pstructNeighbour = BuildingLayer::GetStructure( nW, nH );
	// if the neighbour is a road structure
	// then add this new road structure as its new neighbour
	// we do the same with the new road structure
		if ( (pstructNeighbour != NULL)
		   &&(pstructNeighbour->GetCode() == enumStructCode)) {
//debug cout << "found 1 neighbour in the North" << endl;
			ppathstructNeighbour = (PathStructure*)pstructNeighbour;
			ppathstructNeighbour->AddNeighbour(
				*pNewStructure, OC_DIR_S );
			pNewStructure->AddNeighbour(
				*ppathstructNeighbour, OC_DIR_N );
		}
	}

// Get the neighbour in the South
	if (gpMapMgr->GetNeighbourWH( w, h, nW, nH, OC_DIR_S ) == true) {
		pstructNeighbour = BuildingLayer::GetStructure( nW, nH );
		if ( (pstructNeighbour != NULL)
		   &&(pstructNeighbour->GetCode() == enumStructCode)) {
			ppathstructNeighbour = (PathStructure*)pstructNeighbour;
			ppathstructNeighbour->AddNeighbour(
				*pNewStructure, OC_DIR_N );
			pNewStructure->AddNeighbour(
				*ppathstructNeighbour, OC_DIR_S );
		}
	}

// Get the neighbour in the West
	if (gpMapMgr->GetNeighbourWH( w, h, nW, nH, OC_DIR_W ) == true) {
		pstructNeighbour = BuildingLayer::GetStructure( nW, nH );
		if ( (pstructNeighbour != NULL)
		   &&(pstructNeighbour->GetCode() == enumStructCode)) {
			ppathstructNeighbour = (PathStructure*)pstructNeighbour;
			ppathstructNeighbour->AddNeighbour(
				*pNewStructure, OC_DIR_E );
			pNewStructure->AddNeighbour(
				*ppathstructNeighbour, OC_DIR_W );
		}
	}

// Get the neighbour in the East
	if (gpMapMgr->GetNeighbourWH( w, h, nW, nH, OC_DIR_E ) == true) {
		pstructNeighbour = BuildingLayer::GetStructure( nW, nH );
	// if the neighbour is a road structure
	// then add this new road structure as its new neighbour
	// we do the same with the new road structure
		if ( (pstructNeighbour != NULL)
		   &&(pstructNeighbour->GetCode() == enumStructCode)) {
			ppathstructNeighbour = (PathStructure*)pstructNeighbour;
			ppathstructNeighbour->AddNeighbour(
				*pNewStructure, OC_DIR_W );
			pNewStructure->AddNeighbour(
				*ppathstructNeighbour, OC_DIR_E );
		}
	}
}


   /*=====================================================================*/
/* TODO: next version
const OPENCITY_ERR_CODE
BuildingLayer::_BuildRCIStructure(
	uint W1, uint L1,
	const OPENCITY_STRUCTURE_CODE & enumStructCode,
	uint& rCost )
{
	OPENCITY_DEBUG( "I'm building some RCI structures" );

	uint W2, L2;
	uint sw, sl, sh;				// Structure's width, length and height
	OPENCITY_GRAPHIC_CODE gcode;

// Get the graphic code of the structure
	gcode = gpPropertyMgr->GetGC( enumStructCode );
	if ( gcode == OC_EMPTY ) {
		OPENCITY_DEBUG( "WARNING: not implemented" );
		assert(0);
	}

// Get the corresponding WLH and calculate the range
// NOTE: An coal electric plant is 4x4 size by default
	gpPropertyMgr->GetWLH( gcode, sw, 4, sl, 4, sh, 1 );
	W2 = W1 + sw - 1;
	L2 = L1 + sl - 1;

// Isn't there enough space ?
// OR is there already something on the surface ?
	if (IsConstructive(W1, L1, W2, L2, enumStructCode) == false) {
		return OC_ERR_SOMETHING;
	}

// Let's do it
	_BuildRCIStructure( W1, L1, W2, L2, enumStructCode, rCost );

	return OC_ERR_FREE;
}
*/


   /*=====================================================================*/
/* TODO: next version
void
BuildingLayer::_BuildRCIStructure(
	uint W1, uint L1,
	uint W2, uint L2,
	const OPENCITY_STRUCTURE_CODE & enumStructCode,
	uint& rCost )
{
	uint w, l;
	Structure* pNewStructure, *pMainStructure;
	uint linearIndex;

// Create the main structure first, but we don't insert it
	pMainStructure = new RCIStructure( enumStructCode );
	rCost = gpPropertyMgr->Get( OC_BUILD_COST, enumStructCode );

// Create all the area as parts of a bigger main structure
	l = L1;
	while (l <= L2) {
		linearIndex = (l*_uiLayerWidth) + W1;
		w = W1;
		while (w <= W2) {
		// Build new WEG as a structure part
			_tabpStructure[ linearIndex++ ] =
				new RCIStructure( OC_STRUCTURE_PART, pMainStructure);
			w++;
			// linearIndex++; already done
		}
		l++;
	}

// Delete the part structure at the coordinates W1,L1
	linearIndex = (L1*_uiLayerWidth) + W1;
	pNewStructure = _tabpStructure[ linearIndex ];
	delete pNewStructure;
// Put the main structure upon it
	_tabpStructure[ linearIndex ] = pMainStructure;
}
*/


   /*=====================================================================*/
const OPENCITY_ERR_CODE
BuildingLayer::_BuildRCIStructure(
	uint W1, uint L1,
	uint W2, uint L2,
	const OPENCITY_STRUCTURE_CODE & enumStructCode,
	uint& rCost )
{
	OPENCITY_DEBUG( "I'm building some RCI structures" );

	static uint w, l;
	static uint linearIndex;
	static int cost;


	OPENCITY_SWAP( W1, W2, uint );
	OPENCITY_SWAP( L1, L2, uint );

//debug testing
//cout << "size of RCIStructure is: " << sizeof( RCIStructure ) << endl;

	rCost = 0;
	cost = gpPropertyMgr->Get( OC_BUILD_COST, enumStructCode );

// Let's GO !
	l = L1;
	while (l <= L2) {
		linearIndex = (l*_uiLayerWidth) + W1;
		w = W1;
		while (w <= W2) {
			if ((_tabpStructure[ linearIndex ] == NULL)
			 && (gpMapMgr->IsSquarePlane(w, l) == true )) {
				_tabpStructure[ linearIndex ] = new RCIStructure(enumStructCode);
				rCost += cost;
			}
			w++;
			linearIndex++;
		} // while w
		l++;
	} // while l

	return (rCost > 0) ? OC_ERR_FREE : OC_ERR_SOMETHING;
}


   /*=====================================================================*/
const OPENCITY_ERR_CODE
BuildingLayer::_BuildWEGStructure(
	uint W1, uint L1,
	const OPENCITY_STRUCTURE_CODE & enumStructCode,
	uint& rCost )
{
	OPENCITY_DEBUG( "I'm building some WEG structures" );

	uint W2, L2;
	uint sw, sl, sh;				// Structure's width, length and height
	OPENCITY_GRAPHIC_CODE gcode;

// Get the graphic code of the structure
	gcode = gpPropertyMgr->GetGC( enumStructCode );
	if ( gcode == OC_EMPTY ) {
		OPENCITY_DEBUG( "WARNING: not implemented" );
		assert(0);
	}

// Get the corresponding WLH and calculate the range
// NOTE: An coal electric plant is 4x4 size by default
	gpPropertyMgr->GetWLH( gcode, sw, 4, sl, 4, sh, 1 );
	W2 = W1 + sw - 1;
	L2 = L1 + sl - 1;

// Isn't there enough space ?
// OR is there already something on the surface ?
	if (IsConstructive(W1, L1, W2, L2, enumStructCode) == false) {
		return OC_ERR_SOMETHING;
	}

// Let's do it
	_BuildWEGStructure( W1, L1, W2, L2, enumStructCode, rCost );

	return OC_ERR_FREE;
}


   /*=====================================================================*/
void
BuildingLayer::_BuildWEGStructure(
	uint W1, uint L1,
	uint W2, uint L2,
	const OPENCITY_STRUCTURE_CODE & enumStructCode,
	uint& rCost )
{
	uint w, l;
	Structure* pNewStructure, *pMainStructure;
	uint linearIndex;

// Create the main structure first, but we don't insert it
	pMainStructure = new WEGStructure( enumStructCode );
	rCost = gpPropertyMgr->Get( OC_BUILD_COST, enumStructCode );

// Create all the area as parts of a bigger main structure
	l = L1;
	while (l <= L2) {
		linearIndex = (l*_uiLayerWidth) + W1;
		w = W1;
		while (w <= W2) {
		// Build new WEG as a structure part
			_tabpStructure[ linearIndex++ ] =
				new WEGStructure( OC_STRUCTURE_PART, pMainStructure );
			w++;
			// linearIndex++; already done
		}
		l++;
	}

// Delete the part structure at the coordinates W1,L1
	linearIndex = (L1*_uiLayerWidth) + W1;
	pNewStructure = _tabpStructure[ linearIndex ];
	delete pNewStructure;
// Put the main structure upon it
	_tabpStructure[ linearIndex ] = pMainStructure;
}


   /*=====================================================================*/
const OPENCITY_ERR_CODE
BuildingLayer::_DestroyStructure(
	const uint & w,
	const uint & l,
	uint& rCost )
{
	uint linearIndex = (l*_uiLayerWidth) + w;
	Structure* pstruct, *pstructMain;
	uint dw1 = w, dl1 = l, dw2 = w, dl2 = l;
	uint tempW, tempL;
	uint sw, sl, sh;				// Structure's width, length and height
	OPENCITY_ERR_CODE errCode;

// The fund used to destroy this structure
	rCost = 0;

// Get the structure, if there is nothing, we return error
	pstruct = _tabpStructure[ linearIndex ];
	if ( pstruct == NULL )
		return OC_ERR_SOMETHING;

	rCost = gpPropertyMgr->Get( OC_DESTROY_COST, pstruct->GetCode(), pstruct );

// The main structure is pstruct itself
	pstructMain = pstruct;
	errCode = OC_ERR_FREE;
	switch ( pstruct->GetCode() ) {
		case OC_STRUCTURE_PART:
		// We get the main structure otherwise
			pstructMain = pstruct->GetMain();
		case OC_STRUCTURE_RES:
		case OC_STRUCTURE_COM:
		case OC_STRUCTURE_IND:

		case OC_STRUCTURE_PARK:
		case OC_STRUCTURE_TEST:
		case OC_STRUCTURE_FIREDEPT:
		case OC_STRUCTURE_POLICEDEPT:
		case OC_STRUCTURE_EDUCATIONDEPT:
		case OC_STRUCTURE_EPLANT_COAL:
		// Get the width, length and height of the main structure
		// WARNING: invalid default values given.
			gpPropertyMgr->GetWLH( pstructMain->GetGraphicCode(), sw, 0, sl, 0, sh, 0 );
			assert( (sw != 0) && (sl != 0) );
			sw--; sl--;			// We calculate the range

		// Calculate the possible area to limit cpu usage
			gpMapMgr->GetPossibleWH( dw1, dl1, -sw, -sl );
			gpMapMgr->GetPossibleWH( dw2, dl2,  sw,  sl );
		// Now search for all STRUCTURE_PART and the main structure, destroy em
			for ( tempL = dl1; tempL <= dl2; tempL++ ) {
				linearIndex = tempL*_uiLayerWidth + dw1;
				for ( tempW = dw1; tempW <= dw2; tempW++ ) {
//debug cout << "Hi coco " << endl;
				// WARNING: pointer comparaison !
					pstruct = _tabpStructure[ linearIndex ];
					if ( pstruct != NULL )
					if (( pstruct->GetMain() == pstructMain )
					 || (pstruct == pstructMain)) {
						_tabpStructure[ linearIndex ] = NULL;
						delete pstruct;
//debug cout << "coco died" << endl;
					}
					linearIndex++;
				}
			}
			break;

		case OC_STRUCTURE_ELINE:
		case OC_STRUCTURE_ROAD:
			errCode = _DestroyPathStructure(
					w, l, pstruct->GetCode() );
			break;

		default:
			errCode = OC_ERR_SOMETHING;
	}

	return errCode;
}


   /*=====================================================================*/
const OPENCITY_ERR_CODE
BuildingLayer::_DestroyPathStructure(
	uint w, uint l,
	const OPENCITY_STRUCTURE_CODE & enumStructCode )
{
	Structure* pstruct, *pstructNeighbour;
	PathStructure* ppathstructNeighbour, *ppathstruct;
	uint nW, nH;

	pstruct = _tabpStructure[ l*_uiLayerWidth + w ];
// this is not necessary because it's already checked by the user "Destroy"
	if (pstruct == NULL)
		return OC_ERR_SOMETHING;

	ppathstruct = (PathStructure*)pstruct;
// get the neighbour in the North
	if (gpMapMgr->GetNeighbourWH( w, l, nW, nH, OC_DIR_N ) == true) {
		pstructNeighbour = BuildingLayer::GetStructure( nW, nH );
	// if the neighbour is a path structure
		if ( (pstructNeighbour != NULL)
		   &&(pstructNeighbour->GetCode() == enumStructCode)) {
			ppathstructNeighbour = (PathStructure*)pstructNeighbour;
			ppathstructNeighbour->RemoveNeighbour( *ppathstruct, OC_DIR_S );
		}
	}

// get the neighbour in the South
	if (gpMapMgr->GetNeighbourWH( w, l, nW, nH, OC_DIR_S ) == true) {
		pstructNeighbour = BuildingLayer::GetStructure( nW, nH );
		if ( (pstructNeighbour != NULL)
		   &&(pstructNeighbour->GetCode() == enumStructCode)) {
			ppathstructNeighbour = (PathStructure*)pstructNeighbour;
			ppathstructNeighbour->RemoveNeighbour( *ppathstruct, OC_DIR_N );
		}
	}

// get the neighbour in the West
	if (gpMapMgr->GetNeighbourWH( w, l, nW, nH, OC_DIR_W ) == true) {
		pstructNeighbour = BuildingLayer::GetStructure( nW, nH );
		if ( (pstructNeighbour != NULL)
		   &&(pstructNeighbour->GetCode() == enumStructCode)) {
			ppathstructNeighbour = (PathStructure*)pstructNeighbour;
			ppathstructNeighbour->RemoveNeighbour( *ppathstruct, OC_DIR_E );
		}
	}

// get the neighbour in the East
	if (gpMapMgr->GetNeighbourWH( w, l, nW, nH, OC_DIR_E ) == true) {
		pstructNeighbour = BuildingLayer::GetStructure( nW, nH );
		if ( (pstructNeighbour != NULL)
		   &&(pstructNeighbour->GetCode() == enumStructCode)) {
			ppathstructNeighbour = (PathStructure*)pstructNeighbour;
			ppathstructNeighbour->RemoveNeighbour( *ppathstruct, OC_DIR_W );
		}
	}

	delete pstruct;
	_tabpStructure[ l*_uiLayerWidth + w ] = NULL;

	return OC_ERR_FREE;
}





































