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
class GLvlMinimaBase: public SGLFlObj
{
public:
	static shared_ptr< vincent::Bild_vimage<vincent::lab_value> > img;
	unsigned int start,end;
	static shared_ptr< vincent::PunktList<vincent::lab_value> > plist;
	GLvlMinimaBase(unsigned int pos);
    SGLVektor getCenter();
	static void setup(boost::shared_ptr< vincent::Bild_vimage<vincent::lab_value>  > img);
	static bool incl_wshed;
	inline const unsigned int size(){return end-start;}
};

#endif
