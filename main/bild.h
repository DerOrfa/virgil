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
#include "pixel.h"

template <class T> class Bild
{
	public:
	unsigned int xsize,ysize,zsize;
	inline unsigned int size(){return xsize*ysize*zsize;}
	Bild(unsigned short x,unsigned short y,unsigned short z):xsize(x),ysize(y),zsize(z){}
};


template <class T> class Bild_mem:Bild<T>
{
	T *data;
	public:
	Bild_mem(unsigned short x,unsigned short y,unsigned short z,T initVal):Bild<T>(x,y,z)
	{
		printf("Erzeuge Datenpuffer %d*%d*%d=%d\n",xsize,ysize,zsize,size());
		if(initVal==0)data=(T*)calloc(size(),sizeof(T));
		else 
		{
			data=(T*)malloc(size()*sizeof(T));
			for(int i=size()-1;i>=0;i--)data[i]=initVal;
		}
	}
	~Bild_mem(){free(data);}
	inline T &operator[](Punkt &p){return data[p.posx+p.posy*xsize+p.posz*xsize*ysize];}
	inline T &operator[](PunktRef &p){return operator[](*p);}
};

template <class T> class Bild_vimage : public Bild<T>
{
	VImage img;//@todo was is, wenn die Umgebung das Img l�scht
	VPointer data;
	int lastBand;
	public:
	Bild_vimage(VImage _img):
		Bild<T>(VImageNRows(_img),VImageNColumns(_img),VImageNBands(_img)),img(_img),
		lastBand(-1){}
	inline T &operator[](Punkt &p)
	{
		int pixMax;
		if(lastBand!=p.posz)VSelectBand("Vol2Tex",img,p.posz,&pixMax,&data);
		assert(pixMax==xsize*ysize*zsize);
		return ((T*)data)[p.posx+p.posy*xsize];
	}
	inline T &operator[](PunktRef &p){Punkt ret(p); return operator[](ret);}
};

#endif
