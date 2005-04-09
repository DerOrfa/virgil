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

#include <libsgl/sglsignal.h>
#include <libsgl/primitives/sglflobj.h>
#include "../wshed/vincent_transform.h"
#include "../wshed/vincent_punkt.h"
#include <GL/glu.h>

#include <libsgl/sglqtspace.h>

#define MAX_MINIMA_SIZE 500000

#include <set>

class GLvlSegment;

/**
@author Enrico Reimer,,,
*/

class GLvlMinima:public SGLFlObj
{
	static GLuint caps;
	static SGLVektor scale;
	set<GLvlSegment*> users;
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
	inline VUByte GLvlMinima::bottomBorder()const{return (*org)[(*plist)[start]];};
	inline VUByte GLvlMinima::topBorder()const{return (*org)[(*plist)[end-1]];};

	static boost::shared_ptr< vincent::Bild_vimage<vincent::lab_value> > img;
	static boost::shared_ptr< vincent::Bild_vimage<VUByte> > org;
	VUByte bottomCap,topCap;
	
	
	unsigned int start,end;
	static boost::shared_ptr< vincent::PunktList<vincent::lab_value> > plist;
	GLvlMinima(unsigned int pos);
	bool incl_wshed;
	inline unsigned int volume()const{return end-start;}
	inline double volume_mm()const{
		return volume()*
		GLvlMinima::img->xsize.getElsize('X')*
		GLvlMinima::img->ysize.getElsize('Y')*
		GLvlMinima::img->zsize.getElsize('Z');
	}
    bool chCapRel(signed char topdelta,signed char bottomdelta);
    bool chCapAbs(VUByte top,VUByte bottom);
    bool chCapAbsTop(VUByte top);
    bool chCapAbsBottom(VUByte bottom);
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
	SGLVektor getCenter()const;
	void writeTex(const unsigned short offset[3],Bild<GLubyte> &textur)const;
	void getPktKoord(const unsigned int indexRel,unsigned short &x,unsigned short &y,unsigned short &z)const;
	vincent::lab_value getID()const;
	GLvlMinima(const GLvlMinima&);
	void reshow(SGLqtSpace &space,GLvlSegment &seg,const shared_obj &self);
	void show(SGLqtSpace &space,GLvlSegment &seg,const shared_obj &self);
	void unshow(SGLqtSpace &space,GLvlSegment &seg,const shared_obj &self);
	void update_users();
};

#endif
