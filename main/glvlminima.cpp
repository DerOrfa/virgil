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
#include "glvlminima.h"
#include <sglvektor.h>

GLvlMinimaBase::GLvlMinimaBase(unsigned int pos):start(pos)
{
	minEdge.x=minEdge.y=minEdge.z=numeric_limits<unsigned short>::max();
	maxEdge.x=maxEdge.y=maxEdge.z=numeric_limits<unsigned short>::min();
	if(img)
	{
		end=start;
		vincent::iPunkt<vincent::lab_value> p=(*plist)[start];
		vincent::lab_value ID=p.wert;
		bottomCap = (*org)[p];
		do 
		{
			writeEdgeData(p,minEdge,maxEdge);
			vincent::iPunkt<vincent::lab_value>  nachb[6];
			const unsigned short nachb_cnt=p.getNachb(nachb,*img);
			for(unsigned short i=0;i<nachb_cnt;i++)
				if(nachb[i].wert==vincent::WSHED_WSHED && incl_wshed)
					writeEdgeData(nachb[i],minEdge,maxEdge);

			p=(*plist)[++end]; 
		}
		while(end<plist->size && p.wert==ID);
			
		topCap = (*org)[(*plist)[end-1]];
		//getPixel verwendet stable_sort -> die Punkte eines Objektes sind noch der Größe nach geordnet
	}
	else{SGLprintError("GLvlMinima::setup wurde nich ausgeführt, das Objekt kann nicht angelegt werden");abort();}
}

shared_ptr<vincent::Bild_vimage<vincent::lab_value> > GLvlMinimaBase::img;
shared_ptr< vincent::Bild_vimage<VUByte> > GLvlMinimaBase::org;

shared_ptr<vincent::PunktList<vincent::lab_value> > GLvlMinimaBase::plist;
bool GLvlMinimaBase::incl_wshed=true;

void GLvlMinimaBase::setup(const vincent::transform &transform,boost::shared_ptr< vincent::Bild_vimage<vincent::lab_value>  > img,VImage _org)
{
	GLvlMinimaBase::img=img;
	GLvlMinimaBase::plist=transform.getVoxels(*img);
	org= shared_ptr< vincent::Bild_vimage<VUByte> >(new vincent::Bild_vimage<VUByte>(_org));
}

void GLvlMinimaBase::chCap(short Top_delta,short Bottom_delta)
{
	bool update;
	const VUByte bottomBorder = (*org)[(*plist)[start]],topBorder = (*org)[(*plist)[end-1]];
	if(Top_delta==numeric_limits<short>::max())
	{
		if(this->topCap!=topBorder)
		{this->topCap=topBorder;update=true;}
	}
	else if(Top_delta>0)
	{
		if(this->topCap >=topBorder)
		{
			if(!incl_wshed){incl_wshed=true;update =true;}
		}
		else {this->topCap++;update =true;}
	}
	else if(Top_delta<0)
	{
		if(incl_wshed){incl_wshed=false;update =true;}
		else if(this->topCap>this->bottomCap){this->topCap--;update =true;}
	}
	
	if(Bottom_delta==numeric_limits<short>::min())
	{
		if(this->bottomCap!=bottomBorder)
		{this->bottomCap=bottomBorder;update=true;}
	}
	if(Bottom_delta>0)
	{
		if(this->bottomCap <this->topCap)
		{this->bottomCap++;update =true;}
	}
	else if(Bottom_delta<0)
	{
		if(this->bottomCap > bottomBorder)
		{this->bottomCap--;update =true;}
	}
	
}

dim GLvlMinimaBase::getXDim()const
{
	dim ret;
	assert(maxEdge.x-minEdge.x+1);
	ret.setCnt(maxEdge.x-minEdge.x+1);
	ret.setElsize(GLvlMinimaBase::img->xsize.getElsize('X'));
	return ret;
}
dim GLvlMinimaBase::getYDim()const
{
	dim ret;
	assert(maxEdge.y-minEdge.y+1);
	ret.setCnt(maxEdge.y-minEdge.y+1);
	ret.setElsize(GLvlMinimaBase::img->ysize.getElsize('Y'));
	return ret;
}
dim GLvlMinimaBase::getZDim()const
{
	dim ret;
	assert(maxEdge.z-minEdge.z+1);
	ret.setCnt(maxEdge.z-minEdge.z+1);
	ret.setElsize(GLvlMinimaBase::img->zsize.getElsize('Z'));
	return ret;
}
