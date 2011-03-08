/***************************************************************************
 *   Copyright (C) 2004 by Enrico Reimer, 1.01.2005,hayd,,@[e/ea],-131.-   *
 *   reimer@santiago                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef BILD_H
#define BILD_H

#include <assert.h>
#include <limits.h>
#include <libsgl/sglmisc.h>
#include <math.h>
#include <typeinfo>
#include <libsgl/sglshptr.h>
#include <limits>

class dim
{
	unsigned short cnt;
	float Elsize;
public:
	inline operator unsigned short()const{return cnt;}
	inline double mm_size(const unsigned short div){return (idx2mm(cnt/div));}
	inline double minus_mm_size(const unsigned short div){return (idx2mm(cnt/div-cnt));}
	inline double idx2mm(const unsigned short tex_koord){return Elsize*tex_koord;}
	inline unsigned short mm2idx(const double tex_koord){
		const double ret=rint(tex_koord/Elsize);
		return ret<0 ? std::numeric_limits<unsigned short>::max():(unsigned short)ret;
	}
	inline unsigned short getCnt(char dir){
		if(cnt==0)
		{SGLprintError("Der Datensatz hat keine Dimension in %c-Richtung. Darstellung ist nicht möglich.",dir);abort();}
		else return cnt;
	}
	inline unsigned short setCnt(unsigned short c){return cnt=c;}
	inline float& ElsizeRef(){return Elsize;};
	inline float getElsize(char dir){
		if(Elsize==0)
		{SGLprintWarning("Die Voxel des Datensatzes haben keine Dimension in %c-Richtung, nehme 1mm an",dir);Elsize=1;}
		return Elsize;
	}
	inline float setElsize(float s){return Elsize=s;}
};

template <class T> class Bild
{
public:
	union {dim xsize;dim Columns;};
	union {dim ysize;dim Rows;};
	union {dim zsize;dim Bands;dim layer;};
	inline unsigned int size()const{return xsize*ysize*zsize;}
	struct ValRange{T min,max;};
/*private:
	ValRange range;*/
public:
/*	ValRange getValRange()
	{
		std::less<T> l;
		if(l(range.max, range.min))
		{
			SGLprintState("Scanning %s data",typeid(T).name());
			for(unsigned int i=0;i<this->size();i++)
			{
				const T& dummy=at(i);
				if(dummy>range.max)range.max=dummy;
				if(dummy<range.min)range.min=dummy;
			}
			SGLprintState("max: %f, min %f",range.max,range.min);
		}
		return range;
	}*/
	Bild(unsigned short x,unsigned short y,unsigned short z)
	{
		xsize.setCnt(x);
		ysize.setCnt(y);
		zsize.setCnt(z);
		xsize.setElsize(0);
		ysize.setElsize(0);
		zsize.setElsize(0);
/*		range.max=numeric_limits<T>::min();
		range.min=numeric_limits<T>::max();*/
	}
	virtual ~Bild(){}
	inline virtual T &at(const unsigned int index)=0;
	inline T &at(const short x,const unsigned short y,const unsigned short z){
		return at(x+(y*xsize)+(z*xsize*ysize));
	}
	inline T *copy_line(const unsigned short y,const unsigned short z,void *dst){
		return ((T*)memcpy(dst,&at(0,y,z),xsize*sizeof(T)))+xsize;
	}
};

template <class T> class Bild_mem:public Bild<T>
{
protected:
	T *data;
public:
	void reinit(unsigned short x,unsigned short y,unsigned short z,T initVal)
	{
		this->xsize.cnt=x;
		this->ysize.cnt=y;
		this->zsize.cnt=z;
		if(data)free(data);
		init(initVal);
	}
	void init(T initVal)
	{
		if(initVal==0)data=(T*)calloc(this->size(),sizeof(T));
		else
		{
			data=(T*)malloc(this->size()*sizeof(T));
			for(int i=this->size()-1;i>=0;i--)data[i]=initVal;
		}
	}
	Bild_mem(unsigned short x,unsigned short y,unsigned short z,T initVal):Bild<T>(x,y,z){init(initVal);}
	virtual ~Bild_mem(){free(data);}
	inline T &at(const unsigned int index){return data[index];}
};

#endif
