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
class PunktRef;

struct Punkt
{
	unsigned short posx,posy,posz;
	Punkt(
		unsigned short _x=numeric_limits<unsigned short>::max(),
		unsigned short _y=numeric_limits<unsigned short>::max(),
		unsigned short _z=numeric_limits<unsigned short>::max()
	);
};


class PunktList
{
public:
	unsigned int size_x,size_y,size_z;
	unsigned int *m;
	PunktRef operator[](unsigned int _id);
	PunktList(unsigned int xsize,unsigned int ysize=1,unsigned int zsize=1);
};

class PunktRef
{
	PunktList *list;
	unsigned int id;
public:
    typedef PunktList* PunktRef::*unspecified_bool_type;
	
	PunktRef(PunktList *_list,unsigned int _id);
	PunktRef();
	
	static Punkt pos2Punkt(const unsigned int pos,PunktList *list);
	Punkt operator *();
	Punkt *operator ->();
	
	PunktRef operator +(unsigned int _id);
	PunktRef operator -(unsigned int _id);
	
	PunktRef &operator ++();
	PunktRef operator ++(int);
  
    operator unspecified_bool_type() const;

	unsigned short PunktRef::getNachb(Punkt p[]);
	
	inline static unsigned short pos2x(const unsigned int pos,const unsigned short size_x){
		return pos%size_x;
	}
	inline  unsigned short x(){
		return PunktRef::pos2x(list->m[id],list->size_x);
	}
	
	inline static unsigned short pos2y(const unsigned int pos,const unsigned short size_x,const unsigned short size_y){
		return	(pos/size_x)%size_y; 
	}
	inline  unsigned short y(){
		return PunktRef::pos2y(list->m[id],list->size_x,list->size_y);
	}

	inline static unsigned short pos2z(const unsigned int pos,const unsigned short size_xy){
		return pos/size_xy; /*Z-Achse (Band)*/
	}	
	inline  unsigned short z(){
		return PunktRef::pos2z(list->m[id],list->size_x*list->size_y);
	}
	
	inline static unsigned short pos2xy(const unsigned int pos,const unsigned short size_xy){
		return pos%size_xy;
	}
	inline  unsigned int xy(){
		return PunktRef::pos2xy(list->m[id],list->size_x*list->size_y);
	}

};

class ptr_fifo:public deque<PunktRef>
{
	public:
	void push_null();
	void push(PunktRef p);
	PunktRef pop();
};

#endif
