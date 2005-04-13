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

#include <libsgl/sglsignal.h>
#include <libsgl/sglqtspace.h>
#include "glvlsegmentdialog.h"
#include "glvlplanecam.h"
#include "PlaneView.h"
#include "glvlvolumetex.h"
#include <libsgl/primitives/sglquader.h>
#include "../wshed/vincent_transform.h"
#include "glvlplanecursor.h"
#include "glvlpin.h"
#include "config.h"
#include <vista/VImage.h>

#include <eclasses/EWndRegistry.h>

#include <qlabel.h> 


using namespace efc;

class GLvlPinsDlg;

class GLvlView : public GLvlViewBase
{
	Q_OBJECT
public:
	GLvlView(
		SGLqtSpace* mw, 
		boost::shared_ptr<GLvlVolumeTex> tex,
		EWndRegistry *myReg
	);
	virtual bool loadCfg();
	virtual bool saveCfg();
	SGLqtSpace * glview;
	void showOthersHere(bool toggle);
	void showObjList();
	boost::shared_ptr<GLvlVolumeTex> tex;
	static GLvlSegmentDialog* wshed;
	static GLvlPinsDlg* pinsDlg;
	static ConfigDlg* configDlg;
	static boost::shared_ptr<SGLBaseCam> activeCam;
	void	selectView(const SGLVektor dir[3]);

protected:
	EWndRegistry *myReg;
	void setupSpace(SGLqtSpace *space);
	void closeEvent(QCloseEvent *e);
	boost::shared_ptr<SGLBaseCam> myCam;
private:
	class SGLgotFocusSlot:public  SGLSlot
	{
		const boost::shared_ptr<SGLBaseCam> &myCam;
	public:
		SGLgotFocusSlot(const boost::shared_ptr<SGLBaseCam> &_myCam):myCam(_myCam){}
		void operator()(int reason){GLvlView::activeCam=myCam;}
	}onGotFocus;

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
	void	selectViewMode(int view);
	void sichtVonHinten();
	void sichtVonVorn();
	void sichtVonOben();
	void sichtVonUnten();
	void sichtVonRechts();
	void sichtVonLinks();

	virtual void onMsg(QString msg,bool canskip);
    virtual void showPinsDlg(bool);
	virtual void showConfigDlg(bool toggle);
};

class GLvlPlaneView: public GLvlView
{
	Q_OBJECT
public:
	GLvlPlaneView(
		SGLqtSpace* mw, 
		boost::shared_ptr<GLvlVolumeTex> tex,
		EWndRegistry *myReg
		);
	virtual ~GLvlPlaneView();
	virtual bool loadCfg();
	virtual bool saveCfg();
	void showInOthers(bool toggle);
	void init();
	void showCursThere(bool toggle);
	void showCursHere(bool toggle);
    void showSegmentAt(unsigned int index);
	void resizeCurrSegment(short topdelta,short bottomdelta);
	void selectCurrSegment();
	boost::shared_ptr<GLvlPlaneCursor> cursor;
	list<GLvlPlaneView *>::iterator it;
private:
	QLabel	AimXStatus,AimYStatus,AimZStatus;

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
	void onResizeSegment(signed char ,signed char );
	void selectSegment();
};

#endif
