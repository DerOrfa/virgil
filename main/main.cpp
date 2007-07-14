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

//<vista-zeuch>

//#include <vista/Vlib.h>
#include <vista/VImage.h>
#include <vista/mu.h>
#include <vista/option.h>

#include <stdio.h>
#include <stdlib.h>
//</vista-zeuch>

#include <exception>

#include <odindata/fileio.h>

int main( int argc, char ** argv ) 
{
	FileIO::ProtocolDataMap data;
	unsigned short verbose = 2;
	switch(verbose)
	{
	case 0:SGLshowState=false;
	case 1:SGLshowWarnings=false;
	case 2:SGLshowInfos=false;
	}
	
	SGLprintState("Lese Daten ein ...");
	
	{
		Protocol prot;
	// 	FileIO::set_trace_status(true);
		char lastarg[ODIN_MAXCHAR];
		char parname[ODIN_MAXCHAR];
		STD_string parstring;
		parname[0]='\0';
	
		getLastArgument(argc,argv,lastarg,ODIN_MAXCHAR,false);
		if(getCommandlineOption(argc,argv,"-jdx",parname,ODIN_MAXCHAR,false)) {
			parstring=STD_string("::")+parname;
		}
	
		prot.seqpars.set_MatrixSize(readChannel,1);
		prot.seqpars.set_MatrixSize(phaseChannel,1);
		prot.seqpars.set_MatrixSize(sliceChannel,1);
		prot.set_parmode(noedit);
		prot.parse_cmdline_options(argc,argv);
		
		FileReadOpts ropts;
		ropts.parse_cmdline_options(argc,argv);
		
	// 	mopts.parse_cmdline_options(GuiApplication::argc(),GuiApplication::argv());
		// 	
	// 	fmri.parse_cmdline_options(GuiApplication::argc(),GuiApplication::argv());
	
		JDXfileName fname;
		char optval[ODIN_MAXCHAR];
	
		if(getLastArgument(argc,argv,optval,ODIN_MAXCHAR)) {
			fname=optval;
		} else exit(0);
		if(fname=="")exit(0);
		
		if(FileIO::autoread(data,fname,ropts,prot)<0) exit(-1);
	}
	
	if (data.empty()) 
	{
		SGLprintError(" no input image found");
		exit(1);
	}
	std::set_terminate(__gnu_cxx::__verbose_terminate_handler);

	SGLshowInfos=false;
  QApplication a(argc,argv);
	SGLprintState("Initialisiere Schnittstelle ...");
	GLvlMasterView *w =new GLvlMasterView(data);
	w->show();
/*	if(bench)
	{
		SGLprintState("Starte automatischen Bechmark ...");
		w->doBenchmark(bench);
	}*/
	SGLprintState("fertsch");
    a.connect( &a, SIGNAL(lastWindowClosed()), SLOT(quit()) );
    return a.exec();
}


