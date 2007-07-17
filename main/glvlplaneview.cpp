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
#include "glvlmasterview.h"
#include "glvlpinsdlg.h"

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
	SGLshPtr<GLvlVolumeTex> tex,
	EWndRegistry *myReg
):
GLvlView(mw, tex,myReg),
cursor(new GLvlPlaneCursor()),
AimXStatus((QWidget*)statusBar()),
AimYStatus((QWidget*)statusBar()),
AimZStatus((QWidget*)statusBar())
{
	setupSpace(new SGLqtSpace(mw,glViewContainer));
	glview->resizeMode=SGLBaseCam::moveCam;
	
	fileExitAction->setMenuText("Sicht schließen");
	myReg->Suicide=true;
	connect(glview,SIGNAL(destroyed(QObject *)),SLOT(lostView()));
	connect(glview,SIGNAL(mouseMoved(QMouseEvent *,SGLVektor )),SLOT(mouseMovedInGL(QMouseEvent *,SGLVektor )));

	setCaption("Schnitt");
	
	AimXStatus.setMinimumSize(80,5);
	AimYStatus.setMinimumSize(80,5);
	AimZStatus.setMinimumSize(80,5);

	statusBar()->addWidget(&AimXStatus,10,true);
	statusBar()->addWidget(&AimYStatus,10,true);
	statusBar()->addWidget(&AimZStatus,10,true);
	
	AimXStatus.setText("0");
	AimYStatus.setText("0");
	AimZStatus.setText("0");
	
	GLvlMasterView::views.push_front(this);
	it=GLvlMasterView::views.begin();

}

GLvlPlaneView::~GLvlPlaneView()
{
	GLvlMasterView::views.erase(it);
}

bool GLvlPlaneView::loadCfg()
{
	schalterdiesen_Schnitt_in_anderen_Ansichten_zeigenAction->setOn(myReg->getbVal("Andere anzeigen",true));
	schalterandere_Schnitte_in_dieser_Ansicht_zeigenAction->setOn(myReg->getbVal("In anderen anzeigen",true));
	return GLvlView::loadCfg();
}

bool GLvlPlaneView::saveCfg()
{
	GLvlView::saveCfg();	
	if(schalterdiesen_Schnitt_in_anderen_Ansichten_zeigenAction->isVisible())
		myReg->setbVal("Andere anzeigen",schalterdiesen_Schnitt_in_anderen_Ansichten_zeigenAction->isOn());
	if(schalterandere_Schnitte_in_dieser_Ansicht_zeigenAction->isVisible())
		myReg->setbVal("In Anderen anzeigen",schalterandere_Schnitte_in_dieser_Ansicht_zeigenAction->isOn());
}



void GLvlPlaneView::showInOthers(bool toggle)
{
	SGLshPtr<GLvlPlaneCam> cam=dynamic_pointer_cast<GLvlPlaneCam>(glview->Camera);
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

GLvlView::GLvlView(SGLqtSpace* mw, SGLshPtr<GLvlVolumeTex> tex,EWndRegistry *myReg):
GLvlViewBase(NULL,NULL,mw?Qt::WDestructiveClose:0),//Das MasterWnd darf erst durch die app gelöscht werden
onGotFocus(myCam)
{
	selfChange=false;
	this->myReg=myReg;
	this->tex=tex;
	connect(dialogeselect_DatasetAction,SIGNAL(activated()),SLOT(selectDataDlg()));
}

void GLvlView::setupSpace(SGLqtSpace *space)
{
	glview = space;
	glview->registerDynamicTex(*tex);
	glview->gotFocus.connect(onGotFocus);
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
	
	const SGLVektor &Pos=cam.Pos;
	const SGLVektor &LookAt=cam.LookAt;
	
	const SGLVektor senkr=(cam.Pos-cam.LookAt).kreuzprod(SGLVektor(1,0,0));//Senkrechte auf Pos in Y-Z-Ebene liegend
	const float winkel=cam.UpVect.SGLV_X<0 ? cam.UpVect.VektWink(senkr):-cam.UpVect.VektWink(senkr);
	rollDeg->setValue((int)winkel);

	xCoordCam->setValue((int)Pos.SGLV_X);
	yCoordCam->setValue((int)Pos.SGLV_Y);
	zCoordCam->setValue((int)Pos.SGLV_Z);
	
	xCoordAim->setValue((int)LookAt.SGLV_X);
	yCoordAim->setValue((int)LookAt.SGLV_Y);
	zCoordAim->setValue((int)LookAt.SGLV_Z);
	
	selfChange=false;
}


void GLvlView::sichtVonHinten(){selectView(GLvlView::default_hinten);}
void GLvlView::sichtVonVorn(){selectView(GLvlView::default_vorn);}
void GLvlView::sichtVonOben(){selectView(GLvlView::default_oben);}
void GLvlView::sichtVonUnten(){selectView(GLvlView::default_unten);}
void GLvlView::sichtVonRechts(){selectView(GLvlView::default_rechts);}
void GLvlView::sichtVonLinks(){selectView(GLvlView::default_links);}


void GLvlView::selectView(const SGLVektor dir[3])
{
	glview->Camera->Pos=dir[0];
	glview->Camera->LookAt=dir[1];
	glview->Camera->UpVect=dir[2];
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
	return true;
}


/*!
    \fn GLvlView::saveCam()
 */
bool GLvlView::saveCfg()
{
	ERegistry CamReg("Camera",myReg);
	bool ret;
	if(!myCam)
	{
		SGLprintWarning("Kopie der Kamera fehlt, versuche aktuelle Kamera zu ermitteln");
		myCam=glview->Camera;
	}
	ret=CamReg.setdVal("Pos.x",myCam->Pos.SGLV_X);
	ret&=CamReg.setdVal("Pos.y",myCam->Pos.SGLV_Y);
	ret&=CamReg.setdVal("Pos.z",myCam->Pos.SGLV_Z);

	ret&=CamReg.setdVal("Aim.x",myCam->LookAt.SGLV_X);
	ret&=CamReg.setdVal("Aim.y",myCam->LookAt.SGLV_Y);
	ret&=CamReg.setdVal("Aim.z",myCam->LookAt.SGLV_Z);

	ret&=CamReg.setdVal("Up.x",myCam->UpVect.SGLV_X);
	ret&=CamReg.setdVal("Up.y",myCam->UpVect.SGLV_Y);
	ret&=CamReg.setdVal("Up.z",myCam->UpVect.SGLV_Z);
	
	ret&=myReg->setsVal("ResizeMode",scaleMode->currentText());
	return ret; 
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

//GLvlSegmentDialog* GLvlView::wshed=NULL;
GLvlPinsDlg* GLvlView::pinsDlg=NULL;
ConfigDlg* GLvlView::configDlg=NULL;
SGLshPtr<SGLBaseCam> GLvlView::activeCam;


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
/*		if(!Pins->empty())
		{
			SGLVektor len= weltKoord-Pins->back()->pos;
			QString lenStr=stellen?
				QString::number(len.Len(),'f',stellen):
				QString::number(int(len.Len()));
			statusBar()->message("Entfernung zum letzen Pin: "+lenStr+"mm");
		}*/
		if(followSegments->isEnabled() && followSegments->isOn())
			onVoxel(tex->texKoord2texIndex(cursor->OldPos-GLvlVolumeTex::masteroffset));
		//@todo Wenn fang aus is, wird das oft unnötig ausgelöst
		//besser prüfen, ob sich das Voxel geändert hat
	}
}

void GLvlPlaneView::init()
{
	SGLshPtr<GLvlPlaneCam> cam(new GLvlPlaneCam(tex));
	show();
	glview->defaultCam(cam);
	
	glview->registerObj(cursor);

	cam->connect(glview,
		SIGNAL(pressedMouseMoveRel(QMouseEvent *,float ,float )),
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
	VUByte  top_resize=0,bottom_resize=0;
	bool done=false;
		newPinDlg dlg;
		dlg.koord_x_text->setText(QString::number(cursor->OldPos.SGLV_X));
		dlg.koord_y_text->setText(QString::number(cursor->OldPos.SGLV_Y));
		dlg.koord_z_text->setText(QString::number(cursor->OldPos.SGLV_Z));
		if(dlg.exec()==QDialog::Accepted)
		{
			showPinsDlg(true);
			new GLvlPinsDlg::pinItem(glview,SGLVektor(
				dlg.koord_x_text->text().toFloat(),
				dlg.koord_y_text->text().toFloat(),
				dlg.koord_z_text->text().toFloat()),
				dlg.pinNameEdit->text()
			);
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

void GLvlView::onMsg(QString msg,bool canskip){}


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
	if(e->state()==(Qt::LeftButton | ControlButton) && !glview->MouseInfo.MovedPastDownBtn)
		jumpToCursor();
	else if(e->state()==Qt::LeftButton && !glview->MouseInfo.MovedPastDownBtn)
	{
		selectSegment();
		e->accept();
	}
}

void GLvlView::showConfigDlg(bool toggle)
{
	assert(configDlg);
	if(toggle)configDlg->show();
	else configDlg->hide();
}

void GLvlView::selectDataDlg()
{
	assert(GLvlMasterView::dataDialog);
	GLvlMasterView::dataDialog->show();
}

void GLvlView::showPinsDlg(bool toggle)
{
	assert(pinsDlg);
	if(toggle)pinsDlg->show();
	else pinsDlg->hide();
}


void GLvlPlaneView::jumpToCursor(){jumpTo(cursor->OldPos);}
void GLvlPlaneView::jumpTo(const SGLVektor &to)
{
	glview->Camera->MoveAimTo(to);
	glview->sendRedraw();
	onCamChanged();
}
