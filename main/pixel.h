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

/**
@author Enrico Reimer, 1.01.2005,hayd,,@[e/ea],-131.-221. 143
*/

using namespace std;
template<class T> class PunktList;
template <class T> class Bild_vimage;

template<class T> class kPunkt
{
public:
	unsigned short posx,posy,posz;
	T wert;
	kPunkt(
		unsigned short _x=numeric_limits<unsigned short>::max(),
		unsigned short _y=numeric_limits<unsigned short>::max(),
		unsigned short _z=numeric_limits<unsigned short>::max()
	):posx(_x),posy(_y),posz(_z){}
};

template<class T> class iPunkt
{
public:
	T wert;
	unsigned int pos;
	
	iPunkt(unsigned int _id=numeric_limits<unsigned int>::max()):pos(_id){}
	
	static kPunkt<T> pos2koord(const unsigned int pos){
		iPunkt<T> p(pos);
		return kPunkt<T>(p.x(),p.y(),p.z());
	}
	unsigned short getNachb(kPunkt<T> p[])
	{
		unsigned short posx=x();
		unsigned short posy=y();
		unsigned short posz=z();
		
		unsigned short size_x=PunktList<T>::size_x;
		unsigned short size_y=PunktList<T>::size_y;
		unsigned short size_z=PunktList<T>::size_z;
		
		unsigned short pCnt=0;
		if(posx+1<size_x) /*nich in letzter Spalte*/
			p[pCnt++]=iPunkt::pos2koord(pos+1);/*östlicher Nachb*/
		if(posx)
			p[pCnt++]=iPunkt::pos2koord(pos-1);/*westlicher Nachb*/
		
		if(posy+1<size_y) /*nich in letzter Zeile*/
			p[pCnt++]=iPunkt::pos2koord(pos+size_x);/* südlicher Nachb */
		if(posy) /*nich in letzter Zeile*/
			p[pCnt++]=iPunkt::pos2koord(pos-size_x);/* nördlicher Nachb */
	
		if(posz+1<size_z) /*nich in letzter Ebene*/
			p[pCnt++]=iPunkt::pos2koord(pos+size_y*size_x);/*oberer Nachb*/
		if(posz) /*nich in letzter Ebene*/
			p[pCnt++]=iPunkt::pos2koord(pos-size_y*size_x);/*unterer Nachb*/
	
		return pCnt;
	}

	
	inline static unsigned short pos2x(const unsigned int pos,const unsigned short size_x){return pos%size_x;}
	inline  unsigned short x(){return iPunkt::pos2x(pos,PunktList<T>::size_x);}
	
	inline static unsigned short pos2y(const unsigned int pos,const unsigned short size_x,const unsigned short size_y){return	(pos/size_x)%size_y;}
	inline  unsigned short y(){return iPunkt::pos2y(pos,PunktList<T>::size_x,PunktList<T>::size_y);}

	inline static unsigned short pos2z(const unsigned int pos,const unsigned short size_xy){return pos/size_xy;}	
	inline  unsigned short z(){return iPunkt::pos2z(pos,PunktList<T>::size_x*PunktList<T>::size_y);}
	
	inline static unsigned short pos2xy(const unsigned int pos,const unsigned short size_xy){return pos%size_xy;}
	inline unsigned int xy(){return iPunkt::pos2xy(pos,PunktList<T>::size_x*PunktList<T>::size_y);}
};

template<class T> class PunktList
{
public:
	static unsigned short size_x,size_y,size_z;
	iPunkt<T> *m;
	PunktList(unsigned int xsize,unsigned int ysize=1,unsigned int zsize=1)
	{
		size_x=xsize;
		size_y=ysize;
		size_z=zsize;
		m= (iPunkt<T>*)malloc(xsize*ysize*zsize*sizeof(iPunkt<T>));
		for(unsigned int i=0;i<xsize*ysize*zsize;i++)m[i].pos=i;
	}
	PunktList(Bild_vimage<T> &img)
	{
		size_x=img.xsize ;
		size_y=img.ysize;
		size_z=img.zsize;
		m= (iPunkt<T>*)malloc(img.size()*sizeof(iPunkt<T>));
		for(unsigned int i=0;i<img.size();i++)
		{
			iPunkt<T> p(i);
			p.wert=img.at(p.xy(),p.z());
			m[i]=p;
		}
	}
	inline iPunkt<T> &operator[](unsigned int idx){return m[idx];}

};


#endif
