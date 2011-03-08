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
#ifndef GLVLMASTERVIEW_H
#define GLVLMASTERVIEW_H

#include "ui_GLvlView.h"
#include "glvlplaneview.h"
#include <libsgl/qt4_glue/sglqtmultispacemgr.h>
//#include "../wshed/vincent_transform.h"
#include <list>
#include <time.h>
//#include "glvlminima.h"
#include <libsgl/sglsignal.h>
#include "bild.h"


using namespace boost;

/**
@author Enrico Reimer, 1.01.2005,hayd,,@[e/ea],-131.-221. 143
*/

class GLvlMasterView : public GLvlView,public SGLqtMultiSpaceMgr{
	SGLqtSpace * mw;
	SGLshPtr<SGLCube> rahmen;
	SGLSignal<void ()> updatePlanes;
	class SelectSlot:public SGLSlot
	{
		GLvlMasterView *master;
		public:
			void operator()();
			SelectSlot(GLvlMasterView* p);
	}onDataSelect;
public:
	GLvlMasterView();
	~GLvlMasterView();
	static SGLshPtr<Bild<GLubyte> > MasterImg;
	static list<GLvlPlaneView *> views;
	void newPlane();
	void doConfig();
	void doBenchmark(time_t benchLen);
//	void loadWShedDlg();
	void onMsg(QString msg,bool canskip);
	bool loadData();
/*	class MemCreateNotify:public MemConsumer::NotifyCreateSlot<MemCreateNotify>
	{
		public:
		void operator()(const MemConsumer &newob) const;
	}TexCreate;
	class MemDeleteNotify:public MemConsumer::NotifyDeleteSlot<MemDeleteNotify>
	{
		public:
		void operator()(const MemConsumer &newob) const;
	}TexDelete;*/

protected:
	void doBenchmark();
	void closeEvent(QCloseEvent *e);
};

#endif
