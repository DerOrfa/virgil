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
	color[0]=color[2]=0;
	color[1]=1;
	if(img)
	{
		end=start;
		vincent::iPunkt<vincent::lab_value> p=(*plist)[start];
		vincent::lab_value ID=p.wert;
		buttomCap = (*org)[p];
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
			
		topCap = (*org)[(*plist)[end-1]];
		//getPixel verwendet stable_sort -> die Punkte eines Objektes sind noch der Größe nach geordnet
	}
	else{SGLprintError("GLvlMinima::setup wurde nich ausgeführt, das Objekt kann nicht angelegt werden");abort();}
}

shared_ptr<vincent::Bild_vimage<vincent::lab_value> > GLvlMinimaBase::img;
shared_ptr< vincent::Bild_vimage<VUByte> > GLvlMinimaBase::org;

shared_ptr<vincent::PunktList<vincent::lab_value> > GLvlMinimaBase::plist;
bool GLvlMinimaBase::incl_wshed=false;

void GLvlMinimaBase::setup(const vincent::transform &transform,boost::shared_ptr< vincent::Bild_vimage<vincent::lab_value>  > img,VImage _org)
{
	GLvlMinimaBase::img=img;
	GLvlMinimaBase::plist=transform.getVoxels(*img);
	org= shared_ptr< vincent::Bild_vimage<VUByte> >(new vincent::Bild_vimage<VUByte>(_org));
}

