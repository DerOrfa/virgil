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

#include <libsgl/primitives/sglflobj.h>
#include "../wshed/vincent_transform.h"
#include "../wshed/vincent_punkt.h"
#include <GL/glu.h>

#include <libsgl/sglqtspace.h>

#define MAX_MINIMA_SIZE 500000

/**
@author Enrico Reimer,,,
*/

class GLvlMinima:public SGLFlObj
{
	static GLuint caps;
	static SGLVektor scale;
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
	GLvlMinima(unsigned int pos);
	static void setup(const vincent::transform &transform,boost::shared_ptr< vincent::Bild_vimage<vincent::lab_value>  > img,VImage _org);
	bool incl_wshed;
	inline const unsigned int size(){return end-start;}
    void chCap(short topdelta,short bottomdelta);
	dim getXDim()const;
	dim getYDim()const;
	dim getZDim()const;
	template <class T> inline static void writeEdgeData(const vincent::iPunkt<T> &p, EdgeData &min,EdgeData &max)
	{
		const unsigned short x=p.x(GLvlMinima::img->xsize);
		const unsigned short y=p.y(GLvlMinima::img->xsize,GLvlMinima::img->ysize);
		const unsigned short z=p.z(GLvlMinima::img->xsize,GLvlMinima::img->ysize);
			
		min.x = min.x <? x;
		min.y = min.y <? y;
		min.z = min.z <? z;
	
		max.x = max.x >? x;
		max.y = max.y >? y;
		max.z = max.z >? z;
	}
	void generate();
	static void setup(
		SGLVektor norm,
		const vincent::transform &transform,
		VImage &src
	);
    SGLVektor getCenter();
    shared_ptr<Bild_mem<VBit> > genTex();
	void writeTex(const unsigned short offset[3],Bild<GLubyte> &textur)const;
};

//#include "glvlvolumetex.h"
#endif
