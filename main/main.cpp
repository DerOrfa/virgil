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
#include <DataStorage/io_factory.hpp>
#include <CoreUtils/singletons.hpp>
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

	isis::util::slist files;
	GLvlMasterView master;
	if(argc<=1)
		BOOST_FOREACH(QString file,QFileDialog::getOpenFileNames(&master,"Open image data","","*.nii"))
			files.push_back(file.toStdString());
	else 
		files=isis::util::slist(argv+1,argv+argc);

	std::list< isis::data::Image > images=isis::data::IOFactory::load(files);
	
	if(!images.empty()){
		SGLprintState("Initializing interface ...");
		
		GLvlMultiviewManager &manager=isis::util::Singletons::get<GLvlMultiviewManager,10>();
		BOOST_FOREACH(const isis::data::Image &img,images){
			const Bild &i=manager.addImage(img);//register loaded image in handler
			master.glview->showObj(i.frame); // and show it in the master view
			master.glview->showObj(i.label); // and show it in the master view
		}

		SGLprintState("fertsch");
		a.connect( &a, SIGNAL(lastWindowClosed()), SLOT(quit()) );
		int ret= a.exec();
		manager.clear();
		return ret;
	} else {
		QMessageBox::warning(&master,"No data","No images where loaded, closing ...");
	}
}


