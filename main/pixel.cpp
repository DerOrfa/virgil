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
 
 // $Id$
 // $Revision$
 
#include "pixel.h"

#include <limits>
#include <iostream>

PunktRef::PunktRef(PunktList *_list,unsigned int _id):list(_list),id(_id){}
PunktRef::PunktRef():list(NULL){}

Punkt PunktRef::pos2Punkt(const unsigned int pos,PunktList *list)
{
	const unsigned short size_x=list->size_x;
	const unsigned short size_y=list->size_y;
	const unsigned short size_xy=size_x*size_y;
	
	return Punkt(
		PunktRef::pos2x(pos,size_x), /*X-Achse (Band)*/
		PunktRef::pos2y(pos,size_x,size_y), /*Y-Achse (Band)*/
		PunktRef::pos2z(pos,size_xy) /*Z-Achse (Band)*/
	);
}
Punkt PunktRef::operator *(){return pos2Punkt(list->m[id],list);}

Punkt *PunktRef::operator ->(){return &(this->operator *());}

PunktRef PunktRef::operator +(unsigned int _id){return PunktRef(list,id+_id);}
PunktRef PunktRef::operator -(unsigned int _id){return PunktRef(list,id-_id);}

PunktRef &PunktRef::operator ++(){id++;return *this;}
PunktRef PunktRef::operator ++(int){PunktRef ret(list,id);id++;return ret;}

PunktRef::operator unspecified_bool_type() const{return list == NULL? NULL: &PunktRef::list;}

unsigned short PunktRef::getNachb(Punkt p[])
{
	unsigned int pos=list->m[id];
	unsigned short posx=x();
	unsigned short posy=y();
	unsigned short posz=z();
	
	unsigned short size_x=list->size_x;
	unsigned short size_y=list->size_y;
	unsigned short size_z=list->size_z;
	
	unsigned short pCnt=0;
	if(posx+1<size_x) /*nich in letzter Spalte*/
		p[pCnt++]=PunktRef::pos2Punkt(pos+1,list);/*östlicher Nachb*/
	if(posx)
		p[pCnt++]=PunktRef::pos2Punkt(pos-1,list);/*westlicher Nachb*/
	
	if(posy+1<size_y) /*nich in letzter Zeile*/
		p[pCnt++]=PunktRef::pos2Punkt(pos+size_x,list);/* südlicher Nachb */
	if(posy) /*nich in letzter Zeile*/
		p[pCnt++]=PunktRef::pos2Punkt(pos-size_x,list);/* nördlicher Nachb */

	if(posz+1<size_z) /*nich in letzter Ebene*/
		p[pCnt++]=PunktRef::pos2Punkt(pos+size_y*size_x,list);/*oberer Nachb*/
	if(posz) /*nich in letzter Ebene*/
		p[pCnt++]=PunktRef::pos2Punkt(pos-size_y*size_x,list);/*unterer Nachb*/

	return pCnt;
}

Punkt::Punkt(unsigned short _x,unsigned short _y,unsigned short _z):posx(_x),posy(_y),posz(_z){};

PunktRef PunktList::operator [](unsigned int _id){return PunktRef(this,_id);}

PunktList::PunktList(unsigned int xsize,unsigned int ysize,unsigned int zsize):size_x(xsize),size_y(ysize),size_z(zsize)
{
	m= (unsigned int*)malloc(xsize*ysize*zsize*sizeof(unsigned int));
	for(unsigned int i=0;i<xsize*ysize*zsize;i++)m[i]=i;
}

void ptr_fifo::push_null(){push_back(PunktRef());}
void ptr_fifo::push(PunktRef p){push_back(p);}
PunktRef ptr_fifo::pop(){PunktRef ret=front();pop_front();return ret;}

