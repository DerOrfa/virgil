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

/**
@author Enrico Reimer,,,
*/
class GLvlVolumeTex;
class GLvlMinimaList;

class Segment: public list< shared_ptr<GLvlMinima> >
{
protected:
	static boost::shared_ptr<GLvlVolumeTex> targetTex;
	static SGLqtSpace *target3D;
	static boost::shared_ptr<GLvlVolumeTex> mTexDummy;
public:
	static void setup(SGLqtSpace *_target3D,boost::shared_ptr<GLvlVolumeTex> _targetTex);
	void showDummy();
	virtual void display()=0;
	virtual void undisplay()=0;
};

#endif
