//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "glvlminimatex.h"

GLvlMinimaTex::GLvlMinimaTex(unsigned int pos) : GLvlMinimaBase(pos)
{
	begin.x=begin.y=begin.z=numeric_limits<unsigned short>::max();
	end.x=end.y=end.z=numeric_limits<unsigned short>::min();

	key=GLvlMinimaBase::plist.at(start);
	for(unsigned int i=start;i<end;i++)
	{
		const vincent::iPunkt<vincent::lab_value> p=(*plist)[i];
		const unsigned short x=p.x(img->xsize);
		const unsigned short y=p.x(img->ysize);
		const unsigned short z=p.x(img->zsize);
		if(x<begin.x)begin.x=x;
	}
}

void GLvlMinimaTex::setup(boost::shared_ptr< vincent::Bild_vimage<vincent::lab_value>  > img)
{
	GLvlMinimaBase::setup(img);
}
