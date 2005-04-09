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
#include "glvlplanecursor.h"

bool GLvlPlaneCursor::goTo(SGLVektor pos)
{
	if(fang)for(int i=0;i<3;i++)
		pos[i]=rint(pos[i]);
	if(OldPos!=pos)
	{
		OldPos=pos;
		MoveTo(SGLVektor(pos));
		compileNextTime();
		return true;
	}
	else return false;
}

SGLVektor GLvlPlaneCursor::getCenter()const
{
	return SGLVektor();
}

/*!
    \fn GLvlPlaneCursor::compileSubObjects()
 */
void GLvlPlaneCursor::compileSubObjects()
{
	Objs.clear();
	TrObjs.clear();
	for(list<SGLCube>::iterator i=Cubes.begin();i!=Cubes.end();i++)
		Objs.push_back(i->Compile(false));
}



/*!
    \fn GLvlPlaneCursor::setFang(bool)
 */
void GLvlPlaneCursor::setFang(bool fang)
{
	this->fang=fang;
}


/*!
    \fn GLvlPlaneCursor::setSize(int size)
 */
void GLvlPlaneCursor::setSize(int size)
{
	list<SGLCube>::iterator i;
	if(Cubes.size())
		Cubes.resize(size*size*size,*(i=Cubes.begin()));
	else 
	{
		SGLCube Muster;Muster.DrahtGitter();
		Cubes.resize(size*size*size,Muster);
		i=Cubes.begin();
	}
	
	for(int x=0;x<size;x++)
	for(int y=0;y<size;y++)
	for(int z=0;z<size;z++)
		(i++)->MoveTo(x-size/2.+.5,y-size/2.+.5,z-size/2.+.5);
			
//	compileSubObjects();Sollte eig. automatisch gehen
	compileNextTime();
}
