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
#ifndef VBILD_H
#define VBILD_H

#include "vincent_punkt.h"
#include "../main/bild.h"

namespace vincent
{

template <class T> class Bild_mem: 
public ::Bild_mem<T>{
public:
	Bild_mem(unsigned short x,unsigned short y,unsigned short z,T initVal): 
	::Bild_mem<T>(x,y,z,initVal){}
	template <class PT> inline T &operator[](iPunkt<PT> &p){return data[p.pos];}
};

template <class T> class Bild_vimage : 
public ::Bild_vimage<T>{
public:
	Bild_vimage(VImage _img): ::Bild_vimage<T>(_img){}
	template <class PT> inline T &operator[](iPunkt<PT> &p){return at(p.pos);}
};
}
#endif
