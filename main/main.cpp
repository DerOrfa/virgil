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
#include "main.h"

#include <qapplication.h>
#include <list>

#include <exception>
#include <isis/DataStorage/io_factory.hpp>
#include <isis/CoreUtils/singletons.hpp>
#include "glvlmultiviewmanager.h"
#include <QFileOpenEvent>

class EventHandler : public QObject
{
public:
	EventHandler( QObject *parent=NULL):QObject(parent){}
protected:
     bool eventFilter(QObject *obj, QEvent *event){
		switch(event->type())
		{
		case QEvent::FileOpen:{
				event->accept();
				BOOST_FOREACH(const isis::data::Image &img,isis::data::IOFactory::load(static_cast<QFileOpenEvent*>(event)->file().toStdString()))
					isis::util::Singletons::get<GLvlMultiviewManager,10>().addImage(img);
				return true;
			}break;
		default:return QObject::eventFilter(obj, event);
		}
	}
};

int main( int argc, char ** argv )
{
	ENABLE_LOG( Runtime, isis::util::DefaultMsgPrint, isis::warning );
	ENABLE_LOG( Debug, isis::util::DefaultMsgPrint, isis::info );
	std::set_terminate(__gnu_cxx::__verbose_terminate_handler);

	QApplication a(argc,argv);
	EventHandler *handle= new EventHandler(&a);
	a.installEventFilter(handle);

	unsigned short verbose = 3;
	switch(verbose)
	{
	case 0:SGLshowState=false;
	case 1:SGLshowWarnings=false;
	case 2:SGLshowInfos=false;
	default:SGLshowInfos=SGLshowWarnings=SGLshowState=true;
	}

	if(argc<=1){
		BOOST_FOREACH(QString file,QFileDialog::getOpenFileNames(NULL,"Open image data","","*.nii"))
			BOOST_FOREACH(const isis::data::Image &img,isis::data::IOFactory::load(file.toStdString()))
				isis::util::Singletons::get<GLvlMultiviewManager,10>().addImage(img);

	} else 
		BOOST_FOREACH(const isis::data::Image &img,isis::data::IOFactory::load(argv[1]))
			isis::util::Singletons::get<GLvlMultiviewManager,10>().addImage(img);

	SGLprintState("Initialisiere Schnittstelle ...");
	(new GLvlMasterView)->show();


	SGLprintState("fertsch");
	a.connect( &a, SIGNAL(lastWindowClosed()), SLOT(quit()) );
	return a.exec();
}


