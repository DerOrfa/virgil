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
#include <EWorkOnCfgDlg.h>

#include <vista/VImage.h>
#include <qstatusbar.h> 
#include <qapplication.h> 

using namespace boost;
using namespace efc;

GLvlMasterView::GLvlMasterView(std::list<VImage> src):
GLvlView( NULL, shared_ptr<GLvlVolumeTex>(new GLvlVolumeTex) ,new EWndRegistry("overview",new ERegistry("GLvl"))),//uuuhh dirty :-D
rahmen(new SGLCube()),Pins(new shared_pin_list)
{
	setupSpace(new SGLqtSpace(glViewContainer));
	toolTabs->removePage(toolTabs->page(3));

	masterReg=myReg->Parent;//MasterRegistry von oben wieder rausfischen
	
	MasterImg=*src.begin();
	tex->Load3DImage(*src.begin());//Master-Textur
	GLvlVolumeTex::masteroffset=SGLVektor(-tex->Info.X.inner_mm_size/2,-tex->Info.Y.inner_mm_size/2,-tex->Info.Z.inner_mm_size/2);
	tex->calcMatr();
	tex->ResetTransformMatrix((const GLdouble*)tex->mm2tex_Matrix);

	std::list<VImage>::iterator i=++src.begin();
	if(i!=src.end())
		tex->loadTint(*i);
	
	viewsNeue_SichtAction->setEnabled(tex->valid);
	rahmen->setDiag(SGLVektor(0,0,0),tex->Info.size);
	rahmen->DrahtGitter(true);
	
	//Lichtabnahme komplett aus
	glview->StdLight->Abnahme.Linear=0;
	glview->StdLight->Abnahme.Quadratisch=0;
	glview->StdLight->CamLight();//StdLight is (hoffentlich immer) ein Cameralicht, die müssen nie neu generiert werden => änderungen werden nur duch reinit wirksam

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
	onCamChanged();
	setCaption("Übersicht");
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
		SGLprintError("Es lassen sich keine Planes erzeugen, da die Textur ungültig ist");
		return;
	}
	GLvlPlaneView *view =new GLvlPlaneView (mw,tex,hisReg,Pins);

	view->init();
	onNewSpace(view->glview);
	view->showInOthers(view->showInOthersBtn->isOn());
	view->showOthersHere(view->showOthersHereBtn->isOn());

	((GLvlView*)this)->connect(view,SIGNAL(onVoxel(unsigned int)),SLOT(showSegmentAt(unsigned int)));
	
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
void  GLvlMasterView::loadIntoWShed()
{

	switch(VPixelRepn(MasterImg))
	{
		case VUByteRepn:	
		{
			glview->SetQuality(0);
			v_transform = shared_ptr<vincent::transform>(new vincent::transform(MasterImg));
			((GLvlView*)this)->connect(&*v_transform,SIGNAL(reached(vincent::VBild_value ,unsigned short)),SLOT(onReached(vincent::VBild_value,unsigned short )));
			((GLvlView*)this)->connect(&*v_transform,SIGNAL(msg(QString,bool)),SLOT(onMsg(QString,bool)));
			((GLvlView*)this)->connect(&*v_transform,SIGNAL(end()),SLOT(onTransformEnd()));
			qApp->processEvents();//Nebenläufigkeit faken
			v_transform->init();
			qApp->processEvents();
			v_transform->operator()();
			qApp->processEvents();
			onTransformEnd();
		}break;
	}
}
	
void GLvlMasterView::onTransformEnd()
{
	tex->setupPal(1,255);//@todo sollten eigentlich die Originalen palettendaten sein
	GLvlMinima3D::setup(SGLVektor(tex->Info.X.Elsize,tex->Info.Y.Elsize,tex->Info.Z.Elsize),v_transform->last_erg);
	qApp->processEvents();
	
	map<vincent::lab_value,shared_ptr<GLvlMinima3D> >::iterator i=objs.end();
	for(unsigned int index = 0;
		index<GLvlMinimaBase::plist->size;
		index=i->second->end
	)
	{
		vincent::lab_value id=GLvlMinima3D::plist->operator[](index).wert;
		i=objs.insert(i,pair<vincent::lab_value,shared_ptr<GLvlMinima3D> >(id,shared_ptr<GLvlMinima3D>(new GLvlMinima3D(index))));
	}
	glview->sendRedraw();
	
	onMsg("Waterschedtransformation nach vincent abgeschlossen, " + QString::number(objs.size()) + " Segmente wurden registriert",false);
}

void GLvlMasterView::onReached(vincent::VBild_value h,unsigned short objs)
{
	if(h<255)
	{
		tex->setupPal(h+1,255);
		glview->sendRedraw();
	}
	onMsg("Stufe " + EString(h) + " erreicht, " + EString(objs) + " Objekte gefunden",true);
}

void GLvlMasterView::onMsg(QString msg,bool canskip)
{
	if(canskip){if(!qApp->tryLock())return;}
	else qApp->lock();
	statusBar()->message(msg);
	qApp->unlock();
	qApp->processEvents();
}


void GLvlMasterView::showSegmentAt(unsigned int index)
{
	if(v_transform && v_transform->last_erg && index<v_transform->last_erg->size())
	{
		vincent::lab_value id=v_transform->last_erg->at(index);
		if(id==vincent::WSHED_WSHED)return;
		map<vincent::lab_value,shared_ptr<GLvlMinima3D> >::iterator it=objs.find(id);
		
		if(it!=objs.end())
		{
			if(aktMinima!=it->second)
			{
				if(aktMinima)glview->unshowObj(aktMinima);
				if(it->second->size() <= MAX_MINIMA_SIZE)
				{
					EVektor<unsigned short> pos;
					pos.fromArray(3,it->second->minEdge.koord);
					tex->loadBitMask(
						*(it->second->genTex()),
						pos,it->second->color
					);
					glview->showObj(aktMinima=it->second);
					updatePlanes();
				}
			}
		}
		else {SGLprintWarning("Ungültiges Objekt %d",id);}
	}
}

