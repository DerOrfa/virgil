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
#ifndef PIXEL_H
#define PIXEL_H

#include <limits>
#include <deque>
#include <algorithm>
#include <functional>

/**
@author Enrico Reimer, 1.01.2005,hayd,,@[e/ea],-131.-221. 143
*/

namespace vincent
{
enum richtung {nord=0,sued=1,ost=2,west=3,ueber=4,unter=5};

template<class T> class PunktList;
template <class T> class Bild_vimage;
template <class T> class Bild;

template<class T> class iPunkt{
public:
	T wert;
	unsigned int pos;
	
	iPunkt(unsigned int _id=std::numeric_limits<unsigned int>::max()):pos(_id){}
	iPunkt(unsigned int _id,const Bild_vimage<T> &img):pos(_id)
	{
		if(	img.xsize==std::numeric_limits<unsigned short>::max() && 
			img.ysize==std::numeric_limits<unsigned short>::max() &&
			img.zsize==std::numeric_limits<unsigned short>::max())
			printf("die Bilddaten wurden m�glicherweise noch nicht initialisiert\n");
		wert=img.at(pos);
	}
	
	unsigned short getNachb(iPunkt<T> p[],const Bild_vimage<T> &img)const
	{
		unsigned short posx=x(img.xsize);
		unsigned short posy=y(img.xsize,img.ysize);
		unsigned short posz=z(img.xsize,img.ysize);
		
		unsigned short pCnt=0;
		if(posx+1<img.xsize) /*nich in letzter Spalte*/
			p[pCnt++]=iPunkt(pos+1,img);/*�stlicher Nachb*/
		if(posx)
			p[pCnt++]=iPunkt(pos-1,img);/*westlicher Nachb*/
		
		if(posy+1<img.ysize) /*nich in letzter Zeile*/
			p[pCnt++]=iPunkt(pos+img.xsize,img);/* s�dlicher Nachb */
		if(posy) /*nich in letzter Zeile*/
			p[pCnt++]=iPunkt(pos-img.xsize,img);/* n�rdlicher Nachb */
	
		if(posz+1<img.zsize) /*nich in letzter Ebene*/
			p[pCnt++]=iPunkt(pos+img.ysize*img.xsize,img);/*oberer Nachb*/
		if(posz) /*nich in letzter Ebene*/
			p[pCnt++]=iPunkt(pos-img.ysize*img.xsize,img);/*unterer Nachb*/
	
		return pCnt;
	}
	
	void getNachbStruct(iPunkt<T> p[],const Bild_vimage<T> &img)const
	{
		unsigned short posx=x(img.xsize);
		unsigned short posy=y(img.xsize,img.ysize);
		unsigned short posz=z(img.xsize,img.ysize);
		
		unsigned short pCnt=0;
		if(posx+1<img.xsize) /*nich in letzter Spalte*/
			p[ost]=iPunkt(pos+1,img);/*�stlicher Nachb*/
		if(posx)
			p[west]=iPunkt(pos-1,img);/*westlicher Nachb*/
		
		if(posy+1<img.ysize) /*nich in letzter Zeile*/
			p[nord]=iPunkt(pos+img.xsize,img);/* s�dlicher Nachb */
		if(posy) /*nich in letzter Zeile*/
			p[sued]=iPunkt(pos-img.xsize,img);/* n�rdlicher Nachb */
	
		if(posz+1<img.zsize) /*nich in letzter Ebene*/
			p[ueber]=iPunkt(pos+img.ysize*img.xsize,img);/*oberer Nachb*/
		if(posz) /*nich in letzter Ebene*/
			p[unter]=iPunkt(pos-img.ysize*img.xsize,img);/*unterer Nachb*/
	}
	inline bool invalid()	{return pos==std::numeric_limits<unsigned int>::max();}
	
	inline static unsigned short pos2x(const unsigned int pos,const unsigned short size_x){return pos%size_x;}
	inline  unsigned short x(const unsigned short size_x)const{return iPunkt::pos2x(pos,size_x);}
	
	inline static unsigned short pos2y(const unsigned int pos,const unsigned short size_x,const unsigned short size_y){return	(pos/size_x)%size_y;}
	inline  unsigned short y(const unsigned short size_x,const unsigned short size_y)const{return iPunkt::pos2y(pos,size_x,size_y);}

	inline static unsigned short pos2z(const unsigned int pos,const unsigned int size_xy){return pos/size_xy;}	
	inline  unsigned short z(const unsigned short size_x,const unsigned short size_y)const{return iPunkt::pos2z(pos,size_x*size_y);}
	
	inline static unsigned short pos2xy(const unsigned int pos,const unsigned int size_xy){return pos%size_xy;}
	inline unsigned int xy(const unsigned short size_x,const unsigned short size_y)const{return iPunkt::pos2xy(pos,size_x*size_y);}
};

template<class T> class PunktList{
public:
	iPunkt<T> *m;
	unsigned int size;
	inline size_t m_size()const{return size*sizeof(PunktList<T>);}
	PunktList(unsigned int xsize,unsigned int ysize=1,unsigned int zsize=1)
	{
		m= (iPunkt<T>*)malloc(xsize*ysize*zsize*sizeof(iPunkt<T>));
		for(unsigned int i=0;i<xsize*ysize*zsize;i++)m[i].pos=i;
	}
	PunktList(const Bild_vimage<T> &img):size(img.size())
	{
		m= (iPunkt<T>*)malloc(size*sizeof(iPunkt<T>));
		for(unsigned int i=0;i<size;i++)
			m[i]=iPunkt<T>(i,img);
	}
	PunktList(const Bild_vimage<T> &img,T exclude):size(0)
	{
		m= (iPunkt<T>*)malloc(img.size()*sizeof(iPunkt<T>));
		for(unsigned int i=0;i<img.size();i++)
			if(img.at(i)!=exclude)m[size++]=iPunkt<T>(i,img);
		m=(iPunkt<T>*)realloc(m,m_size());
	}
	template <class AT> PunktList(const PunktList<AT> &order,const Bild_vimage<T> &img,T exclude):size(0)
	{
		m= (iPunkt<T>*)malloc(order.size*sizeof(iPunkt<T>));
		for(unsigned int i=0;i<order.size;i++)
		{
			const iPunkt<T> p=iPunkt<T>(order.m[i].pos,img);
			if(p.wert!=exclude)
				m[size++]=p;
		}
		m=(iPunkt<T>*)realloc(m,m_size());
	}
	~PunktList(){free(m);}
	inline iPunkt<T> &operator[](unsigned int idx)
	{
		return m[idx];
	}
};

template<class T> class PunktFifo:public std::deque< iPunkt<T> >{
public:
	void push_null(){push_back(iPunkt<T>());}
	void push(iPunkt<T> p){push_back(p);}
	iPunkt<T> pop(){iPunkt<T> ret=this->front();this->pop_front();return ret;}
};

template <class T> class sort_q:public std::binary_function< iPunkt<T>, iPunkt<T>, bool >{
public:
	inline bool operator()(iPunkt<T> p1,iPunkt<T> p2){return p1.wert <p2.wert;}
};
}
#endif
