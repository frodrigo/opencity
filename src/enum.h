/***************************************************************************
						enum.h  -  description
							-------------------
	begin                : may 8th, 2006
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

#ifndef _OPENCITY_ENUM_H_
#define _OPENCITY_ENUM_H_ 1

/** OpenCity's error codes
*/
	enum OPENCITY_ERR_CODE {
		OC_ERR_FREE = 0,		///< No error
		OC_ERR_SOMETHING,		///< Something went wrong ?
		OC_ERR_INVALID,			///< Invalid value on conversion / invalid method argument
		OC_ERR_FILE				///< File error: read/open/write
	};

	enum OPENCITY_MAP_VARIATION {
		OC_MAP_UP = 1,
		OC_MAP_DOWN = -1
	};

/** The different layers of OpenCity's city
*/
	enum OPENCITY_CITY_LAYER {
		OC_LAYER_BUILDING = 0,		///< The building layer contains buildings :)
		OC_LAYER_WATER,				///< The water layer shows the water pipes
		OC_LAYER_GAS,				///< ?
		OC_LAYER_SUBWAY				///< ?
	};

	enum OPENCITY_CITY_SPEED {
		OC_SPEED_PAUSE = 0,
		OC_SPEED_NORMAL,
		OC_SPEED_FAST
	};

	enum OPENCITY_CITY_KEY {
		KEY_UP = 0,
		KEY_DOWN,
		KEY_LEFT,
		KEY_RIGHT,
		KEY_PAGEUP,
		KEY_PAGEDOWN,
		KEY_ALT,
		KEY_INSERT,
		KEY_DELETE,

		KEY_NUMBER			// Total number of keys
	};

// OpenCity's code for tool
	enum OPENCITY_TOOL_CODE {
		OC_TOOL_NONE = 0,
		OC_TOOL_DESTROY,

		OC_TOOL_ZONE_RES,
		OC_TOOL_ZONE_COM,
		OC_TOOL_ZONE_IND,

		OC_TOOL_HEIGHT_UP,
		OC_TOOL_HEIGHT_DOWN,

		OC_TOOL_ROAD,

		OC_TOOL_ELINE,
		OC_TOOL_EPLANT_COAL,
//		OC_TOOL_EPLANT_GAS,
//		OC_TOOL_EPLANT_OIL,
		OC_TOOL_EPLANT_NUCLEAR,

		OC_TOOL_PARK,
		OC_TOOL_FLORA,

		OC_TOOL_FIRE,
		OC_TOOL_POLICE,
		OC_TOOL_HOSPITAL,
//		OC_TOOL_MILITARY,
		OC_TOOL_EDUCATION,

		OC_TOOL_AGENT_POLICE,
		OC_TOOL_AGENT_DEMONSTRATOR,
		OC_TOOL_AGENT_ROBBER,

		OC_TOOL_TEST_BUILDING,
		OC_TOOL_QUERY,

		OC_TOOL_NUMBER			// Total number of tools
	};

// OpenCity's codes for structures
// See the notes above the GRAPHIC_CODE
	enum OPENCITY_STRUCTURE_CODE {
		OC_STRUCTURE_UNDEFINED = 0,
		OC_STRUCTURE_TEST,
		OC_STRUCTURE_ANY,
		OC_STRUCTURE_PART,

		OC_STRUCTURE_RES = 10,
		OC_STRUCTURE_COM,
		OC_STRUCTURE_IND,

		OC_STRUCTURE_PARK,
		OC_STRUCTURE_FLORA,

		OC_STRUCTURE_ROAD = 20,

		OC_STRUCTURE_ELECTRIC = 30,
		OC_STRUCTURE_ELINE,
		OC_STRUCTURE_EPLANT_COAL,
		OC_STRUCTURE_EPLANT_GAS,
		OC_STRUCTURE_EPLANT_OIL,
		OC_STRUCTURE_EPLANT_NUCLEAR,

		OC_STRUCTURE_FIREDEPT,
		OC_STRUCTURE_POLICEDEPT,
		OC_STRUCTURE_HOSPITALDEPT,
		OC_STRUCTURE_MILITARYDEPT,
		OC_STRUCTURE_EDUCATIONDEPT
	};

/** Opencity's graphic codes ( model internal code )
	These codes are different from structure codes in the following ways:
	a) The same structure code can have more than 1 associated graphic code.
	b) The same graphic code can have more than 1 associated models.
	c) For eg. RES_ZONE0 is used for residential zone level 0. However, the
		associated model can have up to 10 internal graphic levels. They are
		represented by the groups contained in the models.
*/
	enum OPENCITY_GRAPHIC_CODE {
		OC_EMPTY = 0,
		OC_TEST_BUILDING = 1,			// Used by the graphists for testing their works

		OC_RES_ZONE0 = 10,
		OC_RES_ZONE1,
		OC_RES_ZONE2,
		OC_RES_ZONE3,
		OC_RES_ZONE4,
		OC_RES_ZONE5,
		OC_RES_ZONE6,
		OC_RES_ZONE7,
		OC_RES_ZONE8,
		OC_RES_ZONE9,

		OC_COM_ZONE0 = 20,
		OC_COM_ZONE1,
		OC_COM_ZONE2,
		OC_COM_ZONE3,
		OC_COM_ZONE4,
		OC_COM_ZONE5,
		OC_COM_ZONE6,
		OC_COM_ZONE7,
		OC_COM_ZONE8,
		OC_COM_ZONE9,

		OC_IND_ZONE0 = 30,
		OC_IND_ZONE1,
		OC_IND_ZONE2,
		OC_IND_ZONE3,
		OC_IND_ZONE4,
		OC_IND_ZONE5,
		OC_IND_ZONE6,
		OC_IND_ZONE7,
		OC_IND_ZONE8,
		OC_IND_ZONE9,

		OC_TERRAIN_PLANE = 100,
/*not used
		OC_TERRAIN_N,
		OC_TERRAIN_NE,
		OC_TERRAIN_E,
		OC_TERRAIN_SE,
		OC_TERRAIN_S,
		OC_TERRAIN_SW,
		OC_TERRAIN_W,
		OC_TERRAIN_NW,
		OC_TERRAIN_SE3,
		OC_TERRAIN_NW3,
		OC_TERRAIN_SW3,
		OC_TERRAIN_NE3,
*/

	// The order of the following enumerations are important
	// See pathstructure.cpp for more information
		OC_ROAD_O_N = 150,
		OC_ROAD_O_E,     // +1
		OC_ROAD_O_S,
		OC_ROAD_O_W,
		OC_ROAD_S_N,
		OC_ROAD_W_E,     // +5
		OC_ROAD_N_E,
		OC_ROAD_N_W,
		OC_ROAD_S_E,
		OC_ROAD_S_W,
		OC_ROAD_S_N_E,   // +10
		OC_ROAD_S_W_E,
		OC_ROAD_S_N_W,
		OC_ROAD_N_W_E,
		OC_ROAD_S_N_W_E,

	// The order of the following enumerations are important
	// See pathstructure.cpp for more information
		OC_ELINE_O_N = 170,
		OC_ELINE_O_E,
		OC_ELINE_O_S,
		OC_ELINE_O_W,
		OC_ELINE_S_N,
		OC_ELINE_W_E,
		OC_ELINE_N_E,
		OC_ELINE_N_W,
		OC_ELINE_S_E,
		OC_ELINE_S_W,
		OC_ELINE_S_N_E,
		OC_ELINE_S_W_E,
		OC_ELINE_S_N_W,
		OC_ELINE_N_W_E,
		OC_ELINE_S_N_W_E,

		OC_WPIPE_O_N = 190,
		OC_WPIPE_O_E,     // +1
		OC_WPIPE_O_S,
		OC_WPIPE_O_W,
		OC_WPIPE_S_N,
		OC_WPIPE_W_E,     // +5
		OC_WPIPE_N_E,
		OC_WPIPE_N_W,
		OC_WPIPE_S_E,
		OC_WPIPE_S_W,
		OC_WPIPE_S_N_E,   // +10
		OC_WPIPE_S_W_E,
		OC_WPIPE_S_N_W,
		OC_WPIPE_N_W_E,
		OC_WPIPE_S_N_W_E,

		OC_GPIPE_O_N = 210,
		OC_GPIPE_O_E,     // +1
		OC_GPIPE_O_S,
		OC_GPIPE_O_W,
		OC_GPIPE_S_N,
		OC_GPIPE_W_E,     // +5
		OC_GPIPE_N_E,
		OC_GPIPE_N_W,
		OC_GPIPE_S_E,
		OC_GPIPE_S_W,
		OC_GPIPE_S_N_E,   // +10
		OC_GPIPE_S_W_E,
		OC_GPIPE_S_N_W,
		OC_GPIPE_N_W_E,
		OC_GPIPE_S_N_W_E,

		OC_EPLANT_COAL = 300,	// to 319
		OC_EPLANT_GAS,
		OC_EPLANT_OIL,
		OC_EPLANT_NUCLEAR,

		OC_PARK0 = 320,
//		OC_PARK_MEDIUM = 330,
//		OC_PARK_BIG = 340,

		OC_TREE_BEGIN = 330,
		OC_TREE_FIR = 330,
		OC_TREE_007,
		OC_TREE_PEKINGWILLOW,
		OC_TREE_PINE1,
		OC_TREE_PINE2,
		OC_TREE_END = 334,

		OC_FIRE_DEPT = 350,
		OC_POLICE_DEPT = 360,
		OC_HOSPITAL_DEPT = 370,
		OC_MILITARY_DEPT = 380,
		OC_EDUCATION_DEPT = 390,

		OC_VEHICLE_STD = 400,
		OC_VEHICLE_FX = 430,
		OC_VEHICLE_PICKUP = 440,
		OC_VEHICLE_ROBBER = 441,
		OC_VEHICLE_FIRE = 450,
		OC_VEHICLE_POLICE = 460,
		OC_VEHICLE_HOSPITAL = 470,
		OC_VEHICLE_MILITARY = 480,
		OC_VEHICLE_EDUCATION = 490,

		OC_GRAPHIC_CODE_MAX
	};

/** In game home made sound effects
*/
	enum OPENCITY_SOUND_EFFECT {
		OC_SOUND_DESTROY,
		OC_SOUND_ELINE,
		OC_SOUND_EPLANT,
		OC_SOUND_PARK,
		OC_SOUND_RCI,
		OC_SOUND_ROAD,
		OC_SOUND_TERRAIN
	};

/** Code for getting structure's properties
*/
	enum OPENCITY_PROPERTY_CODE {
		OC_TOOL_COST,				///< Get the build cost
		OC_DESTROY_COST,			///< Get the destroy cost
		OC_MAINTENANCE_COST,		///< Get the maintenance cost
		OC_INCOME,					///< Get the income generated by one structure
		OC_CODE_TO_TYPE				///< Get the type of a structure from its code
	};

#endif




































