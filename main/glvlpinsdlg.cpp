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


#include "glvlpinsdlg.h"
#include "glvlplaneview.h"
#include <libsgl/sglqtspace.h>

GLvlPinsDlg::GLvlPinsDlg(QWidget* parent) : pins(parent,0)
{}

void GLvlPinsDlg::deletePin()
{
	for(QListViewItemIterator it(pinList);it.current();it++)
	{
		GLvlPinsDlg::pinItem* pinItem=dynamic_cast<GLvlPinsDlg::pinItem*>(it.current());
		if(pinItem && pinItem->isSelected())
		{
			pinItem->mySpace.sendUnshowObj(*pinItem);
			pinItem->mySpace.unregisterObj(*pinItem);
			delete pinItem;
		}
	}
}



void GLvlPinsDlg::onSelChange()
{
	double len=0;
	QString trackStr;
	GLvlPinsDlg::pinItem* prev=NULL;
	for(QListViewItemIterator it(pinList);it.current();it++)
	{
		GLvlPinsDlg::pinItem* pinItem=dynamic_cast<GLvlPinsDlg::pinItem*>(it.current());
		if(pinItem && pinItem->isSelected())
		{
			if(prev)
			{
				trackStr += "-"+pinItem->text(0);
				len+=(pinItem->operator*().pos-prev->operator*().pos).Len();
			}
			else trackStr = pinItem->text(0);
			prev=pinItem;
		}
	}
	abst_path->setText(trackStr);
	abst_text->setText(QString::number(len)+"mm");
	
}

GLvlPinsDlg::pinItem::pinItem(SGLqtSpace * space,const SGLVektor &pos,const QString text):
QListViewItem(
	GLvlView::pinsDlg->pinList,
	text,
	QString::number(pos.SGLV_X),QString::number(pos.SGLV_Y),QString::number(pos.SGLV_Z)
),
mySpace(*space)
{
	this->reset(new GLvlPin(pos,text));
	operator*().setCamera(space->Camera.get());
	space->registerObj(*this);
	space->sendShowObj(*this);
}


GLvlPinsDlg::pinItem* GLvlPinsDlg::newPin(SGLqtSpace * space,const SGLVektor &pos,const QString text)
{
	return  new GLvlPinsDlg::pinItem(space,pos,text);
}
