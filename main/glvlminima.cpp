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
#include "glvlminima.h"
#include <sglvektor.h>

GLvlMinimaBase::GLvlMinimaBase(unsigned int pos):start(pos)
{
	if(img)
	{
		end=start;
		vincent::iPunkt<vincent::lab_value> p=(*plist)[start];
		vincent::lab_value ID=p.wert;
		do p=(*plist)[++end]; 
		while(end<plist->size && p.wert==ID);
	}
	else
	{SGLprintError("GLvlMinima::setup wurde nich ausgeführt, das Objekt kann nicht angelegt werden");}
}
SGLVektor GLvlMinimaBase::getCenter(){
    /// @todo implement me
}

shared_ptr<vincent::Bild_vimage<vincent::lab_value> > GLvlMinimaBase::img;
shared_ptr<vincent::PunktList<vincent::lab_value> > GLvlMinimaBase::plist;
bool GLvlMinimaBase::incl_wshed=false;

void GLvlMinimaBase::setup(	boost::shared_ptr< vincent::Bild_vimage<vincent::lab_value>  > img)
{
	GLvlMinimaBase::img=img;
	GLvlMinimaBase::plist=vincent::transform::getVoxels(*img);
}

