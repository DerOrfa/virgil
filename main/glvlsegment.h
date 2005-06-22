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
#ifndef GLVLSEGMENT_H
#define GLVLSEGMENT_H

#include <libsgl/sglsignal.h>
#include <list>
#include "bild.h"

class GLvlVolumeTex;
class SGLqtSpace;
class GLvlMinima;
/**
@author Enrico Reimer,,,
*/

class GLvlSegment: public std::list< SGLshPtr<GLvlMinima> >
{
protected:
	union EdgeData{
		struct {unsigned short x,y,z;};
		unsigned short koord[3];
	}minEdge,maxEdge;
	SGLshPtr<GLvlVolumeTex> myTex;
public:
	GLvlSegment(const GLvlSegment &);
	GLvlSegment(unsigned int index);
	GLvlSegment(SGLshPtr<GLvlMinima> img);
	virtual ~GLvlSegment();
	static void setup(SGLqtSpace *_target3D,SGLshPtr<GLvlVolumeTex> _targetTex);
	bool display();
	void undisplay();
	void redisplay();
	void getDim(dim &X,dim &Y, dim &Z);
	void getOffset(unsigned short offset[3],GLvlSegment::iterator i);

	static SGLshPtr<GLvlVolumeTex> targetTex;
	static SGLqtSpace *target3D;
	
	bool isMinima;
};

#endif
