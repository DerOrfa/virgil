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

void GLvlMinimaTex::setup(boost::shared_ptr< vincent::Bild_vimage<vincent::lab_value>  > img)
{
	GLvlMinimaBase::setup(img);
}

/*!
    \fn GLvlMinimaTex::genTex()
 */
shared_ptr<Bild_mem<VBit> > GLvlMinimaTex::genTex()
{
	Bild_mem<VBit> *ret = new Bild_mem<VBit>(maxEdge.x-minEdge.x+1,maxEdge.y-minEdge.y+1,maxEdge.z-minEdge.z+1,false);
	for(unsigned int i=GLvlMinimaBase::start;i<GLvlMinimaBase::maxEdge;i++)
	{
		const vincent::iPunkt<vincent::lab_value> p=(*GLvlMinimaBase::plist)[i];
		const unsigned short x=p.x(GLvlMinimaBase::img->xsize)-minEdge.x;
		const unsigned short y=p.x(GLvlMinimaBase::img->ysize)-minEdge.y;
		const unsigned short z=p.x(GLvlMinimaBase::img->zsize)-minEdge.z;
		((Bild<VBit> *)ret)->at(x,y,z)=true;
	}
	return shared_ptr<Bild_mem<VBit> >(ret);
}
