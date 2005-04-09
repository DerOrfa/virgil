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
#include "glvlpin.h"
#include <libsgl/util/sglmaterial.h>

GLvlPin::GLvlPin(SGLVektor pos,QString Name):
QObject(NULL,Name.latin1()),Title(Name.latin1())
{
	Kopf.MoveTo(0,0,5);
	Title.MoveTo(0,0,7);
	Kopf.Mat->SetColor(0,0,255);
	Title.Mat->SetColor(255,0,0);
	
	Nadel.height=5;
	Nadel.size_bottom=0;
	this->Name=Name;
	this->pos=pos;
	MoveTo(pos);
}

SGLVektor GLvlPin::getCenter()const
{
	return SGLVektor();
}

/*!
    \fn GLvlPlaneCursor::compileSubObjects()
 */
void GLvlPin::compileSubObjects()
{
	bool draht=((camera->Pos-pos).Len()<20);
	Title.DrahtGitter(draht);
	Kopf.DrahtGitter(draht);
	Nadel.DrahtGitter(draht);
	Objs.clear();
	TrObjs.clear();
	Objs.push_back(Title.Compile(false));
	Objs.push_back(Kopf.Compile(false));
	Objs.push_back(Nadel.Compile(false));
}


/*!
    \fn GLvlPin::setCamera(SGLBaseCam *cam)
 */
void GLvlPin::setCamera(SGLBaseCam *cam)
{
	camera=cam;
	FaceAt=&cam->Pos;
	cam->link(*this);
}
