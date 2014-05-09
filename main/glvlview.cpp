#include "glvlview.h"
#include "glvlmultiviewmanager.h"
#include "isis/CoreUtils/singletons.hpp"

GLvlView::GLvlView():onGotFocus(myCam)
{
	setupUi(this);
	selfChange=false;
}

void GLvlView::setupSpace(QWidget *parent)
{
	GLvlMultiviewManager &manager=isis::util::Singletons::get<GLvlMultiviewManager,10>();
	glview = manager.createSharedSpace(parent);
	glview->gotFocus.connect(onGotFocus);
	glview->keyIgnore[Qt::Key_Escape]=true; // we dont want libsgl to close the widget when esc is pressed
	connect(glview,SIGNAL(camChanged()),SLOT(onCamChanged()));
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
	SGLBaseCam &cam=*(glview->Camera);

	const SGLVektor &Pos=cam.Pos;
	const SGLVektor &LookAt=cam.LookAt;

//	const SGLVektor senkr=(cam.Pos-cam.LookAt).kreuzprod(SGLVektor(1,0,0));//Senkrechte auf Pos in Y-Z-Ebene liegend
//	const float winkel=cam.UpVect.SGLV_X<0 ? cam.UpVect.VektWink(senkr):-cam.UpVect.VektWink(senkr);
//	rollDeg->setValue((int)winkel);

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
#warning "Implement me, pleeease"
/*	myCam=glview->Camera;
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
	*/

	myCam->ViewMatr.outDated=true;
	myCam->Compile();

	int mode;
/*	QString sMode=myReg->getsVal("ResizeMode",scaleMode->text(glview->resizeMode));
	for(mode=scaleMode->count()-1;mode>=0 && sMode!=scaleMode->text(mode);mode--);
	scaleMode->setCurrentItem(mode);
	selectViewMode(mode);*/
	return true;
}


/*!
	\fn GLvlView::saveCam()
 */
bool GLvlView::saveCfg()
{
#warning "implement me"
/*	ERegistry CamReg("Camera",myReg);
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
	*/
	return true;
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
/*	if(!myReg->Suicide)
		saveCfg();
	delete myReg;*/
	e->accept();
}

void GLvlView::on_actionNewPlane_triggered(bool)
{
    (new GLvlPlaneView)->show();
}


SGLVektor GLvlView::default_oben[3]={SGLVektor(0,0,-200),SGLVektor(0,0,0),SGLVektor(0,1,0)};
SGLVektor GLvlView::default_unten[3]={SGLVektor(0,0,200),SGLVektor(0,0,0),SGLVektor(0,1,0)};

SGLVektor GLvlView::default_vorn[3]={SGLVektor(-200,0,0),SGLVektor(0,0,0),SGLVektor(0,1,0)};
SGLVektor GLvlView::default_hinten[3]={SGLVektor(200,0,0),SGLVektor(0,0,0),SGLVektor(0,1,0)};

SGLVektor GLvlView::default_rechts[3]={SGLVektor(0,200,0),SGLVektor(0,0,0),SGLVektor(0,0,-1)};
SGLVektor GLvlView::default_links[3]={SGLVektor(0,-200,0),SGLVektor(0,0,0),SGLVektor(0,0,1)};

//GLvlSegmentDialog* GLvlView::wshed=NULL;
//GLvlPinsDlg* GLvlView::pinsDlg=NULL;
//ConfigDlg* GLvlView::configDlg=NULL;
SGLshPtr<SGLBaseCam> GLvlView::activeCam;
