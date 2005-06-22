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
#include "glvlrule.h"
#include "glvlplaneview.h"
#include <libsgl/sglqtspace.h>

GLvlPinsDlg::GLvlPinsDlg(QWidget* parent,SGLqtSpace *masterSpace) : pins(parent,0),space(masterSpace)
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
	for(list<QListViewItemIterator>::iterator selIt=selPinList.begin();selIt!=selPinList.end();)
		if(selIt->current()->isSelected())selIt++;
		else selIt=selPinList.erase(selIt);
	for(QListViewItemIterator listIt(pinList);listIt.current();listIt++)
	{
		GLvlPinsDlg::pinItem* pinItem=dynamic_cast<GLvlPinsDlg::pinItem*>(listIt.current());
		if(pinItem && pinItem->isSelected())
		{
			bool found=false;
			for(list<QListViewItemIterator>::iterator selIt=selPinList.begin();selIt!=selPinList.end();selIt++)
				if(selIt->current()==listIt.current()){
					found=true;break;
				}
			if(!found)selPinList.push_back(listIt);
		}
	}
	measure();
}

GLvlPinsDlg::pinItem::pinItem(SGLqtSpace * space,const SGLVektor &pos,const QString text):
	QListViewItem(
		GLvlView::pinsDlg->pinList,text,
		QString::number(pos.SGLV_X),
		QString::number(pos.SGLV_Y),
		QString::number(pos.SGLV_Z)
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


/*!
    \fn GLvlPinsDlg::measure()
 */
void GLvlPinsDlg::measure()
{
	double len=0;
	
	for(list<SGLshPtr<GLvlRuler> >::iterator i=rulers.begin();i!=rulers.end();i++)
	{
		space->sendUnshowObj(*i);
		space->unshowObj(*i);
	}
	rulers.clear();
	if(selPinList.size()>1)
	{
		list<QListViewItemIterator>::iterator it=selPinList.begin();
		QString pathText="["+it->current()->text(0)+"]";
		GLvlPinsDlg::pinItem* prev=dynamic_cast<GLvlPinsDlg::pinItem*>(it->current());
		while((++it)!=selPinList.end())
		{
			GLvlPinsDlg::pinItem *const curr=dynamic_cast<GLvlPinsDlg::pinItem*>(it->current());
			if(curr){
				pathText+="-["+curr->text(0)+"]";
				len+=((*curr)->pos-(*prev)->pos).Len();
				
				rulers.push_back(SGLshPtr<GLvlRuler>(new GLvlRuler((*prev)->pos,(*curr)->pos)));
				space->showObj(rulers.back());
				space->sendShowObj(rulers.back());
	
				prev=curr;
			}
		}
		abst_path->setText(pathText);
		abst_text->setText(QString::number(len)+"mm");
	}	
}
