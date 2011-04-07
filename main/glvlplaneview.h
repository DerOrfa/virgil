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

#include <qlabel.h>
#include <libsgl/primitives/sglquader.h>
#include <QComboBox>

#include "glvlplanecam.h"
#include "glvlplanecursor.h"
#include "glvlview.h"

//#include "glvlsegmentdialog.h"
//#include "PlaneView.h"
//#include "../wshed/.h"

class GLvlPlaneView: public GLvlView
{
	Q_OBJECT
public:
	GLvlPlaneView();
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
	void jumpToCursor();
	void jumpTo(const SGLVektor &to);

	SGLshPtr<GLvlPlaneCursor> cursor;
	virtual void onMouseDblClick(QMouseEvent *e);
	private:
	QLabel	AimXStatus,AimYStatus,AimZStatus;

public Q_SLOTS:
	void	onSelectMasterImg(int index);
	void	lostView();
	void	onImgListChange();
private:
	QComboBox *selector;
private Q_SLOTS:
	void mouseMovedInGL(QMouseEvent *e,SGLVektor weltKoord);
protected:
	virtual void wheelEvent ( QWheelEvent * e );
	virtual void mouseReleaseEvent(QMouseEvent * e );
Q_SIGNALS:
	void onVoxel(unsigned int index);
	void onResizeSegment(signed char ,signed char );
	void selectSegment();
};

#endif
