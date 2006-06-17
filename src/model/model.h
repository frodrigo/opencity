/***************************************************************************
                          model.h  -  description
          $Id$
                             -------------------
    begin                : sam mai 22 2004
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

#ifndef _OPENCITY_MODEL_H_
#define _OPENCITY_MODEL_H_ 1

#include "main.h"

#include <map>


//========================================================================
/** This is an internal 3d model representation used by OpenCity.
*/
class Model {
public:
	Model(
		const GLfloat data [],
		const GLuint & size );

	Model(
		const GLfloat data [],
		const GLuint & size,
		const GLfloat rgb [],
		const GLfloat tcoord [],
		const GLuint  tname [] );

	Model(
		GLuint dl,
		std::map<string, GLuint> mapTex );

	~Model();


//========================================================================
/** Issue the necessary gl commands to display the model. Be careful, you
	should call this method in the glBegin( GL_QUADS ) / glEnd() block;
\param rcfW = GL x
\param rcfH = GL z
\param rcfY = GL y
*/
	void
	Display(
		const OC_FLOAT & rcfW = 0,
		const OC_FLOAT & rcfH = 0,
		const OC_FLOAT & rcfY = 0 ) const;


//========================================================================
/** Same as modelDisplay but automatically add Y coordinates
\sa modelDisplay()
*/
	void
	Display2(
		const OC_FLOAT & rcfW,
		const OC_FLOAT & rcfH,
		const OC_BYTE tabY []) const;


//========================================================================
	void
	DisplayPoly(
		const OC_FLOAT & rcfW,
		const OC_FLOAT & rcfH,
		const OC_BYTE tabY []
	) const;


//========================================================================
/** Simply call the internal OpenGL display list
*/
	void
	DisplayList() const;


//========================================================================
/** Call the internal OpenGL display list after modifying the MODELVIEW
	matrix according to the given parameters
\param rcfW = GL x
\param rcfL = GL z
\param tabY = GL y
*/
	void
	DisplayList(
		const OC_FLOAT & rcfW,
		const OC_FLOAT & rcfL,
		const OC_BYTE tabY []
	) const;


private:
	const GLfloat* ftabData;		// table of vertice
	const GLfloat* ftabRGB;
	const GLfloat* ftabTexCoord;
	const GLuint*  uitabTexName;
	GLuint uiTabSize;

// Test display list with AC3D
	GLuint uiDisplayList;
	std::map<string, GLuint> mapTexture;
};


#endif




































