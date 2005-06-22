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
#include <libsgl/sglsignal.h>
#include "glvlsegment.h"
#include <libsgl/sglqtspace.h>
#include "glvlvolumetex.h"
#include "glvlminima.h"

GLvlSegment::GLvlSegment(const GLvlSegment &)
{
	cout << "Kopiere Segment" << endl;
	abort();
}


GLvlSegment::GLvlSegment(SGLshPtr<GLvlMinima> img):isMinima(true)
{
	push_back(img);
}

GLvlSegment::GLvlSegment(unsigned int index):isMinima(true)
{
	push_back(SGLshPtr<GLvlMinima>(new GLvlMinima(index)));
}
GLvlSegment::~GLvlSegment()
{
	undisplay();//@todo eigentlich nicht nett - es könnte ja sein, daß Teilevon ihm gerade von anderen Angez werden
}

bool GLvlSegment::display()
{
	for(GLvlSegment::iterator i=begin();i!=end();i++)
		(*i)->show(*target3D,*this,*i);
	
	EVektor<unsigned short> pos;
	myTex=SGLshPtr<GLvlVolumeTex>(new GLvlVolumeTex());
	myTex->renderMode=SGL_MTEX_MODE_COLORMASK;
	
	myTex->loadSegment(*this);
	myTex->envColor[0]=0;
	myTex->envColor[1]=isMinima ? 1:0;
	myTex->envColor[2]=isMinima ? 0:1;
	SGLVektor offset(myTex->Info.X.getElsize('X'),myTex->Info.Y.getElsize('Y'),myTex->Info.Z.getElsize('Z'));
	pos.fromArray(3,minEdge.koord);
	myTex->calcMatr(offset.linearprod(pos));
	myTex->ResetTransformMatrix((const GLdouble*)myTex->mm2tex_Matrix);
	myTex->weich=false;
	/*if(isMinima)*/targetTex->addMTexEnd(myTex,true);
//	else targetTex->addMTexBegin(myTex,true);
	return true;
}
void GLvlSegment::undisplay()
{
	if(myTex)targetTex->delMTex(myTex,true);
	myTex=SGLshPtr<GLvlVolumeTex>();
	for(GLvlSegment::iterator i=begin();i!=end();i++)
		(*i)->unshow(*target3D,*this,*i);
}

void GLvlSegment::setup(SGLqtSpace *_target3D,SGLshPtr<GLvlVolumeTex> _targetTex)
{
	target3D=_target3D;
	targetTex=_targetTex;
}

void GLvlSegment::getOffset(unsigned short offset[3],GLvlSegment::iterator i)
{
	assert((*i)->minEdge.x-minEdge.x >= 0);
	assert((*i)->minEdge.y-minEdge.y >= 0);
	assert((*i)->minEdge.z-minEdge.z >= 0);

	offset[0]+= ((*i)->minEdge.x-minEdge.x);
	offset[1]+= ((*i)->minEdge.y-minEdge.y);
	offset[2]+= ((*i)->minEdge.z-minEdge.z);
}

void GLvlSegment::getDim(dim &X,dim &Y, dim &Z)
{
	X.setElsize(GLvlMinima::img->xsize.getElsize('X'));
	Y.setElsize(GLvlMinima::img->ysize.getElsize('Y'));
	Z.setElsize(GLvlMinima::img->zsize.getElsize('Z'));

	minEdge.x=minEdge.y=minEdge.z=numeric_limits<unsigned short>::max();
	maxEdge.x=maxEdge.y=maxEdge.z=numeric_limits<unsigned short>::min();

	for(GLvlSegment::iterator i=begin();i!=end();i++)
	{
		minEdge.x = std::min((*i)->minEdge.x,minEdge.x);
		minEdge.y = std::min((*i)->minEdge.y,minEdge.y);
		minEdge.z = std::min((*i)->minEdge.z,minEdge.z);

		maxEdge.x = std::max((*i)->maxEdge.x,maxEdge.x);
		maxEdge.y = std::max((*i)->maxEdge.y,maxEdge.x);
		maxEdge.z = std::max((*i)->maxEdge.z,maxEdge.x);
	}
	X.setCnt(maxEdge.x-minEdge.x+1);
	Y.setCnt(maxEdge.y-minEdge.y+1);
	Z.setCnt(maxEdge.z-minEdge.z+1);
}



SGLshPtr<GLvlVolumeTex> GLvlSegment::targetTex;
SGLqtSpace *GLvlSegment::target3D;

void GLvlSegment::redisplay()
{
	EVektor<unsigned short> pos;
	
	if(myTex)for(GLvlSegment::iterator i=begin();i!=end();i++)
		(*i)->reshow(*target3D,*this,*i);
	
	myTex->loadSegment(*this);
	SGLVektor offset(myTex->Info.X.getElsize('X'),myTex->Info.Y.getElsize('Y'),myTex->Info.Z.getElsize('Z'));
	pos.fromArray(3,minEdge.koord);//Achtung minEdge.koord darf erst nach loadSegment verwendet werden
	myTex->calcMatr(offset.linearprod(pos));
	myTex->ResetTransformMatrix((const GLdouble*)myTex->mm2tex_Matrix);
	GLvlSegment::targetTex->changed();
}
