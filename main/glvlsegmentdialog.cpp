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

GLvlSegmentDialog::GLvlSegmentDialog(VImage &_Img): 
SegmentDialog(NULL,NULL,FALSE, 0 ),Img(_Img){}

GLvlSegmentDialog::~GLvlSegmentDialog(){}

/*$SPECIALIZATION$*/
void GLvlSegmentDialog::onSelectItem(QListViewItem*){}
void GLvlSegmentDialog::bottomCapChanged(int){}
void GLvlSegmentDialog::topCapChanged(int){}

void GLvlSegmentDialog::onWatershedReady(bool)
{
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
	new GLvlSegmentDialog::SegmentItem(segList,seg);
}

void GLvlSegmentDialog::setMinCapData(VUByte minValue,VUByte Value,VUByte maxValue)
{
	cap_slider_bottom->setValue(Value);
	cap_slider_bottom->setMinValue(minValue);
	cap_slider_bottom->setMaxValue(maxValue);
	
	cap_spinBox_bottom->setValue(Value);
	cap_spinBox_bottom->setMinValue(minValue);
	cap_spinBox_bottom->setMaxValue(maxValue);
	
	top_cap_min_text->setText(EString(minValue));
	top_cap_max_text->setText(EString(maxValue));
	
	bottom_cap_min_text->setText(EString(minValue));
	bottom_cap_max_text->setText(EString(maxValue));
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
			const GLvlMinima &m=*min->front();
			
			setMinCapData(m.bottomBorder(),m.bottomCap,m.topBorder());
			setMinCapData(m.bottomBorder(),m.topCap,m.topBorder());
			
			volume_tex->setText(QString::number(m.end-m.start));
		}
	}
}


GLvlSegmentDialog::MinimaItem::MinimaItem(QListViewItem *parent,boost::shared_ptr<GLvlMinima> min):
QListViewItem(parent),boost::shared_ptr<GLvlMinima>(min)
{}

GLvlSegmentDialog::SegmentItem::SegmentItem(QListView *parent,boost::shared_ptr<GLvlSegment> seg):
QListViewItem(parent),boost::shared_ptr<GLvlSegment>(seg)
{
	for(GLvlSegment::iterator i=operator*().begin();i!=operator*().end();i++)
		GLvlSegmentDialog::MinimaItem(this,*i);
}

void GLvlSegmentDialog::startTransform()
{

	switch(VPixelRepn(Img))
	{
		case VUByteRepn:	
		{
//			glview->SetQuality(0);
			startWShedBtn->setDown(true);
			v_transform = shared_ptr<vincent::transform>(new vincent::transform(GLvlMasterView::MasterImg));
			this->connect(&*v_transform,SIGNAL(reached(vincent::VBild_value ,unsigned short)),SLOT(onReached(vincent::VBild_value,unsigned short )));
			qApp->processEvents();//Nebenläufigkeit faken
			v_transform->init();
			qApp->processEvents();
			v_transform->operator()();
			qApp->processEvents();
			onWatershedReady(true);
			startWShedBtn->setDown(false);
		}break;
	}
}

/*!
    \fn GLvlSegmentDialog::findSegment(unsigned int index)
 */
void GLvlSegmentDialog::findMinima(unsigned int index)
{
	if(	index!= std::numeric_limits<unsigned int>::max() && 
		v_transform && 
		v_transform->last_erg && 
		index<v_transform->last_erg->size()
	)
	{
		vincent::lab_value id=v_transform->last_erg->at(index);
		if(id==vincent::WSHED_WSHED)return;
		map<vincent::lab_value,shared_ptr<GLvlSegment> >::iterator it=objs.find(id);
		if(it!=objs.end())
		{
			selectMinima(it->second);
		}
		else {SGLprintWarning("Ungültiges Objekt %d",id);}
	}
}


/*!
    \fn GLvlSegmentDialog::addAktMinimaToSegment()
 */
void GLvlSegmentDialog::addCurrMinima()
{
	if(!aktMinima)return;
	aktSegment->push_back(aktMinima->front());
	aktSegment->redisplay();
//	glview->sendRedraw();
}

void GLvlSegmentDialog::resizeCurrMinima(short topdelta,short bottomdelta)
{
	if(!aktMinima)return;
	aktMinima->front()->chCap(topdelta,bottomdelta);
	aktMinima->redisplay();
}

void GLvlSegmentDialog::onReached(vincent::VBild_value h ,unsigned short objs )
{
	qApp->processEvents();//Nebenläufigkeit faken
	startWShedBtn->setText("Stufe " + EString(h) + " erreicht, " + EString(objs) + " Objekte gefunden");
}

