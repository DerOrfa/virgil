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
#include "glvlmasterview.h"
#include <qpushbutton.h>
#include <qaction.h> 
#include <qtabwidget.h>
#include "glvlplanecam.h"
#include <eclasses/EWorkOnCfgDlg.h>

#include <vista/VImage.h>
#include <qstatusbar.h> 
#include <qapplication.h> 
#include <qcheckbox.h>

using namespace boost;
using namespace efc;

GLvlMasterView::GLvlMasterView(std::list<VImage> src):
GLvlView( NULL, shared_ptr<GLvlVolumeTex>(new GLvlVolumeTex) ,new EWndRegistry("overview",new ERegistry("GLvl"))),//uuuhh dirty :-D
rahmen(new SGLCube()),Pins(new shared_pin_list)
{
	setupSpace(new SGLqtSpace(glViewContainer));
	toolTabs->removePage(toolTabs->page(3));
	followSegments->setDisabled(true);

	masterReg=myReg->Parent;//MasterRegistry von oben wieder rausfischen
	
	MasterImg=*src.begin();
	tex->Load3DImage(MasterImg);//Master-Textur
	GLvlVolumeTex::masteroffset=SGLVektor(-tex->Info.X.inner_mm_size/2,-tex->Info.Y.inner_mm_size/2,-tex->Info.Z.inner_mm_size/2);
	tex->calcMatr();
	tex->ResetTransformMatrix((const GLdouble*)tex->mm2tex_Matrix);

	std::list<VImage>::iterator i=++src.begin();
	if(i!=src.end())
		tex->loadTint(*i);
	
	viewsNeue_SichtAction->setEnabled(tex->valid);
	rahmen->setDiag(GLvlVolumeTex::masteroffset,tex->Info.size+GLvlVolumeTex::masteroffset);
	rahmen->DrahtGitter(true);
	
	//Lichtabnahme komplett aus
	glview->StdLight->Abnahme.Linear=0;
	glview->StdLight->Abnahme.Quadratisch=0;
	glview->StdLight->CamLight();//StdLight is (hoffentlich immer) ein Cameralicht, die m�ssen nie neu generiert werden => �nderungen werden nur duch reinit wirksam

	glview->setGridsSize((tex->Info.size.SGLV_X >? tex->Info.size.SGLV_Y >? tex->Info.size.SGLV_Z)*1.1);
	glview->resizeMode=SGLBaseCam::scaleView;
	glview->registerObj(rahmen);

	mw = glview;
	onNewSpace(mw);

	toolTabs->removePage(toolTabs->page(4));
	toolTabs->removePage(toolTabs->page(4));
	loadCfg();
	EArray<EString> Regs=masterReg->findSubKeyStr(QRegExp("planeview"));
	
	SGLprintState("Lade %d %s",Regs.size(), (Regs.size()==1 ? " Schnitt":" Schnitte"));
	for(int i=0;i<Regs.size();i++)
		newPlane(new EWndRegistry(*Regs[i],masterReg));
		
	GLvlSegment::setup(glview,tex);
	
	onCamChanged();
	setCaption("�bersicht");
}


GLvlMasterView::~GLvlMasterView()
{
	delete masterReg;
}


/*$SPECIALIZATION$*/
void GLvlMasterView::newPlane()
{
	int i;
	EArray<EString> Regs=masterReg->findSubKeyStr(QRegExp("planeview"));
	for(i=1;Regs.findVal(EString("planeview "+EString(i)))>=0;i++);
	newPlane(new EWndRegistry("planeview "+EString(i),masterReg));
}
void GLvlMasterView::newPlane(EWndRegistry *hisReg)
{
	if(!tex->valid)
	{
		SGLprintError("Es lassen sich keine Planes erzeugen, da die Textur ung�ltig ist");
		return;
	}
	GLvlPlaneView *view =new GLvlPlaneView (mw,tex,hisReg,Pins);

	view->init();
	onNewSpace(view->glview);
	view->showInOthers(view->showInOthersBtn->isOn());
	view->showOthersHere(view->showOthersHereBtn->isOn());

	shared_ptr<GLvlPlaneCam> cam= boost::dynamic_pointer_cast<GLvlPlaneCam>(view->glview->Camera);
	if(!cam){SGLprintError("Die Kamera des Planeview ist keine PlaneCam??");return;}

	updatePlanes.connect(cam->myPlane->compileNextTime);
	view->onCamChanged();
}

void GLvlMasterView::closeEvent(QCloseEvent *e)
{
	removeAllChilds();
	GLvlView::closeEvent(e);
}


/*!
    \fn GLvlMasterView::doConfig()
 */
void GLvlMasterView::doConfig()
{
	EWorkOnCfgDlg tDlg(masterReg);
	tDlg.DoModal();
}


/*!
    \fn GLvlMasterView::doBenchmark()
 */
void GLvlMasterView::doBenchmark(time_t benchLen)
{
	time_t t=time(NULL);
	bool contBenchm=true;
	while(contBenchm)
	{
		if(childs.size()<=1)
		{
			SGLprintWarning("Der Benchmark macht nur Sinn, wenn mindestens ein Schnitt existiert. Ich lege einen an!");
			newPlane();
		}
		QValueList<SGLqtSpace *>::iterator it=childs.begin();
		while(it!=childs.end())
		{
			SGLqtSpace *i= *it;
			if(i!=glview)
			{
				i->benchmark();
				if(i->benchTime-benchLen>t)contBenchm=false;
			}
			else glview->updateGL();
			it++;
		}
	}
}
void GLvlMasterView::doBenchmark(){	doBenchmark(5);}


/*!
    \fn GLvlMasterView::loadIntoWShed()
 */
void  GLvlMasterView::loadWShedDlg()
{
	wshed = boost::shared_ptr<GLvlSegmentDialog>(new GLvlSegmentDialog(MasterImg));
	for(list<GLvlPlaneView *>::iterator i=views.begin();i!=views.end();i++)
	{
		wshed->connect(*i,SIGNAL(onVoxel(unsigned int)),SLOT(findMinima(unsigned int)));
		wshed->connect(*i,SIGNAL(onResizeSegment(short,short)),SLOT(resizeCurrMinima(short,short)));
		wshed->connect(*i,SIGNAL(selectSegment()),SLOT(addCurrMinima()));
	}
	wshed->show();
}
	

void GLvlMasterView::onMsg(QString msg,bool canskip)
{
	if(canskip){if(!qApp->tryLock())return;}
	else qApp->lock();
	statusBar()->message(msg);
	qApp->unlock();
	qApp->processEvents();
}

// void GLvlMasterView::MemCreateNotify::operator()(const MemConsumer &newob) const
// {
// 	cout << "Ob " << &newob << " erzeugt " << MemConsumer::list.size() << " Consumer registriert" << endl;
// }
// void GLvlMasterView::MemDeleteNotify::operator()(const MemConsumer &newob) const
// {
// 	cout << "Ob " << &newob << " gel�scht " << MemConsumer::list.size() << " Consumer registriert" << endl;
// }

VImage GLvlMasterView::MasterImg=NULL;
list<GLvlPlaneView *> GLvlMasterView::views;
