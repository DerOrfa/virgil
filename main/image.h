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
#ifndef IMAGE_H
#define IMAGE_H

#include <GL/gl.h>
#include "bild.h"
#include "pixel.h"
#include <algorithm>
#include <functional>



/**
@author Enrico Reimer, 1.01.2005,hayd,,@[e/ea],-131.-221. 143
*/

template<class T> class gPunkt
{
	public:
	unsigned int pos;
	T wert;
};

template <class T> class sort_q:
public binary_function< gPunkt<T>, gPunkt<T>, bool>
{
public:
	inline bool operator()(gPunkt<T> p1,gPunkt<T> p2)
	{
		return p1.wert <p2.wert;
	}
};

template <class T> class sort_f:
public binary_function<unsigned int,unsigned int,bool>
{
	Bild_vimage<T> *im;
	PunktList *D;
public:
	sort_f(Bild_vimage<T> *_im,PunktList *_D):im(_im),D(_D){}
	inline bool operator()(unsigned int p1,unsigned int p2)
	{
		const T farbe1=im->at(PunktRef::pos2xy(p1,D->size_x*D->size_y),PunktRef::pos2z(p1,D->size_x*D->size_y));
		const T farbe2=im->at(PunktRef::pos2xy(p2,D->size_x*D->size_y),PunktRef::pos2z(p2,D->size_x*D->size_y));
		return farbe1<farbe2;
	}
};


template<class T> class ws_image{
	Bild_vimage<T> im;
public:
	PunktList D;
	ws_image(VImage src):im(src),D(im.xsize,im.ysize,im.zsize)
	{
		printf("Lege %d Punkte an\n",im.size());
	}
	void test()	{
		printf("Sortiere\n");
		sort2();
		printf("fertsch\n");
		PunktRef r=D[5000];
		Punkt l[6];
		unsigned short len=r.getNachb(l);
		for(int i=0;i<len;i++)
			printf("%d-%d-%d:%d\n",l[i].posx,l[i].posy,l[i].posz,im[l[i]]);
	}
	unsigned int* sort()
	{
		sort_f<T> comp(&im,&D);
		const unsigned int N = im.size();
		std::sort(D.m, D.m + N,comp);
		return D.m;
	}
	unsigned int* sort2()
	{
		unsigned int s=im.size();
		gPunkt<T> *pkts=new gPunkt<T>[s];
		for(int i=0;i<s;i++)
		{
			pkts[i].wert=im.at(PunktRef::pos2xy(i,D.size_x*D.size_y),PunktRef::pos2z(i,D.size_x*D.size_y));
			pkts[i].pos=i;
		}
		sort_q<T> comp;
		std::sort(pkts, pkts + im.size(),comp);
		for(int i=0;i<s;i++)
		{
			D.m[i]=pkts[i].pos;
		}
		delete pkts;
	}
};

#endif
