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

#include "pixel.h"

int main( int argc, char ** argv ) 
{
//<vista-zeuch>
	FILE *in_file;

	VAttrList list=NULL;
	VLong bench=0;
	VUByte verbose=2;
	VAttrListPosn posn;
	std::list<VImage> src;
	static VOptionDescRec  options[] = {
		{"benchmark",VLongRepn,1,(VPointer) &bench,VOptionalOpt,NULL,"führt beim Start einen Benchmark aus (in sec.)"},
		{"v",VUByteRepn,1,(VPointer) &verbose,VOptionalOpt,NULL,"verbosity"},
	};

	VParseFilterCmd (VNumber (options),options,argc,argv,&in_file,NULL);
	
	switch(verbose)
	{
	case 0:SGLshowState=false;
	case 1:SGLshowWarnings=false;
	case 2:SGLshowInfos=false;
	}
	
	SGLprintState("Lese Daten ein ...");
	if (! (list = VReadFile (in_file, NULL))) exit (1);
	fclose(in_file);
	
	for (VFirstAttr (list, & posn); VAttrExists (& posn); VNextAttr (& posn)) 
	{
		switch(VGetAttrRepn (& posn))
		{
		case VImageRepn:
			VImage tImage;
			VGetAttrValue (& posn, NULL,VImageRepn, & tImage);
			src.push_back(tImage);
			break;
		default:break;
		}
	}
	if (src.empty()) 
	{
		VError(" no input image found");
		exit(1);
	}
//</vista-zeuch>
	
	SGLshowInfos=false;
    QApplication a(argc,argv);
	SGLprintState("Initialisiere Schnittstelle ...");
	GLvlMasterView *w =new GLvlMasterView(src);
	w->show();
	if(bench)
	{
		SGLprintState("Starte automatischen Bechmark ...");
		w->doBenchmark(bench);
	}
	SGLprintState("fertsch");
    a.connect( &a, SIGNAL(lastWindowClosed()), SLOT(quit()) );
    return a.exec();
}


