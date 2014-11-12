#include "glvlview.h"
#include "glvlmultiviewmanager.h"

#include <CoreUtils/singletons.hpp>
#include <DataStorage/io_factory.hpp>

GLvlView::GLvlView()
{
	setupUi(this);
	selfChange=false;
}

void GLvlView::setupSpace(QWidget *parent)
{
	GLvlMultiviewManager &manager=isis::util::Singletons::get<GLvlMultiviewManager,10>();
	glview = manager.createSharedSpace(parent);
	onGotFocus.setCam(glview->Camera);
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
// 	if(selfChange)return;
// 	assert(glview && glview->Camera);
// 	glview->Camera->ViewMatr.outDated=true;
// 	glview->Camera->UpVect=SGLVektor(0,1,0);
// 	glview->Camera->ReCalcUpVect(false,deg);
// 	glview->Camera->Compile();
// 	glview->sendRedraw();
#warning Implement me
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



void GLvlView::on_actionAxial_triggered(bool){selectView(GLvlView::axial);}
void GLvlView::on_actionCoronal_triggered(bool){selectView(GLvlView::coronal);}
void GLvlView::on_actionSagittal_triggered(bool){selectView(GLvlView::sagittal);}


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

// void GLvlView::on_actionOpen_file_triggered(bool)
// {
// 	BOOST_FOREACH(QString file,QFileDialog::getOpenFileNames(this,"Open image data","","*.nii"))
// 		BOOST_FOREACH(const isis::data::Image &img,isis::data::IOFactory::load(file.toStdString()))
// 			isis::util::Singletons::get<GLvlMultiviewManager,10>().addImage(img);
// }
// 


SGLVektor GLvlView::sagittal[3]={SGLVektor(-400,0,0),SGLVektor(0,0,0),SGLVektor(0,0,1)};
SGLVektor GLvlView::axial[3]={SGLVektor(0,0,400),SGLVektor(0,0,0),SGLVektor(0,-1,0)};
SGLVektor GLvlView::coronal[3]={SGLVektor(0,-400,0),SGLVektor(0,0,0),SGLVektor(0,0,1)};

//GLvlSegmentDialog* GLvlView::wshed=NULL;
//GLvlPinsDlg* GLvlView::pinsDlg=NULL;
//ConfigDlg* GLvlView::configDlg=NULL;
SGLshPtr<SGLBaseCam> GLvlView::activeCam;
