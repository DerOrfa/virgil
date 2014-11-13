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


#include "glvlplaneview.h"
#include "ui_GLvlView.h"
#include "glvlplanecam.h"
#include "glvlmasterview.h"

#include <qframe.h>
#include <qlayout.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <assert.h>
#include <qtabwidget.h>
#include <qpushbutton.h>
#include <qstatusbar.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qmenubar.h>
#include <qaction.h>
#include <qlineedit.h>
#include <QWheelEvent>

#include "glvlmultiviewmanager.h"
#include "main.h"

#include <CoreUtils/singletons.hpp>

using namespace boost;

GLvlPlaneView::GLvlPlaneView():cursor(new GLvlPlaneCursor())/*,
AimXStatus((QWidget*)statusBar()),
AimYStatus((QWidget*)statusBar()),
AimZStatus((QWidget*)statusBar())*/
{
	setAttribute(Qt::WA_DeleteOnClose);
	setObjectName("PlaneView");
	setWindowTitle("PlaneView");

	QAction *actionShowOthersHere = new QAction("show &other slices here",this);

	actionShowOthersHere->setCheckable(true);
	menuViews->addAction(actionShowOthersHere);

	this->connect(actionShowOthersHere,SIGNAL(toggled(bool)),SLOT(showOthersHere(bool)));

	glViewContainer->setObjectName("PlaneViewContainer");
	setupSpace(glViewContainer);

	init();
	glview->resizeMode=SGLBaseCam::moveCam;
	glview->Grids.doGrid=0;

	connect(glview,SIGNAL(destroyed(QObject *)),SLOT(lostView()));
	connect(glview,SIGNAL(mouseMoved(QMouseEvent *,SGLVektor )),SLOT(mouseMovedInGL(QMouseEvent *,SGLVektor )));

/*	AimXStatus.setMinimumSize(80,5);
	AimYStatus.setMinimumSize(80,5);
	AimZStatus.setMinimumSize(80,5);

	statusBar()->addWidget(&AimXStatus,10,true);
	statusBar()->addWidget(&AimYStatus,10,true);
	statusBar()->addWidget(&AimZStatus,10,true);

	AimXStatus.setText("0");
	AimYStatus.setText("0");
	AimZStatus.setText("0");
*/
	GLvlMultiviewManager &manager=isis::util::Singletons::get<GLvlMultiviewManager,10>();
	manager.planeViews.push_front(this); //register myself at the view manager

	selector=new QComboBox(centralWidget());
	centralWidget()->layout()->addWidget(selector);

	connect(selector,SIGNAL(activated(int)),SLOT(onSelectMasterImg(int)));

	connect(&manager,SIGNAL(newData()),SLOT(onImgListChange()));

	// first update
	showOthersHere(actionShowOthersHere->isChecked());
	onImgListChange();
}

GLvlPlaneView::~GLvlPlaneView()
{
	isis::util::Singletons::get<GLvlMultiviewManager,10>().planeViews.removeOne(this);
	qDebug("Removing view 0x%x",this);
}

void GLvlPlaneView::showInOthers(bool toggle)
{
	SGLshPtr<GLvlPlaneCam> cam=dynamic_pointer_cast<GLvlPlaneCam>(glview->Camera);
	if(!cam){SGLprintError("Die aktuelle Camera ist keine PLaneCam");return;}
	if(toggle)
	{
		LOG(Debug,isis::info) << "Showing " << this->objectName().toStdString() << "'s camera in others.";
		glview->sendShowObj(cam);
		glview->sendShowObj(cam->myPlane);
	}
	else
	{
		LOG(Debug,isis::info) << "Stop showing " << this->objectName().toStdString() << "'s camera in others.";
		glview->sendUnshowObj(cam);
		glview->sendUnshowObj(cam->myPlane);
	}
}

void GLvlPlaneView::lostView()
{
	qWarning("Implement me");
	close();
}

/*!
	\fn GLvlPlaneView::mouseMovedInGL(QMouseEvent *e,SGLVektor weltKoord);
 */
void GLvlPlaneView::mouseMovedInGL(QMouseEvent *e,SGLVektor weltKoord)
{
//	if(cursor->goTo(weltKoord))
	{
/*		bool here=showCursHereBtn->isChecked(),there=showCursThereBtn->isChecked();
		if(here && there)glview->sendRedraw();
		else if(here)glview->updateGL();
//		else if(there)glview->sendRedrawOther();
		unsigned short stellen=fangToggle->isChecked() ? 0:2;

		if(stellen)
		{
			AimXStatus.setText("X: "+QString::number(cursor->OldPos[0],'f',stellen)+"mm");
			AimYStatus.setText("Y: "+QString::number(cursor->OldPos[1],'f',stellen)+"mm");
			AimZStatus.setText("Z: "+QString::number(cursor->OldPos[2],'f',stellen)+"mm");
		}
		else
		{
			AimXStatus.setText("X: "+QString::number(int(cursor->OldPos[0]))+"mm");
			AimYStatus.setText("Y: "+QString::number(int(cursor->OldPos[1]))+"mm");
			AimZStatus.setText("Z: "+QString::number(int(cursor->OldPos[2]))+"mm");
		}
		if(!Pins->empty())
		{
			SGLVektor len= weltKoord-Pins->back()->pos;
			QString lenStr=stellen?
				QString::number(len.Len(),'f',stellen):
				QString::number(int(len.Len()));
			statusBar()->message("Entfernung zum letzen Pin: "+lenStr+"mm");
		}*/
/*		if(followSegments->isEnabled() && followSegments->isOn())
			onVoxel(tex->texKoord2texIndex(cursor->OldPos-GLvlVolumeTex::masteroffset));*/
		//@todo Wenn fang aus is, wird das oft unnötig ausgelöst
		//besser prüfen, ob sich das Voxel geändert hat
	}
}

void GLvlPlaneView::init()
{
	show();
	SGLshPtr<GLvlPlaneCam> cam(new GLvlPlaneCam());
	cam->ResetUpVect(180); //were working with an upside down coordinate system
	glview->defaultCam(cam);

//	glview->registerObj(cursor);

	cam->connect(glview,
		SIGNAL(pressedMouseMoveRel(QMouseEvent *,float ,float )),
		SLOT(schieben(QMouseEvent *,float ,float )));

	glview->registerObj(cam->myPlane);
	connect(cam.get(),SIGNAL(camChanged()),SLOT(onCamChanged()));
	showInOthers(true);
}

void GLvlPlaneView::onMouseDblClick(QMouseEvent *e)
{
/*	printf("Double click\n");
	uint8_t  top_resize=0,bottom_resize=0;
	bool done=false;
		newPinDlg dlg;
		dlg.koord_x_text->setText(QString::number(cursor->OldPos.SGLV_X));
		dlg.koord_y_text->setText(QString::number(cursor->OldPos.SGLV_Y));
		dlg.koord_z_text->setText(QString::number(cursor->OldPos.SGLV_Z));
		if(dlg.exec()==QDialog::Accepted)
		{
			new GLvlPinsDlg::pinItem(glview,SGLVektor(
				dlg.koord_x_text->text().toFloat(),
				dlg.koord_y_text->text().toFloat(),
				dlg.koord_z_text->text().toFloat()),
				dlg.pinNameEdit->text()
			);
		}
		*/
}


/*!
	\fn GLvlPlaneView::showCursThere(bool toggle)
 */
void GLvlPlaneView::showCursThere(bool toggle)
{
	if(toggle)glview->sendShowObj(cursor);
	else glview->sendUnshowObj(cursor);
}


/*!
	\fn GLvlPlaneView::showCursHere(bool toggle)
 */
void GLvlPlaneView::showCursHere(bool toggle)
{
	if(toggle)glview->registerObj(cursor);
	else glview->unregisterObj(cursor);
}

void GLvlView::onMsg(QString msg,bool canskip){}


void GLvlPlaneView::wheelEvent ( QWheelEvent * e )
{
	const Qt::KeyboardModifiers btn=e->modifiers();
	/*if(btn & (Qt::ShiftModifier|Qt::ControlModifier))
	{
		bool segmentResize=false;
		short top_resize=0,bottom_resize=0;
		if(btn & Qt::ControlModifier)
		{
			segmentResize=true;
			top_resize=e->delta() > 0 ? -1:1;
		}
		else if(btn & Qt::ShiftModifier)
		{
			segmentResize=true;
			bottom_resize=e->delta() > 0 ? -1:1;
		}

		onResizeSegment(top_resize,bottom_resize);
		e->accept();
	}
	else*/ if(btn & Qt::AltModifier)
	{
		SGLshPtr< SGLBaseCam > cam=glview->Camera;
		const int deg = e->delta();

		const dvector move=cam->getLookVektor()*deg/12000.;
		cam->MoveAim(move[0],move[1],move[2]);
		glview->sendRedraw();
		onCamChanged();
		e->accept();
	}else
		e->ignore();
}

void GLvlPlaneView::mouseReleaseEvent(QMouseEvent * e )
{
	if(e->buttons()==((unsigned int)Qt::LeftButton | (unsigned int)Qt::ControlModifier) && !glview->MouseInfo.MovedPastDownBtn)
		jumpToCursor();
	else if(e->buttons()==Qt::LeftButton && !glview->MouseInfo.MovedPastDownBtn)
	{
		selectSegment();
		e->accept();
	}
}

void GLvlPlaneView::onSelectMasterImg(int index)
{
	Bild &img=isis::util::Singletons::get<GLvlMultiviewManager,10>().master_images[index];

	SGLshPtr<GLvlPlaneCam> cam=dynamic_pointer_cast<GLvlPlaneCam>(glview->Camera);
	if(!cam){
		SGLprintError("Die aktuelle Camera ist keine PLaneCam");return;
	} else {
		cam->myPlane->Mat->SetTex(img.getTex());
		cam->myPlane->compileNextTime();
		glview->sendRedraw();
		glview->sendRedrawOther();
	}
}

void GLvlPlaneView::jumpToCursor(){jumpTo(cursor->OldPos);}
void GLvlPlaneView::jumpTo(const SGLVektor &to)
{
	glview->Camera->MoveAimTo(to);
	glview->sendRedraw();
	onCamChanged();
}

void GLvlPlaneView::onImgListChange(){
	selector->clear();
	int current=selector->currentIndex();

	if(not isis::util::Singletons::get<GLvlMultiviewManager,10>().master_images.isEmpty()){
		glViewContainer->setEnabled(true);

		Q_FOREACH(const Bild &img,(isis::util::Singletons::get<GLvlMultiviewManager,10>().master_images)){
			selector->addItem(QString::fromStdString(img.genName()));
		}
		if(current!=selector->currentIndex()){
			onSelectMasterImg(selector->currentIndex());
		}
	} else {
		glViewContainer->setEnabled(false);
	}
}

