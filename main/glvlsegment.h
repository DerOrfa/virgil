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

#include <libsgl/sglqtspace.h>
#include "glvlminima.h"

class GLvlVolumeTex;

/**
@author Enrico Reimer,,,
*/

class GLvlSegment: public list< shared_ptr<GLvlMinima> >
{
protected:
	GLvlMinima::EdgeData minEdge,maxEdge;
	static boost::shared_ptr<GLvlVolumeTex> targetTex;
	static SGLqtSpace *target3D;
	static boost::shared_ptr<GLvlVolumeTex> mTexDummy;
	boost::shared_ptr<GLvlVolumeTex> myTex;
public:
	GLvlSegment(unsigned int index);
	GLvlSegment();
	GLvlSegment(shared_ptr<GLvlMinima> img);
	static void setup(SGLqtSpace *_target3D,boost::shared_ptr<GLvlVolumeTex> _targetTex);
	bool display();
	void redisplay();
	void undisplay();
	void getDim(dim &X,dim &Y, dim &Z);
	void getOffset(unsigned short offset[3],GLvlSegment::iterator i);
};

#include "glvlvolumetex.h"
#endif
