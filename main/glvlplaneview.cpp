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
#include "glvlplanecam.h"
#include "newPinDlg.h"

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

using namespace boost;
using namespace efc;

GLvlPlaneView::GLvlPlaneView(
	SGLqtSpace* mw, 
	shared_ptr<GLvlVolumeTex> tex,
	EWndRegistry *myReg,
	shared_ptr< shared_pin_list > Pins
):
GLvlView(mw, tex,myReg),
cursor(new GLvlPlaneCursor()),
AimXStatus((QWidget*)statusBar()),
AimYStatus((QWidget*)statusBar()),
AimZStatus((QWidget*)statusBar()),
Pins(Pins)
{
	setupSpace(new SGLqtSpace(mw,glViewContainer));
	glview->resizeMode=SGLBaseCam::moveCam;
//	toolTabs->removePage(toolTabs->page(0));
	menuBar()->removeItemAt (0);
	menuBar()->removeItemAt (0);
	genSegmentsBtn->hide();
	fileExitAction->setMenuText("Sicht schließen");
	myReg->Suicide=true;
	connect(glview,SIGNAL(destroyed(QObject *)),SLOT(lostView()));
	connect(glview,SIGNAL(mouseMoved(QMouseEvent *,SGLVektor )),SLOT(mouseMovedInGL(QMouseEvent *,SGLVektor )));
	setCaption("Schnitt");
	
	AimXStatus.setMinimumSize(80,5);
	AimYStatus.setMinimumSize(80,5);
	AimZStatus.setMinimumSize(80,5);

	statusBar()->addWidget(&AimXStatus,0,true);
	statusBar()->addWidget(&AimYStatus,0,true);
	statusBar()->addWidget(&AimZStatus,0,true);
	
	AimXStatus.setText("0");
	AimYStatus.setText("0");
	AimZStatus.setText("0");
}

bool GLvlPlaneView::loadCfg()
{
	showInOthersBtn->setOn(myReg->getbVal("Andere anzeigen",true));
	showOthersHereBtn->setOn(myReg->getbVal("In anderen anzeigen",true));
	GLvlView::loadCfg();
}

bool GLvlPlaneView::saveCfg()
{
	GLvlView::saveCfg();	
	if(!showInOthersBtn->isHidden())
		myReg->setbVal("Andere anzeigen",showInOthersBtn->isOn());
	if(!showOthersHereBtn->isHidden())
		myReg->setbVal("In Anderen anzeigen",showOthersHereBtn->isOn());
}



void GLvlPlaneView::showInOthers(bool toggle)
{
	boost::shared_ptr<GLvlPlaneCam> cam=dynamic_pointer_cast<GLvlPlaneCam>(glview->Camera);
	if(!cam){SGLprintError("Die aktuelle Camera ist keine PLaneCam");return;}
	if(toggle)
	{
		glview->sendShowObj(cam);
		glview->sendShowObj(cam->myPlane);
	}
	else 
	{
		glview->sendUnshowObj(cam);
		glview->sendUnshowObj(cam->myPlane);
	}
}

void GLvlPlaneView::lostView()
{
	myReg->Suicide=false;
	close();
}

GLvlView::GLvlView(SGLqtSpace* mw, shared_ptr<GLvlVolumeTex> tex,EWndRegistry *myReg):
GLvlViewBase(NULL,NULL,mw?Qt::WDestructiveClose:0)//Das MasterWnd darf erst durch die app gelöscht werden
{
	selfChange=false;
	this->myReg=myReg;
	this->tex=tex;
}

void GLvlView::setupSpace(SGLqtSpace *space)
{
	glview = space;
	glview->registerDynamicTex(*tex);
	connect(glview,SIGNAL(camChanged()),SLOT(onCamChanged()));
	assert(glViewContainer->layout());//eigentlich sollte SGLqtSpace sicherstellen, daß sein Parent ein layout hat ..
	glViewContainer->layout()->setMargin(1);
	myReg->setWindow(this);
}

/*$SPECIALIZATION$*/
void GLvlView::canRoll(bool toggle)
{
	assert(glview && glview->Camera);
	glview->Camera->lockRoll=!toggle;
}

void GLvlView::canGier(bool toggle)
{
	assert(glview && glview->Camera);
	glview->Camera->lockGierCam=!toggle;
}

void GLvlView::canKipp(bool toggle)
{
	assert(glview && glview->Camera);
	glview->Camera->lockKippCam=!toggle;
}

void GLvlView::setCoordAim()
{
	if(selfChange)return;
	assert(glview && glview->Camera);
	SGLVektor V=SGLVektor(xCoordAim->value(),yCoordAim->value(),zCoordAim->value());

	glview->Camera->ViewMatr.outDated=true;
	glview->Camera->LookAt=V;
	glview->Camera->ReCalcUpVect();
	glview->Camera->Compile();
	glview->sendRedraw();
}

void GLvlView::setRollDeg(int deg)
{
	if(selfChange)return;
	assert(glview && glview->Camera);
	glview->Camera->ViewMatr.outDated=true;
	glview->Camera->UpVect=GLvlView::default_oben[0];
	glview->Camera->ReCalcUpVect(false,deg);
	glview->Camera->Compile();
	glview->sendRedraw();
}

void GLvlView::setCoordCam()
{
	if(selfChange)return;
	assert(glview && glview->Camera);
	SGLVektor V=SGLVektor(xCoordCam->value(),yCoordCam->value(),zCoordCam->value());

	glview->Camera->ViewMatr.outDated=true;
	glview->Camera->Pos=V;
	glview->Camera->ReCalcUpVect();
	glview->Camera->Compile();
	glview->sendRedraw();
}

/*!
    \fn GLvlView::onCamChanged()
 */
void GLvlView::onCamChanged()
{
	selfChange=true;
	assert(glview && glview->Camera);
	assert(tex->TexType==GL_TEXTURE_3D);
	SGLBaseCam &cam=*(glview->Camera);
	
	SGLVektor Pos=cam.Pos;
	SGLVektor LookAt=cam.LookAt;
	
	SGLVektor senkr=(cam.Pos-cam.LookAt).kreuzprod(SGLVektor(1,0,0));//Senkrechte auf Pos in Y-Z-Ebene liegend
	float winkel=cam.UpVect.SGLV_X<0 ? cam.UpVect.VektWink(senkr):-cam.UpVect.VektWink(senkr);
	rollDeg->setValue((int)winkel);

	xCoordCam->setValue((int)Pos.SGLV_X);
	yCoordCam->setValue((int)Pos.SGLV_Y);
	zCoordCam->setValue((int)Pos.SGLV_Z);
	
	xCoordAim->setValue((int)LookAt.SGLV_X);
	yCoordAim->setValue((int)LookAt.SGLV_Y);
	zCoordAim->setValue((int)LookAt.SGLV_Z);
	
	if(	GLvlView::default_oben[0].VektWink(cam.Pos)<5 &&
		GLvlView::default_oben[1].VektWink(cam.LookAt)<5 &&
		GLvlView::default_oben[2].VektWink(cam.UpVect)<5)
		viewSelector->setCurrentItem(0);
	else if(	GLvlView::default_unten[0].VektWink(cam.Pos)<5 &&
				GLvlView::default_unten[1].VektWink(cam.LookAt)<5 &&
				GLvlView::default_unten[2].VektWink(cam.UpVect)<5)
				viewSelector->setCurrentItem(1);
	else if(	GLvlView::default_rechts[0].VektWink(cam.Pos)<5 &&
				GLvlView::default_rechts[1].VektWink(cam.LookAt)<5 &&
				GLvlView::default_rechts[2].VektWink(cam.UpVect)<5)
				viewSelector->setCurrentItem(2);
	else if(	GLvlView::default_links[0].VektWink(cam.Pos)<5 &&
				GLvlView::default_links[1].VektWink(cam.LookAt)<5 &&
				GLvlView::default_links[2].VektWink(cam.UpVect)<5)
				viewSelector->setCurrentItem(3);
	else if(	GLvlView::default_vorn[0].VektWink(cam.Pos)<5 &&
				GLvlView::default_vorn[1].VektWink(cam.LookAt)<5 &&
				GLvlView::default_vorn[2].VektWink(cam.UpVect)<5)
				viewSelector->setCurrentItem(4);
	else if(	GLvlView::default_hinten[0].VektWink(cam.Pos)<5 &&
				GLvlView::default_hinten[1].VektWink(cam.LookAt)<5 &&
				GLvlView::default_hinten[2].VektWink(cam.UpVect)<5)
				viewSelector->setCurrentItem(5);
	selfChange=false;
}


/*!
    \fn GLvlView::selectView(int view)
 */
void GLvlView::selectView(int view)
{
	switch(view)
	{
		case 0:
			glview->Camera->Pos=GLvlView::default_oben[0];
			glview->Camera->LookAt=GLvlView::default_oben[1];
			glview->Camera->UpVect=GLvlView::default_oben[2];
			break;
		case 1:
			glview->Camera->Pos=GLvlView::default_unten[0];
			glview->Camera->LookAt=GLvlView::default_unten[1];
			glview->Camera->UpVect=GLvlView::default_unten[2];
			break;
		case 2:
			glview->Camera->Pos=GLvlView::default_rechts[0];
			glview->Camera->LookAt=GLvlView::default_rechts[1];
			glview->Camera->UpVect=GLvlView::default_rechts[2];
			break;
		case 3:
			glview->Camera->Pos=GLvlView::default_links[0];
			glview->Camera->LookAt=GLvlView::default_links[1];
			glview->Camera->UpVect=GLvlView::default_links[2];
			break;
		case 4:
			glview->Camera->Pos=GLvlView::default_vorn[0];
			glview->Camera->LookAt=GLvlView::default_vorn[1];
			glview->Camera->UpVect=GLvlView::default_vorn[2];
			break;
		case 5:
			glview->Camera->Pos=GLvlView::default_hinten[0];
			glview->Camera->LookAt=GLvlView::default_hinten[1];
			glview->Camera->UpVect=GLvlView::default_hinten[2];
			break;
		default: SGLprintWarning("Unbekannte Sicht %d",view);return;break;
	}
	glview->Camera->ViewMatr.outDated=true;
	glview->Camera->Compile();
	onCamChanged();
	glview->sendRedraw();
}

void	GLvlView::selectViewMode(int mode)
{
	switch(mode)
	{
		case 0:
			glview->resizeMode=SGLBaseCam::resizeView;break;
		case 1:
			glview->resizeMode=SGLBaseCam::moveCam;break;
		case 2:
			glview->resizeMode=SGLBaseCam::scaleView;break;
		default: SGLprintWarning("Unbekannter ViewMode %d",mode);return;break;
	}
}

/*!
    \fn GLvlView::loadCam()
 */
bool GLvlView::loadCfg()
{
	myCam=glview->Camera;
	ERegistry CamReg("Camera",myReg);
	myCam->Pos.SGLV_X=CamReg.getdVal("Pos.x",0);
	myCam->Pos.SGLV_Y=CamReg.getdVal("Pos.y",0);
	myCam->Pos.SGLV_Z=CamReg.getdVal("Pos.z",200);
	
	myCam->LookAt.SGLV_X=CamReg.getdVal("Aim.x",0);
	myCam->LookAt.SGLV_Y=CamReg.getdVal("Aim.y",0);
	myCam->LookAt.SGLV_Z=CamReg.getdVal("Aim.z",0);

	myCam->UpVect.SGLV_X=CamReg.getdVal("Up.x",0);
	myCam->UpVect.SGLV_Y=CamReg.getdVal("Up.y",1);
	myCam->UpVect.SGLV_Z=CamReg.getdVal("Up.z",0);
	
	myCam->ViewMatr.outDated=true;
	myCam->Compile();
	
	int mode;
	QString sMode=myReg->getsVal("ResizeMode",scaleMode->text(glview->resizeMode));
	for(mode=scaleMode->count()-1;mode>=0 && sMode!=scaleMode->text(mode);mode--);
	scaleMode->setCurrentItem(mode);
	selectViewMode(mode);
}


/*!
    \fn GLvlView::saveCam()
 */
bool GLvlView::saveCfg()
{
	ERegistry CamReg("Camera",myReg);
	if(!myCam)
	{
		SGLprintWarning("Kopie der Kamera fahlt, versuche aktuelle Kamera zu ermitteln");
		myCam=glview->Camera;
	}
	CamReg.setdVal("Pos.x",myCam->Pos.SGLV_X);
	CamReg.setdVal("Pos.y",myCam->Pos.SGLV_Y);
	CamReg.setdVal("Pos.z",myCam->Pos.SGLV_Z);

	CamReg.setdVal("Aim.x",myCam->LookAt.SGLV_X);
	CamReg.setdVal("Aim.y",myCam->LookAt.SGLV_Y);
	CamReg.setdVal("Aim.z",myCam->LookAt.SGLV_Z);

	CamReg.setdVal("Up.x",myCam->UpVect.SGLV_X);
	CamReg.setdVal("Up.y",myCam->UpVect.SGLV_Y);
	CamReg.setdVal("Up.z",myCam->UpVect.SGLV_Z);
	
	myReg->setsVal("ResizeMode",scaleMode->currentText());
}

void GLvlView::showOthersHere(bool toggle)
{
	if(glview->ObjLst.render_non_native!=toggle)
	{
		glview->ObjLst.render_non_native=toggle;
		glview->ObjLst.Compile();
		glview->updateGL();
	}
}

void GLvlView::closeEvent(QCloseEvent *e)
{
	if(!myReg->Suicide)
		saveCfg();
	delete myReg;
	e->accept();
}



SGLVektor GLvlView::default_oben[3]={SGLVektor(0,0,-200),SGLVektor(0,0,0),SGLVektor(0,1,0)};
SGLVektor GLvlView::default_unten[3]={SGLVektor(0,0,200),SGLVektor(0,0,0),SGLVektor(0,1,0)};

SGLVektor GLvlView::default_vorn[3]={SGLVektor(-200,0,0),SGLVektor(0,0,0),SGLVektor(0,1,0)};
SGLVektor GLvlView::default_hinten[3]={SGLVektor(200,0,0),SGLVektor(0,0,0),SGLVektor(0,1,0)};

SGLVektor GLvlView::default_rechts[3]={SGLVektor(0,200,0),SGLVektor(0,0,0),SGLVektor(0,0,-1)};
SGLVektor GLvlView::default_links[3]={SGLVektor(0,-200,0),SGLVektor(0,0,0),SGLVektor(0,0,1)};


/*!
    \fn GLvlPlaneView::mouseMovedInGL(QMouseEvent *e,SGLVektor weltKoord);
 */
void GLvlPlaneView::mouseMovedInGL(QMouseEvent *e,SGLVektor weltKoord)
{
	if(cursor->goTo(weltKoord))
	{
		bool here=showCursHereBtn->isOn(),there=showCursThereBtn->isOn();
		if(here && there)glview->sendRedraw();
		else if(here)glview->updateGL();
		else if(there)glview->sendRedrawOther();
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
		}
		if(followSegments->isEnabled() && followSegments->isChecked())
			onVoxel(tex->texKoord2texIndex(cursor->OldPos-GLvlVolumeTex::masteroffset));
		//@todo Wenn fang aus is, wird das oft unnötig ausgelöst
		//besser prüfen, ob sich das Voxel geändert hat
	}
}

void GLvlPlaneView::init()
{
	boost::shared_ptr<GLvlPlaneCam> cam(new GLvlPlaneCam(tex));
	show();
	glview->defaultCam(cam);
	
	glview->registerObj(cursor);

	cam->connect(glview,
		SIGNAL(pressedMouseMove(QMouseEvent *,float ,float )),
		SLOT(schieben(QMouseEvent *,float ,float )));
	cursor->connect( fangToggle, SIGNAL( toggled(bool) ),SLOT(setFang(bool)));
	cursor->connect( spinCursorSize, SIGNAL( valueChanged(int) ),SLOT(setSize(int)));
	
	cursor->setFang(fangToggle->isOn());
	cursor->setSize(spinCursorSize->value());

	glview->registerObj(cam->myPlane);

	connect(cam.get(),SIGNAL(camChanged()),SLOT(onCamChanged()));
	loadCfg();
}

void GLvlPlaneView::mouseDoubleClickEvent(QMouseEvent *e)
{
	short top_resize=0,bottom_resize=0;
	bool done=false;
	if(e->state() & Qt::ControlButton)
	{
		done=true;
		top_resize=numeric_limits<short>::max();
	}
	if(e->state() & Qt::ShiftButton)
	{
		done=true;
		bottom_resize=numeric_limits<short>::min();
	}
	if(done)
	{
		onResizeSegment(top_resize,bottom_resize);
		e->accept();
	}else 
	{
		newPinDlg dlg;
		if(dlg.exec()==QDialog::Accepted)
		{
			shared_ptr<GLvlPin> pin=
				shared_ptr<GLvlPin>(new GLvlPin(cursor->OldPos,dlg.pinNameEdit->text()));
			pin->setCamera(glview->Camera.get());
			glview->registerObj(pin);
			glview->sendShowObj(pin);
			Pins->push_back(pin);
		}
	}
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

void GLvlView::onReached(vincent::VBild_value h,unsigned short objs){}
void GLvlView::onMsg(QString msg,bool canskip){}
void GLvlView::onTransformEnd(){};

void GLvlPlaneView::showSegmentAt(unsigned int index){}
void GLvlPlaneView::resizeCurrSegment(short topdelta,short bottomdelta){}
void GLvlPlaneView::selectCurrSegment(){}

void GLvlPlaneView::wheelEvent ( QWheelEvent * e )
{
	short top_resize=0,bottom_resize=0;
	bool done=false;
	if(e->state() & Qt::ControlButton)
	{
		done=true;
		top_resize=e->delta() > 0 ? -1:1;
	}
	if(e->state() & Qt::ShiftButton)
	{
		done=true;
		bottom_resize=e->delta() > 0 ? -1:1;
	}
	if(done)
	{
		onResizeSegment(top_resize,bottom_resize);
		e->accept();
	}else e->ignore();
}

void GLvlPlaneView::mouseReleaseEvent(QMouseEvent * e )
{
	if(e->state()==Qt::LeftButton && !glview->MouseInfo.MovedPastDownBtn)
	{
		selectSegment();
		e->accept();
	}
}
