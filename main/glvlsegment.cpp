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
#include "glvlsegment.h"

GLvlSegment::GLvlSegment(){};
GLvlSegment::GLvlSegment(shared_ptr<GLvlMinima> img)
{
	push_back(img);
}


void Segment::display()
{
/*	assert(targetTex->multitex);
	EVektor<unsigned short> pos;
	pos.fromArray(3,minEdge.koord);
	boost::shared_ptr<GLvlVolumeTex> p(new GLvlVolumeTex());
	p->renderMode=SGL_MTEX_MODE_COLORMASK;
	
	GLvlMinimaList t(this);
	p->loadMinimaMask(t);
	p->envColor[0]=0;
	p->envColor[1]=1;
	p->envColor[2]=0;
	p->calcMatr(SGLVektor(p->Info.X.getElsize('X'),p->Info.Y.getElsize('Y'),p->Info.Z.getElsize('Z')).linearprod(pos));
	p->ResetTransformMatrix((const GLdouble*)p->mm2tex_Matrix);
	p->weich=false;
	targetTex->multitex->multitex=p;
	targetTex->changed();*/
}
void Segment::undisplay()
{
	targetTex->multitex->multitex=boost::shared_ptr<SGLBaseTex>();
	targetTex->changed();
}
void Segment::showDummy()
{
	boost::shared_ptr<SGLBaseTex> temp;
	if(targetTex->multitex)temp=targetTex->multitex->multitex;
	targetTex->multitex=mTexDummy;
	targetTex->multitex->multitex=temp;
}

void Segment::setup(SGLqtSpace *_target3D,boost::shared_ptr<GLvlVolumeTex> _targetTex)
{
	target3D=_target3D;
	targetTex=_targetTex;
	
	mTexDummy =boost::shared_ptr<GLvlVolumeTex> (new GLvlVolumeTex());
	mTexDummy->renderMode=SGL_MTEX_MODE_COLORMASK;
	
	Segment emptySegment;
	mTexDummy->loadMinimaMask(emptyLst);
}

boost::shared_ptr<GLvlVolumeTex> Segment::targetTex;
SGLqtSpace *Segment::target3D;
boost::shared_ptr<GLvlVolumeTex> Segment::mTexDummy;
