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
#include <libsgl/sglqtspace.h>
#include "glvlvolumetex.h"
#include "glvlminima.h"

GLvlSegment::GLvlSegment(){};
GLvlSegment::GLvlSegment(boost::shared_ptr<GLvlMinima> img)
{
	push_back(img);
}

GLvlSegment::GLvlSegment(unsigned int index)
{
	push_back(boost::shared_ptr<GLvlMinima>(new GLvlMinima(index)));
}

void GLvlSegment::redisplay()
{
	EVektor<unsigned short> pos;
	pos.fromArray(3,minEdge.koord);
	
	for(GLvlSegment::iterator i=begin();i!=end();i++)
		(*i)->compileNextTime();
	
	myTex->loadSegment(*this);
	myTex->calcMatr(SGLVektor(myTex->Info.X.getElsize('X'),myTex->Info.Y.getElsize('Y'),myTex->Info.Z.getElsize('Z')).linearprod(pos));
	myTex->ResetTransformMatrix((const GLdouble*)myTex->mm2tex_Matrix);
	targetTex->changed();
}

bool GLvlSegment::display()
{
	for(GLvlSegment::iterator i=begin();i!=end();i++)
		if((*i)->size() <= MAX_MINIMA_SIZE)
			target3D->showObj(*i);
	
	EVektor<unsigned short> pos;
	myTex=boost::shared_ptr<GLvlVolumeTex>(new GLvlVolumeTex());
	myTex->renderMode=SGL_MTEX_MODE_COLORMASK;
	
	myTex->loadSegment(*this);
	myTex->envColor[0]=0;
	myTex->envColor[1]=1;
	myTex->envColor[2]=0;
	SGLVektor offset(myTex->Info.X.getElsize('X'),myTex->Info.Y.getElsize('Y'),myTex->Info.Z.getElsize('Z'));
	pos.fromArray(3,minEdge.koord);
	myTex->calcMatr(offset.linearprod(pos));
	myTex->ResetTransformMatrix((const GLdouble*)myTex->mm2tex_Matrix);
	myTex->weich=false;
	targetTex->addMTexEnd(myTex,true);
	return true;
}
void GLvlSegment::undisplay()
{
	if(myTex)targetTex->delMTex(myTex,true);
	myTex=boost::shared_ptr<GLvlVolumeTex>();
	for(GLvlSegment::iterator i=begin();i!=end();i++)
		target3D->unshowObj(*i);
}

void GLvlSegment::setup(SGLqtSpace *_target3D,boost::shared_ptr<GLvlVolumeTex> _targetTex)
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
                minEdge.x = minEdge.x <? (*i)->minEdge.x;
                minEdge.y = minEdge.y <? (*i)->minEdge.y;
                minEdge.z = minEdge.z <? (*i)->minEdge.z;
        
                maxEdge.x = maxEdge.x >? (*i)->maxEdge.x;
                maxEdge.y = maxEdge.y >? (*i)->maxEdge.y;
                maxEdge.z = maxEdge.z >? (*i)->maxEdge.z;
        }
        X.setCnt(maxEdge.x-minEdge.x+1);
        Y.setCnt(maxEdge.y-minEdge.y+1);
        Z.setCnt(maxEdge.z-minEdge.z+1);
}



boost::shared_ptr<GLvlVolumeTex> GLvlSegment::targetTex;
SGLqtSpace *GLvlSegment::target3D;
boost::shared_ptr<GLvlVolumeTex> GLvlSegment::mTexDummy;
