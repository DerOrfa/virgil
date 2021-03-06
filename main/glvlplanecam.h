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
#ifndef GLVLPLANECAM_H
#define GLVLPLANECAM_H

#include <libsgl/helper/sglcamera.h>
#include <libsgl/primitives/sglvieleck.h>
#include <qobject.h>
#include <QMouseEvent>
#include "glvlvolumetex.h"
#include "glvlcutplane.h"
/**
@author Enrico Reimer,,,
*/
class GLvlPlaneCam : public QObject, public SGLBaseCam
{
Q_OBJECT
public:
	GLvlPlaneCam();
	void generate();
	SGLshPtr<GLvlCutPlane> myPlane;
	GLfloat farbe[3];
Q_SIGNALS:
	void camChanged();
public Q_SLOTS:
	void schieben(QMouseEvent * e,float relMoveX,float relMoveY);
};

#endif

