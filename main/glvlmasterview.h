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

#include "glvlplaneview.h"
#include "sglqtmultispacemgr.h"
#include "../wshed/vincent_transform.h"
#include <list>
#include <time.h>
#include "glvlminima.h"
#include <boost/signal.hpp>

using namespace efc;
using namespace boost;
/**
@author Enrico Reimer, 1.01.2005,hayd,,@[e/ea],-131.-221. 143
*/

class GLvlMasterView : public GLvlView,public SGLqtMultiSpaceMgr{
	SGLqtSpace * mw;
	ERegistry *masterReg;
	shared_ptr<SGLCube> rahmen;
	shared_ptr<shared_pin_list> Pins;
	shared_ptr<vincent::transform> v_transform;
	SGLSignal<void ()> updatePlanes;
public:
	GLvlMasterView(std::list<VImage> src);
	~GLvlMasterView();
	VImage MasterImg;
	void newPlane();
	void newPlane(EWndRegistry *hisReg);
	void doConfig();
	void doBenchmark(time_t benchLen);
    void loadIntoWShed();
	void onReached(vincent::VBild_value h,unsigned short objs);
	void onMsg(QString msg,bool canskip);
	void onTransformEnd();
	void showSegmentAt(unsigned index);
	void resizeCurrSegment(short topdelta,short bottomdelta);
	void selectCurrSegment();
    void redrawAktSegment();
    bool loadSegmentTex(shared_ptr<GLvlMinima> img,EVektor<unsigned short> pos);
    void loadSegmentListTex(GLvlMinimaList &img);

protected:
    void doBenchmark();
	void closeEvent(QCloseEvent *e);
	map<vincent::lab_value,shared_ptr<GLvlMinima> > objs;
	shared_ptr<GLvlMinima> aktMinima;
	GLvlMinimaList selMinima;
};

#endif
