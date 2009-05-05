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

#include <qapplication.h>
#include <list>

#include <exception>

#include <odindata/fileio.h>

int main( int argc, char ** argv )
{
	FileIO::ProtocolDataMap data;
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

	{
		Protocol prot;
		char lastarg[ODIN_MAXCHAR];
		char parname[ODIN_MAXCHAR];
		STD_string parstring;
		parname[0]='\0';

		getLastArgument(argc,argv,lastarg,ODIN_MAXCHAR,false);
		if(getCommandlineOption(argc,argv,"-jdx",parname,ODIN_MAXCHAR,false)) {
			parstring=STD_string("::")+parname;
		}

    prot.seqpars.set_MatrixSize(readDirection,1);
    prot.seqpars.set_MatrixSize(phaseDirection,1);
    prot.seqpars.set_MatrixSize(sliceDirection,1);
		prot.set_parmode(noedit);
		prot.parse_cmdline_options(argc,argv);

		FileReadOpts ropts;
		ropts.parse_cmdline_options(argc,argv);

		JDXfileName fname;
		char optval[ODIN_MAXCHAR];

		if(getLastArgument(argc,argv,optval,ODIN_MAXCHAR)) {
			fname=optval;
		} else exit(0);
		if(fname=="")exit(0);
		GLvlMasterView::dataDialog=new OdinDataSelector(QString(fname),ropts,prot);
	}

	std::set_terminate(__gnu_cxx::__verbose_terminate_handler);

	SGLprintState("Initialisiere Schnittstelle ...");
	GLvlMasterView *w =new GLvlMasterView;
	w->show();

  SGLprintState("fertsch");
    a.connect( &a, SIGNAL(lastWindowClosed()), SLOT(quit()) );
  return a.exec();
}


