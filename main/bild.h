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

#include <vista/VImage.h>
#include <assert.h>
#include <limits.h>
#include <sglmisc.h>
#include <math.h>

class dim
{
public:
	unsigned short cnt;
	float Elsize;
	inline operator unsigned short()const{return cnt;}
		
		inline double mm_size(const unsigned short div){return (idx2mm(cnt/div));}
		inline double minus_mm_size(const unsigned short div){return (idx2mm(cnt/div-cnt));}
		inline double idx2mm(const unsigned short tex_koord){return Elsize*tex_koord;}
		inline unsigned short mm2idx(const double tex_koord){return (unsigned short)rint(tex_koord/Elsize);}
};

template <class T> class Bild
{
public:
	union {dim xsize;dim Columns;};
	union {dim ysize;dim Rows;};
	union {dim zsize;dim Bands;dim layer;};
	inline unsigned int size()const{return xsize*ysize*zsize;} 
	Bild(unsigned short x,unsigned short y,unsigned short z)
	{
		xsize.cnt=x;
		ysize.cnt=y;
		zsize.cnt=z;
		xsize.Elsize=ysize.Elsize=zsize.Elsize=0;
	}
	inline T &at(const unsigned short x,const unsigned short y,const unsigned short z){
		return at(x+(y*xsize)+(z*xsize*ysize));
	}
	inline T *copy_line(const unsigned short y,const unsigned short z,void *dst){
		return ((T*)memcpy(dst,&at(0,y,z),xsize*sizeof(T)))+xsize;
	}
	
	inline virtual T &at(const unsigned int index)=0;
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

template <class T> class Bild_vimage : public Bild<T>
{
	VImage img;//@todo was is, wenn die Umgebung das Img löscht
	VPointer data;
	int lastBand;
	public:
	inline void reset(T value)
	{
		for(int i=this->size()-1;i>=0;i--)at(i)=value;
	}
	Bild_vimage(VImage _img):
		Bild<T>(VImageNColumns(_img),VImageNRows(_img),VImageNBands(_img)),img(_img),
		lastBand(std::numeric_limits<int>::min())
	{
		int pixMax;
		char *AttrStr;

		if(VGetAttr(VImageAttrList(_img),"voxel",NULL,VStringRepn,(VPointer)&AttrStr)==VAttrFound)
			sscanf(AttrStr,"%f %f %f",&this->Columns.Elsize,&this->Rows.Elsize,&this->Bands.Elsize);//@todo  stimmt das so ? wert1 breite der Spalten wert2 dicke der Zeilen wert3 dicke der schichten
		VSelectBand("Vol2Tex",img,-1,&pixMax,&data);
	}

	inline T &at(unsigned int pos){return ((T*)data)[pos];}
	inline T at(unsigned int pos)const{return ((T*)data)[pos];}
	VImage src()const{return img;}
	VImage &src(){return img;}
};
#endif
