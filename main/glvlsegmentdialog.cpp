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


#include "glvlsegmentdialog.h"

GLvlSegmentDialog::GLvlSegmentDialog(VImage &_Img): 
SegmentDialog(NULL,NULL,FALSE, 0 ),Img(_Img)
{
}

GLvlSegmentDialog::~GLvlSegmentDialog()
{
}

/*$SPECIALIZATION$*/
void GLvlSegmentDialog::onSelectItem(QListViewItem*)
{
}

void GLvlSegmentDialog::bottomCapChanged(int)
{
}

void GLvlSegmentDialog::topCapChanged(int)
{
}

void GLvlSegmentDialog::onWatershedReady(bool)
{
}

void GLvlSegmentDialog::registerSegment(boost::shared_ptr<GLvlSegment> seg)
{
	new GLvlSegmentDialog::SegmentItem(segList,seg);
}

void GLvlSegmentDialog::selectMinima(boost::shared_ptr<GLvlMinima> min)
{
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