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
#ifndef GLVLPIN_H
#define GLVLPIN_H

#include <libsgl/sglmetaobj.h>
#include <libsgl/sglmetaobj.h>
#include <libsgl/primitives/sglquadricobj.h>
#include <libsgl/helper/sglcamera.h>

#include <qobject.h>


/**
@author Enrico Reimer, 1.01.2005,hayd,,@[e/ea],-131.-221. 143
*/
class GLvlPin : public QObject, public SGLMetaObj
{
Q_OBJECT
public:
	GLvlPin(SGLVektor pos,QString Name);
	void compileSubObjects();
	SGLVektor getCenter();
    void setCamera(SGLBaseCam *cam);
	QString Name;
	SGLVektor pos;
private:
	SGLSphere Kopf;
	SGLZylinder Nadel;
	SGLBaseCam *camera;
};

#endif
