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
	Punkt(PunktRef ref);
};


class PunktList
{
	unsigned int size_x,size_y,size_z;
	friend class PunktRef;
public:
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
	
	Punkt operator *();
	Punkt *operator ->();
	
	PunktRef operator +(unsigned int _id);
	PunktRef operator -(unsigned int _id);
	
	PunktRef &operator ++();
	PunktRef operator ++(int);
  
    operator unspecified_bool_type() const;

	unsigned short PunktRef::getNachb(PunktRef p[]);
};

class ptr_fifo:public deque<PunktRef>
{
	public:
	void push_null();
	void push(PunktRef p);
	PunktRef pop();
};

#endif
