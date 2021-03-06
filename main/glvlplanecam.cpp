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
//
// C++ Implementation: glvlplanecam
//
// Description:
//
//
// Author: Enrico Reimer,,, <enni@Akira>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "glvlplanecam.h"
#include "glvlplaneview.h"
#include <qslider.h>


GLvlPlaneCam::GLvlPlaneCam():SGLBaseCam(0,0,150),myPlane(new GLvlCutPlane)
{
	move_cam_with_aim=myPlane->is_free=true;
	showCross=false;

	farbe[0]=farbe[1]=farbe[2]=1;
	Compile();//damit die EckVektoren stimmen
	myPlane->LinkEckVekt(Ecken,4);
	link(*myPlane.get());
}

void GLvlPlaneCam::generate()
{
	if(showCross)
	{
		SGLVektor Vert[2],Horiz[2];
		getCross(Horiz,Vert);
		glBegin(GL_LINES);
			Vert[1].DrawPureVertex();Vert[0].DrawPureVertex();
			Horiz[1].DrawPureVertex();Horiz[0].DrawPureVertex();
		glEnd();
	}

	recalcEcken();
	myPlane->resetTexKoord();

	// prevent nvidia from optimizing out the setting
	// 	glLineWidth(1); glColor3f(0,0,0);

	glLineWidth(2);
	glBegin(GL_LINES);
		glColor3f(1,0,0);
		Ecken[0]->DrawPureVertex();
		Ecken[1]->DrawPureVertex();

		glColor3f(0,1,0);
		Ecken[1]->DrawPureVertex();
		Ecken[2]->DrawPureVertex();

		glColor3f(0,0,0);
		Ecken[2]->DrawPureVertex();
		Ecken[3]->DrawPureVertex();

		glColor3f(0,0,1);
		Ecken[3]->DrawPureVertex();
		Ecken[0]->DrawPureVertex();
	glEnd();

	glLineWidth(1);
}

/*!
	\fn GLvlPlaneCam::schieben(float amount)
 */
void GLvlPlaneCam::schieben(QMouseEvent * e,float relMoveX,float relMoveY)
{
	if(e->buttons()&Qt::MidButton)
	{
		const dvector schiebVekt=getLookVektor()*-(relMoveY*.02);
		MoveAim(schiebVekt[0],schiebVekt[1],schiebVekt[2]);
		//movecam ist nicht n�tig, da move_cam_with_aim true ist
		compileNextTime();
		camChanged();
	}
}
