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
#ifndef GLVLMINIMATEX_H
#define GLVLMINIMATEX_H

#include <glvlminima.h>

/**
@author Enrico Reimer,,,
*/
class GLvlMinimaTex : public GLvlMinimaBase,public SGLBaseTex
{
public:
	GLvlMinimaTex(unsigned int pos);
    static void setup(boost::shared_ptr< vincent::Bild_vimage<vincent::lab_value>  > img);
	struct {unsigned short x,y,z;}begin,end;
	vincent::lab_value key;
};

#endif
