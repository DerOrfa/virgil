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

#ifndef GLVLPLANEVIEW_H
#define GLVLPLANEVIEW_H

#include <libsgl/sglqtspace.h>
#include "glvlplanecam.h"
#include "PlaneView.h"
#include "glvlvolumetex.h"
#include <libsgl/primitives/sglquader.h>
#include "../wshed/vincent_transform.h"
#include "glvlplanecursor.h"
#include "glvlpin.h"

#include <eclasses/EWndRegistry.h>

#include <qlabel.h> 

using namespace efc;
typedef list< boost::shared_ptr<GLvlPin> >	shared_pin_list;


class GLvlView : public GLvlViewBase
{
	Q_OBJECT
public:
	GLvlView(
		SGLqtSpace* mw, 
		shared_ptr<GLvlVolumeTex> tex,
		EWndRegistry *myReg
	);
	virtual bool loadCfg();
	virtual bool saveCfg();
	SGLqtSpace * glview;
	void showOthersHere(bool toggle);
	void showObjList();
	shared_ptr<GLvlVolumeTex> tex;
protected:
	EWndRegistry *myReg;
	void setupSpace(SGLqtSpace *space);
	void closeEvent(QCloseEvent *e);
	boost::shared_ptr<SGLBaseCam> myCam;
private:
	bool selfChange;
	static SGLVektor default_oben[3],default_unten[3],default_vorn[3],default_hinten[3],default_rechts[3],default_links[3];

public slots:
	/*$PUBLIC_SLOTS$*/
	void	canRoll(bool toggle);
	void	canGier(bool toggle);
	void	canKipp(bool toggle);
	void	setCoordAim();
	void	setRollDeg(int deg);
	void	setCoordCam();
	void	onCamChanged();
	void	selectView(int view);
	void	selectViewMode(int view);
    virtual void onReached(vincent::VBild_value h,unsigned short objs);
	virtual void onMsg(QString msg,bool canskip);
	virtual void onTransformEnd();
	virtual void showSegmentAt(unsigned int index)=0;
	virtual void resizeCurrSegment(short topdelta,short bottomdelta)=0;
	virtual void selectCurrSegment()=0;
};

class GLvlPlaneView: public GLvlView
{
	Q_OBJECT
public:
	GLvlPlaneView(
		SGLqtSpace* mw, 
		shared_ptr<GLvlVolumeTex> tex,
		EWndRegistry *myReg,
		shared_ptr< shared_pin_list > Pins
		);
	virtual bool loadCfg();
	virtual bool saveCfg();
	void showInOthers(bool toggle);
	void init();
	void showCursThere(bool toggle);
	void showCursHere(bool toggle);
    void showSegmentAt(unsigned int index);
	void resizeCurrSegment(short topdelta,short bottomdelta);
	void selectCurrSegment();
	shared_ptr<GLvlPlaneCursor> cursor;

private:
	QLabel	AimXStatus,AimYStatus,AimZStatus;
	shared_ptr< shared_pin_list > Pins;

public slots:
	void	lostView();

private slots:
    void mouseMovedInGL(QMouseEvent *e,SGLVektor weltKoord);
protected:
	virtual void mouseDoubleClickEvent(QMouseEvent *e);
	virtual void wheelEvent ( QWheelEvent * e );
	virtual void mouseReleaseEvent(QMouseEvent * e );
signals:
    void onVoxel(unsigned int index);
	void onResizeSegment(short,short);
	void selectSegment();
};

#endif
