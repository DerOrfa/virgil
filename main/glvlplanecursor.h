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
#ifndef GLVLPLANECURSOR_H
#define GLVLPLANECURSOR_H

#include <libsgl/sglmetaobj.h>
#include <libsgl/primitives/sglquader.h>
#include <qobject.h>


/**
@author Enrico Reimer, 1.01.2005,hayd,,@[e/ea],-131.-221. 143
*/
class GLvlPlaneCursor : public QObject, public SGLMetaObj
{
	Q_OBJECT
public:
	list<SGLCube> Cubes;
	// es scheint, daß die smart-pointer einen Zeiger auf andere Referenzen halten, und sie bekommen große probs, 
	// wenn sich diese in Folge von reallozierung verlagern => deshalb kein vector
	void compileSubObjects();
	SGLVektor getCenter()const;
	SGLVektor OldPos;
	bool goTo(SGLVektor pos);
private:
	bool fang;

public slots:
    void setFang(bool);
    void setSize(int size);
};

#endif
