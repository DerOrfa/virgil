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

#include <boost/shared_ptr.hpp>
#include <list>
#include "bild.h"

class GLvlVolumeTex;
class SGLqtSpace;
class GLvlMinima;
/**
@author Enrico Reimer,,,
*/

class GLvlSegment: public std::list< boost::shared_ptr<GLvlMinima> >
{
protected:
	union EdgeData{
		struct {unsigned short x,y,z;};
		unsigned short koord[3];
	}minEdge,maxEdge;
	static boost::shared_ptr<GLvlVolumeTex> mTexDummy;
	boost::shared_ptr<GLvlVolumeTex> myTex;
public:
	GLvlSegment(unsigned int index);
	GLvlSegment();
	GLvlSegment(boost::shared_ptr<GLvlMinima> img);
	static void setup(SGLqtSpace *_target3D,boost::shared_ptr<GLvlVolumeTex> _targetTex);
	bool display();
	void redisplay();
	void undisplay();
	void getDim(dim &X,dim &Y, dim &Z);
	void getOffset(unsigned short offset[3],GLvlSegment::iterator i);

	static boost::shared_ptr<GLvlVolumeTex> targetTex;
	static SGLqtSpace *target3D;
};

#endif
