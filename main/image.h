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

template <class T> class sort_q:
public binary_function< iPunkt<T>, iPunkt<T>, bool>
{
public:
	inline bool operator()(iPunkt<T> p1,iPunkt<T> p2)
	{
		return p1.wert <p2.wert;
	}
};

template<class T> class ws_image{
	Bild_vimage<T> im;
public:
	PunktList<T> D;
	ws_image(VImage src):im(src),D(im)
	{
		printf("Lege %d Punkte an\n",im.size());
	}
	void test()	{
		printf("Sortiere\n");
		sort2();
		printf("fertsch\n");
		iPunkt<T> r=D[5000];
		kPunkt<T> l[6];
		unsigned short len=r.getNachb(l);
		for(int i=0;i<len;i++)
			printf("%d-%d-%d:%d\n",l[i].posx,l[i].posy,l[i].posz,im[l[i]]);
	}
	unsigned int* sort2()
	{
		unsigned int s=im.size();
		sort_q<T> comp;
		std::sort(D.m, D.m + im.size(),comp);
	}
};

#endif
