//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "glvlvolumetex.h"
#include "glvlsegmentdialog.h"
#include "glvlmasterview.h"
#include <eclasses/EString.h>

#include <qapplication.h> 
#include <qpushbutton.h> 
#include <qslider.h> 
#include <qspinbox.h>
#include <qcheckbox.h> 

GLvlSegmentDialog::GLvlSegmentDialog(QWidget* parent,VImage &_Img): 
SegmentDialog(parent,NULL,FALSE, Qt::WDestructiveClose),Img(_Img),aktSegment(NULL)
{
}

GLvlSegmentDialog::~GLvlSegmentDialog()
{
	assert(GLvlView::wshed==this);
	GLvlView::wshed=NULL;
}

/*$SPECIALIZATION$*/
void GLvlSegmentDialog::onSelectItem(QListViewItem* it)
{
	SegmentItem* seg=dynamic_cast<SegmentItem*>(it);
	MinimaItem* min=dynamic_cast<MinimaItem*>(it);
	if(seg)
	{
		if(aktSegment!=seg)
		{
			if(aktSegment)aktSegment->undisplay();
			aktSegment=seg;
			aktSegment->display();
		}
	}
	else if(min)
	{
		SegmentItem* seg=dynamic_cast<SegmentItem*>(min->parent());
		if(seg && aktSegment!=seg)
		{
			aktSegment->undisplay();
			aktSegment=seg;
			aktSegment->display();
		}
		findMinima((*min)->getID());
	}
}

void GLvlSegmentDialog::onWatershedReady(bool)
{
	onMsg("registriere Minima",true);
	GLvlMinima::setup(
		SGLVektor(GLvlSegment::targetTex->Info.X.getElsize('X'),GLvlSegment::targetTex->Info.Y.getElsize('Y'),GLvlSegment::targetTex->Info.Z.getElsize('Z')),
		*v_transform,GLvlMasterView::MasterImg
	);
	map<vincent::lab_value,shared_ptr<GLvlSegment> >::iterator i=objs.end();
	for(unsigned int index = 0;
		index<GLvlMinima::plist->size;
		index=i->second->front()->end
	)
	{
		vincent::lab_value id=GLvlMinima::plist->operator[](index).wert;
		i=objs.insert(i,pair<vincent::lab_value,shared_ptr<GLvlSegment> >(id,shared_ptr<GLvlSegment>(new GLvlSegment(index))));
	}
//	onMsg("Waterschedtransformation nach vincent abgeschlossen, " + QString::number(objs.size()) + " Segmente wurden registriert",false);
}

void GLvlSegmentDialog::registerSegment(boost::shared_ptr<GLvlSegment> seg)
{
	if(aktSegment)aktSegment->undisplay();
	aktSegment=new SegmentItem(segList,seg->front());
	segList->setSelected(aktSegment,true);
}

void GLvlSegmentDialog::displayMinimaData(const GLvlMinima &m)
{
#define SET_CAP_DATA(TARGET,MIN,VAL,MAX)		\
TARGET ## _cap_slider->setMinValue(MIN);		\
TARGET ## _cap_slider->setMaxValue(MAX);		\
TARGET ## _cap_slider->setValue(VAL);			\
												\
TARGET ## _cap_min_text->setText(EString(MIN));	\
TARGET ## _cap_max_text->setText(EString(MAX));	\

	SET_CAP_DATA(bottom,m.bottomBorder(),m.bottomCap,m.topBorder()-1);
	SET_CAP_DATA(top,m.bottomBorder()+1,m.topCap,m.topBorder());
	incl_wshed->setChecked(m.incl_wshed);
	cap_text->setText(EString(m.bottomCap)+"-"+EString(m.topCap));
			
	volume_tex->setText(QString::number(m.volume_mm()) +"mm³ ("+ QString::number(m.volume()) + " Voxel)");
	unsigned short x,y,z;m.getPktKoord(0,x,y,z);
	coord_text->setText(EString(x)+" "+EString(y)+" "+EString(z));

	boundig_box_text->setText(EString(m.minEdge.x)+" "+EString(m.minEdge.y)+" "+EString(m.minEdge.z)+" - " + EString(m.maxEdge.x)+" "+EString(m.maxEdge.y)+" "+EString(m.maxEdge.z));
	
	qApp->processEvents();
#undef SET_CAP_DATA
}

void GLvlSegmentDialog::selectMinima(boost::shared_ptr<GLvlSegment> min)
{
	
	if(aktMinima!=min)
	{
		if(aktMinima)
			aktMinima->undisplay();
			
		if(min->display())
		{
			aktMinima=min;
			displayMinimaData(*min->front());
		}
	}
}


GLvlSegmentDialog::MinimaItem::MinimaItem(QListViewItem *parent,boost::shared_ptr<GLvlMinima> min):
QListViewItem(parent,"Minima"+QString::number(min->getID())),boost::shared_ptr<GLvlMinima>(min)
{
	showInf();
}

GLvlSegmentDialog::SegmentItem::SegmentItem(QListView *parent,boost::shared_ptr<GLvlMinima> min):
QListViewItem(parent,"Segment"+QString::number(min->getID())),GLvlSegment(min)
{
	new GLvlSegmentDialog::MinimaItem(this,min);
	isMinima=false;
	display();
	showInf();
}

void GLvlSegmentDialog::SegmentItem::addMinima(boost::shared_ptr<GLvlMinima> min)
{
	push_back(min);
	new GLvlSegmentDialog::MinimaItem(this,min);
	redisplay();
	showInf();
}

void GLvlSegmentDialog::SegmentItem::showInf()
{
	setText(2,
	EString(minEdge.x)+" "+
	EString(minEdge.y)+" "+
	EString(minEdge.z)+" - "+ 
	EString(maxEdge.x)+" "+
	EString(maxEdge.y)+" "+
	EString(maxEdge.z));
	
	double mm=0;
	
	for(GLvlSegment::iterator i=begin();i!=end();i++)
		mm+=(*i)->volume_mm();

	setText(1,QString::number(mm));
}

void GLvlSegmentDialog::MinimaItem::showInf()
{
	const GLvlMinima &m=**this;
	setText(2,
	EString(m.minEdge.x)+" "+
	EString(m.minEdge.y)+" "+
	EString(m.minEdge.z)+" - "+ 
	EString(m.maxEdge.x)+" "+
	EString(m.maxEdge.y)+" "+
	EString(m.maxEdge.z));
	
	setText(1,QString::number(m.volume_mm()));
}

void GLvlSegmentDialog::startTransform()
{
	startWShedBtn->hide();
	switch(VPixelRepn(Img))
	{
		case VUByteRepn:	
		{
//			glview->SetQuality(0);
			v_transform = shared_ptr<vincent::transform>(new vincent::transform(GLvlMasterView::MasterImg));
			this->connect(&*v_transform,SIGNAL(reached(vincent::VBild_value ,unsigned short)),SLOT(onReached(vincent::VBild_value,unsigned short )));
			this->connect(&*v_transform,SIGNAL(msg(QString,bool)),SLOT(onMsg(QString,bool)));
			qApp->processEvents();//Nebenläufigkeit faken
			v_transform->init();
			qApp->processEvents();
			v_transform->operator()();
			qApp->processEvents();
			onWatershedReady(true);
		}break;
	}
}

void GLvlSegmentDialog::onMsg(QString text,bool)
{
	wshed_status_inf->setText(text);
	qApp->processEvents();//Nebenläufigkeit faken
}

void GLvlSegmentDialog::findMinima(unsigned int index)
{
	if(	index!= std::numeric_limits<unsigned int>::max() && 
		v_transform && 
		v_transform->last_erg && 
		index<v_transform->last_erg->size()
	)
	findMinima(v_transform->last_erg->at(index));
}

void GLvlSegmentDialog::findMinima(vincent::lab_value id)
{
		if(id==vincent::WSHED_WSHED)return;
		map<vincent::lab_value,shared_ptr<GLvlSegment> >::iterator it=objs.find(id);
		if(it!=objs.end())
		{
			selectMinima(it->second);
		}
		else {SGLprintWarning("Ungültiges Objekt %d",id);}
}

void GLvlSegmentDialog::addCurrMinima()
{
	if(!aktMinima)return;
	if(!aktSegment)registerSegment(aktMinima);
	else aktSegment->addMinima(aktMinima->front());
//	glview->sendRedraw();
}

void GLvlSegmentDialog::chCapRelCurrMinima(signed char topdelta,signed char bottomdelta)
{
	if(!aktMinima)return;
	if(aktMinima->front()->chCapRel(topdelta,bottomdelta))
	{
// 		redisplayMinima(aktMinima);
		displayMinimaData(*aktMinima->front());
	}
}

void GLvlSegmentDialog::setTopCapAbsCurrMinima(int top)
{
	if(!aktMinima)return;
	if(aktMinima->front()->chCapAbsTop(top))
	{
// 		redisplayMinima(aktMinima);
		displayMinimaData(*aktMinima->front());
	}
}

void GLvlSegmentDialog::setBottomCapAbsCurrMinima(int bottom)
{
	if(!aktMinima)return;
	if(aktMinima->front()->chCapAbsBottom(bottom))
	{
// 		redisplayMinima(aktMinima);
		displayMinimaData(*aktMinima->front());
	}
}

void GLvlSegmentDialog::onReached(vincent::VBild_value h ,unsigned short objs )
{
	qApp->processEvents();//Nebenläufigkeit faken
	onMsg("Stufe " + EString(h) + " erreicht, " + EString(objs) + " Objekte gefunden",true);
}

void GLvlSegmentDialog::newSegment()
{
	if(aktSegment && segList->isSelected(aktSegment))segList->setSelected(aktSegment,false);
	aktSegment->undisplay();
	aktSegment=NULL;
}


/*!
    \fn GLvlSegmentDialog::redisplayMinima(const boost::shared_ptr<GLvlSegment> &min)
 */
// void GLvlSegmentDialog::redisplayMinima(boost::shared_ptr<GLvlSegment> &min)
// {
// 	assert(min->isMinima);
// 	min->redisplay();
// 	
// 	for(QListViewItemIterator it(segList);it.current();it++)
// 	{
// 		MinimaItem* minItem=dynamic_cast<MinimaItem*>(it.current());
// 		if(minItem && *minItem == min->front())
// 		{
// 			SegmentItem* seg=dynamic_cast<SegmentItem*>(minItem->parent());
// 			if(seg)seg->redisplay(true);
// 			else {SGLprintError("Konnte Segment des Minima 0x%x nicht ermitteln",minItem);}
// 		}
// 	}
// }

bool GLvlSegmentDialog::isMinimaInSegm(const boost::shared_ptr<GLvlSegment> &min)
{
	assert(min->isMinima);
	for(QListViewItemIterator it(segList);it.current();it++)
	{
		MinimaItem* minItem=dynamic_cast<MinimaItem*>(it.current());
		if(minItem && *minItem == min->front())return true;
	}
	return false;
}
/*!
    \fn GLvlSegmentDialog::toggleWShed(bool)
 */
void GLvlSegmentDialog::toggleWShed(bool toggle)
{
	if(!aktMinima)return;
	if(aktMinima->front()->incl_wshed != toggle)
	{
		aktMinima->front()->incl_wshed=toggle;
		aktMinima->redisplay();
		displayMinimaData(*aktMinima->front());
	}
}
