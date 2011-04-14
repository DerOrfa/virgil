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

int main( int argc, char ** argv )
{
	ENABLE_LOG( Runtime, isis::util::DefaultMsgPrint, isis::warning );
	ENABLE_LOG( Debug, isis::util::DefaultMsgPrint, isis::info );
	std::set_terminate(__gnu_cxx::__verbose_terminate_handler);

	QApplication a(argc,argv);
	unsigned short verbose = 3;
	switch(verbose)
	{
	case 0:SGLshowState=false;
	case 1:SGLshowWarnings=false;
	case 2:SGLshowInfos=false;
	default:SGLshowInfos=SGLshowWarnings=SGLshowState=true;
	}

	SGLprintState("Lese Daten ein ...");
	std::list<isis::data::Image> images=isis::data::IOFactory::load(argv[1]);
	isis::util::Singletons::get<GLvlMultiviewManager,10>().addImage(images.front());

	SGLprintState("Initialisiere Schnittstelle ...");
	(new GLvlMasterView)->show();
	(new GLvlPlaneView)->show();
	(new GLvlPlaneView)->show();

	SGLprintState("fertsch");
	a.connect( &a, SIGNAL(lastWindowClosed()), SLOT(quit()) );
	return a.exec();
}


