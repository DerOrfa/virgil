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
#include <GL/glu.h>

#define MAX_MINIMA_SIZE 500000

/**
@author Enrico Reimer,,,
*/
class GLvlMinimaBase
{
public:
	union EdgeData{
		struct {unsigned short x,y,z;};
		unsigned short koord[3];
	}minEdge,maxEdge;
	static shared_ptr< vincent::Bild_vimage<vincent::lab_value> > img;
	static shared_ptr< vincent::Bild_vimage<VUByte> > org;
	VUByte bottomCap,topCap;
	
	unsigned int start,end;
	static shared_ptr< vincent::PunktList<vincent::lab_value> > plist;
	GLvlMinimaBase(unsigned int pos);
	static void setup(const vincent::transform &transform,boost::shared_ptr< vincent::Bild_vimage<vincent::lab_value>  > img,VImage _org);
	static bool incl_wshed;
	inline const unsigned int size(){return end-start;}
    void chCap(short topdelta,short bottomdelta);
	dim getXDim()const;
	dim getYDim()const;
	dim getZDim()const;
	template <class T> inline static void writeEdgeData(const vincent::iPunkt<T> &p, EdgeData &min,EdgeData &max)
	{
		const unsigned short x=p.x(GLvlMinimaBase::img->xsize);
		const unsigned short y=p.y(GLvlMinimaBase::img->xsize,GLvlMinimaBase::img->ysize);
		const unsigned short z=p.z(GLvlMinimaBase::img->xsize,GLvlMinimaBase::img->ysize);
			
		min.x = min.x <? x;
		min.y = min.y <? y;
		min.z = min.z <? z;
	
		max.x = max.x >? x;
		max.y = max.y >? y;
		max.z = max.z >? z;
	}
};

#endif
