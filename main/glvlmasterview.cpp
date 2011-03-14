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
#include "glvlplanecam.h"

#include "glvlmultiviewmanager.h"

#include <qstatusbar.h>
#include <qapplication.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qaction.h>
#include <qtabwidget.h>

#include <isis/CoreUtils/singletons.hpp>

using namespace boost;

GLvlMasterView::GLvlMasterView():GLvlView( NULL ),onDataSelect(this)
{
	setupSpace(new SGLqtSpace(glViewContainer));

//	((GLvlView*)this)->connect(fileSegmentierungAction,SIGNAL(activated()),SLOT(loadWShedDlg()));
//	((GLvlView*)this)->connect(viewsNeue_SichtAction,SIGNAL(activated()),SLOT(newPlane()));

	//Lichtabnahme komplett aus
/*	if(glview->StdLight){
		glview->StdLight->Abnahme.Linear=0;
		glview->StdLight->Abnahme.Quadratisch=0;
		glview->StdLight->CamLight();//StdLight is (hoffentlich immer) ein Cameralicht, die müssen nie neu generiert werden => änderungen werden nur duch reinit wirksam
	}
*/

	mw = glview;
	isis::util::Singletons::get<GLvlMultiviewManager,10>().onNewSpace(mw);

/*	for(unsigned short i=0;i<Regs.size();i++)
		newPlane(new EWndRegistry(*Regs[i],masterReg));
*/
// 	GLvlSegment::setup(glview,tex);

	GLvlView::activeCam=glview->Camera;

/*	if(!GLvlView::configDlg)GLvlView::configDlg = new ConfigDlg;
	if(!GLvlView::pinsDlg)GLvlView::pinsDlg = new GLvlPinsDlg(this,glview);
*/
//	GLvlMasterView::dataDialog->onSelect.connect(onDataSelect);
}


GLvlMasterView::~GLvlMasterView()
{
}


/*$SPECIALIZATION$*/
void GLvlMasterView::newPlane()
{
	if(!tex->valid)
	{
		SGLprintError("Cannot create Plane, do valid data available");
		return;
	}
        GLvlPlaneView *view =new GLvlPlaneView (mw);

//	((GLvlView*)this)->connect(view->fileSegmentierungAction,SIGNAL(activated()),SLOT(loadWShedDlg()));
//	((GLvlView*)this)->connect(view->viewsNeue_SichtAction,SIGNAL(activated()),SLOT(newPlane()));

	view->init();
	isis::util::Singletons::get<GLvlMultiviewManager,10>().onNewSpace(view->glview);
	view->showInOthers(view->actionShowThisInOthers->isChecked());
	view->showOthersHere(view->actionShowOthersHere->isChecked());

	SGLshPtr<GLvlPlaneCam> cam= boost::dynamic_pointer_cast<GLvlPlaneCam>(view->glview->Camera);
	if(!cam){SGLprintError("Die Kamera des Planeview ist keine PlaneCam??");return;}

	updatePlanes.connect(cam->myPlane->compileNextTime);
	view->onCamChanged();
}

void GLvlMasterView::closeEvent(QCloseEvent *e)
{
	isis::util::Singletons::get<GLvlMultiviewManager,10>().removeAllChilds();
	GLvlView::closeEvent(e);
}


/*!
	\fn GLvlMasterView::doConfig()
 */
void GLvlMasterView::doConfig()
{
#warning "Implement me";

}


/*!
	\fn GLvlMasterView::doBenchmark()
 */
void GLvlMasterView::doBenchmark(time_t benchLen)
{
#warning "Implement me"
/*	time_t t=time(NULL);
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
	}*/
}
void GLvlMasterView::doBenchmark(){	doBenchmark(5);}


/*void  GLvlMasterView::loadWShedDlg()
{
	if(!wshed)
	{
		wshed = new GLvlSegmentDialog(this,MasterImg);
		for(list<GLvlPlaneView *>::iterator i=views.begin();i!=views.end();i++)
		{
			wshed->connect(*i,SIGNAL(onVoxel(unsigned int)),SLOT(findMinima(unsigned int)));
			wshed->connect(*i,SIGNAL(onResizeSegment(signed char ,signed char)),SLOT(chCapRelCurrMinima(signed char ,signed char )));
			wshed->connect(*i,SIGNAL(selectSegment()),SLOT(addCurrMinima()));
		}
	}
	wshed->show();
}*/


void GLvlMasterView::onMsg(QString msg,bool canskip)
{
#warning "fix me"
/*	if(canskip){if(!qApp->tryLock())return;}
	else qApp->lock();
	statusBar()->message(msg);
	qApp->unlock();
	qApp->processEvents();*/
}


/*!
	\fn GLvlMasterView::loadData(FileIO::ProtocolDataMap::iterator)
 */
/*bool GLvlMasterView::loadData()
{
  Data<GLubyte,4> dat;i->second.convert_to(dat);
	loadData(i->first,dat);
	glview->registerDynamicTex(*tex);
        actionNewPlane->setEnabled(tex->valid);

    rahmen->setDiag(GLvlVolumeTex::masteroffset,tex->Info.size+GLvlVolumeTex::masteroffset);
    rahmen->DrahtGitter(true);
    glview->setGridsSize(
            int(std::max(std::max(tex->Info.size.SGLV_X, tex->Info.size.SGLV_Y), tex->Info.size.SGLV_Z))
            *.6);
    glview->resizeMode=SGLBaseCam::scaleView;
    glview->registerObj(rahmen);
}
	*/

GLvlMasterView::SelectSlot::SelectSlot(GLvlMasterView* p):master(p){}
void GLvlMasterView::SelectSlot::operator()(/*Protocol prot,Data<float,4> dat*/)
{
	qWarning("Implement me");
 /* Data<GLubyte,4> data;dat.convert_to(dat);
  master->loadData(prot,data);*/
}
