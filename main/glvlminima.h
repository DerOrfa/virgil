//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SGLOBJGLVLMINIMA_H
#define SGLOBJGLVLMINIMA_H

#include <primitives/sglflobj.h>
#include "../wshed/vincent_transform.h"
#include "../wshed/vincent_punkt.h"
#include <glvlvolumetex.h>
#include <GL/glu.h>

#define MAX_MINIMA_SIZE 500000

/**
@author Enrico Reimer,,,
*/
class GLvlMinima: public SGLFlObj{
	static shared_ptr<GLvlVolumeTex> tex;
	static GLuint caps;
	static shared_ptr< vincent::Bild_vimage<vincent::lab_value> > img;
	inline static void QuadBegin(GLuint id)
	{
		assert(glIsList(id));
		glNewList(id,GL_COMPILE);
	}
	inline static void QuadEnd()
	{
		glEndList();
		SGLcheckGLError;
	}
	static const GLshort GLvlMinima::diff[6][5][3];
public:
	static shared_ptr< vincent::PunktList<vincent::lab_value> > plist;
	unsigned int start,end;
	static bool incl_wshed;
	GLvlMinima(unsigned int pos);
    void generate();
    SGLVektor getCenter();
    static void setup(
		SGLVektor norm,
		boost::shared_ptr<GLvlVolumeTex> tex,
		boost::shared_ptr< vincent::Bild_vimage<vincent::lab_value>  > img
	);
	inline const unsigned int size(){return end-start;}
};

#endif
