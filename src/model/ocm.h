/***************************************************************************
						ocm.h  -  description
							-------------------
	begin                : may 22th, 2004
	copyright            : (C) 2004 by Duong-Khang NGUYEN
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

#ifndef _OPENCITY_OCM_H_
#define _OPENCITY_OCM_H_ 1

#define OC_OCM_VERTEX	10		///< glVertex3f()
#define OC_OCM_TEXCOOR	11		///< glTexCoord3fv()

#define OC_OCM_COLOR	20		///< glColor4fv()

   /** The following line contains the relative path to the
       texture file. All the textures must be loaded before any
       other texture command
   */
#define OC_OCM_TEXFILE	30
#define OC_OCM_TEXBIND	31		///< glBindTexture()


#endif
































