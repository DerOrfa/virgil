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

template <class T> class sort_f:
public binary_function<unsigned int,unsigned int,bool>
{
	Bild_vimage<T> *im;
	PunktList *D;
public:
	sort_f(Bild_vimage<T> *_im,PunktList *_D):im(_im),D(_D){}
	bool operator()(unsigned int p1,unsigned int p2)
	{
		Punkt P1(D->operator [ ](p1)),P2(D->operator [ ](p2));
		T farbe1=im->operator [ ](P1);
		T farbe2=im->operator [ ](P2);
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
		PunktRef r=D[0];
		for(unsigned short z=0;z<im.zsize;z++)
			for(unsigned short y=0;y<im.ysize;y++)
				for(unsigned short x=0;x<im.xsize;x++)
				{
					r->posx=x;r->posy=y;r->posz=z;
					r++;
				}
	}
	void test()	{
		
		PunktRef r=D[sort()[0]];
		PunktRef l[6];
		unsigned short len=r.getNachb(l);
		for(int i=0;i<len;i++)
			printf("%d-%d-%d:%d\n",l[i]->posx,l[i]->posy,l[i]->posz,im[l[i]]);
	}
	unsigned int* sort()
	{
		unsigned int *s_list=(unsigned int*)malloc(im.size()*sizeof(unsigned int));
		for(unsigned int i=0;i<im.size();i++)s_list[i]=i;
		sort_f<T> comp(&im,&D);
		const unsigned int N = im.size();
		std::sort(s_list, s_list + N,comp);
		return s_list;
	}
};

#endif
