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
	minEdge.x=minEdge.y=minEdge.z=numeric_limits<unsigned short>::max();
	maxEdge.x=maxEdge.y=maxEdge.z=numeric_limits<unsigned short>::min();
	if(img)
	{
		end=start;
		vincent::iPunkt<vincent::lab_value> p=(*plist)[start];
		vincent::lab_value ID=p.wert;
//		key=GLvlMinimaBase::(*img)[p];
		do 
		{
			const unsigned short x=p.x(GLvlMinimaBase::img->xsize);
			const unsigned short y=p.y(GLvlMinimaBase::img->xsize,GLvlMinimaBase::img->ysize);
			const unsigned short z=p.z(GLvlMinimaBase::img->xsize,GLvlMinimaBase::img->ysize);
			
			minEdge.x = minEdge.x <? x;
			minEdge.y = minEdge.y <? y;
			minEdge.z = minEdge.z <? z;
	
			maxEdge.x = maxEdge.x >? x;
			maxEdge.y = maxEdge.y >? y;
			maxEdge.z = maxEdge.z >? z;
			
			p=(*plist)[++end]; 
		}
		while(end<plist->size && p.wert==ID);
	}
	else
	{SGLprintError("GLvlMinima::setup wurde nich ausgeführt, das Objekt kann nicht angelegt werden");abort();}
}

shared_ptr<vincent::Bild_vimage<vincent::lab_value> > GLvlMinimaBase::img;
shared_ptr<vincent::PunktList<vincent::lab_value> > GLvlMinimaBase::plist;
bool GLvlMinimaBase::incl_wshed=false;

void GLvlMinimaBase::setup(	boost::shared_ptr< vincent::Bild_vimage<vincent::lab_value>  > img)
{
	GLvlMinimaBase::img=img;
	GLvlMinimaBase::plist=vincent::transform::getVoxels(*img);
}

