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

#include <CoreUtils/singletons.hpp>

using namespace boost;

GLvlMasterView::GLvlMasterView()
{
	setObjectName("Overview");setWindowTitle("Overview");
	GLvlMultiviewManager &manager=isis::util::Singletons::get<GLvlMultiviewManager,10>();

	glViewContainer->setObjectName("MasterViewContainer");
	setupSpace(glViewContainer);
	glview->resizeMode=SGLBaseCam::scaleView;
	show();

//	((GLvlView*)this)->connect(fileSegmentierungAction,SIGNAL(activated()),SLOT(loadWShedDlg()));
//	((GLvlView*)this)->connect(viewsNeue_SichtAction,SIGNAL(activated()),SLOT(newPlane()));

	if(glview->StdLight){
		glview->StdLight->Abnahme.Linear=0;
		glview->StdLight->Abnahme.Quadratisch=0;
		glview->StdLight->CamLight();//StdLight is (hoffentlich immer) ein Cameralicht, die müssen nie neu generiert werden => Änderungen werden nur duch reinit wirksam
	}

	glview->setGridsSize(150);
	GLvlView::activeCam=glview->Camera;
	glview->Camera->MoveCamTo(SGLVektor(0,0,-300));
	glview->Camera->ResetUpVect(180);

/*	for(unsigned short i=0;i<Regs.size();i++)
		newPlane(new EWndRegistry(*Regs[i],masterReg));
*/
// 	GLvlSegment::setup(glview,tex);



/*	if(!GLvlView::configDlg)GLvlView::configDlg = new ConfigDlg;
	if(!GLvlView::pinsDlg)GLvlView::pinsDlg = new GLvlPinsDlg(this,glview);
*/
}


GLvlMasterView::~GLvlMasterView()
{
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


