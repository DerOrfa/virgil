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
#include <libsgl/sglsignal.h>
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
	class redisplaySlot:public SGLSlot
	{
	public:
		 GLvlSegment &myseg;
		redisplaySlot(GLvlSegment &_myseg);
		void operator()();
	};
protected:
	union EdgeData{
		struct {unsigned short x,y,z;};
		unsigned short koord[3];
	}minEdge,maxEdge;
	boost::shared_ptr<GLvlVolumeTex> myTex;
public:
	GLvlSegment(const GLvlSegment &);
	GLvlSegment(unsigned int index);
	GLvlSegment(boost::shared_ptr<GLvlMinima> img);
	virtual ~GLvlSegment();
	static void setup(SGLqtSpace *_target3D,boost::shared_ptr<GLvlVolumeTex> _targetTex);
	bool display(bool incl3D);
	void undisplay(bool incl3D);
	void redisplay(bool incl3D);
	void getDim(dim &X,dim &Y, dim &Z);
	void getOffset(unsigned short offset[3],GLvlSegment::iterator i);

	static boost::shared_ptr<GLvlVolumeTex> targetTex;
	static SGLqtSpace *target3D;
	
	bool isMinima;
};

#endif
