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
class GLvlMinimaBase: public SGLFlObj{
	static shared_ptr<GLvlVolumeTex> tex;
	static shared_ptr< vincent::Bild_vimage<vincent::lab_value> > img;
	
public:
	static shared_ptr< vincent::PunktList<vincent::lab_value> > plist;
	unsigned int start,end;
	static bool incl_wshed;
	GLvlMinimaBase(unsigned int pos);
    SGLVektor getCenter();
    static void setup(
		SGLVektor norm,
		boost::shared_ptr<GLvlVolumeTex> tex,
		boost::shared_ptr< vincent::Bild_vimage<vincent::lab_value>  > img
	);
	inline const unsigned int size(){return end-start;}
};

class GLvlMinima3D: public GLvlMinimaBase{
public:
	GLvlMinima3D(unsigned int pos);
	static GLuint caps;
    void generate();
    static void setup(
		SGLVektor norm,
		boost::shared_ptr<GLvlVolumeTex> tex,
		boost::shared_ptr< vincent::Bild_vimage<vincent::lab_value>  > img
	);
};

#endif
