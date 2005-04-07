// g++ -lvista -DPIC_SIZE=50


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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


//<vista-zeuch>

//#include <vista/Vlib.h>
#include <vista/VImage.h>
#include <vista/mu.h>
#include <vista/option.h>
#include <vista/VX.h>

#include <stdio.h>
#include <stdlib.h>
//</vista-zeuch>



int main(int argc, char *argv[])
{

//<vista-zeuch>
	FILE *out_file;
	VAttrList out_list;
	VAttrListPosn posn;

	VImage src=NULL, dst1=NULL,dst2=NULL, dst3=NULL,dst4=NULL;
	static VOptionDescRec  options[] = {};

	VParseFilterCmd (VNumber (options), options, argc, argv,NULL,&out_file);
	
	/* Create attribute list for output images */
	out_list = VCreateAttrList();
	VUByte *data;

	VImage dst=VCreateImage(PIC_SIZE,PIC_SIZE,PIC_SIZE,VUByteRepn);
	VSelectBand("Vol2Tex",dst,-1,NULL,(void**)&data);


	for(int z=0;z<PIC_SIZE;z++)
	  for(int y=0;y<PIC_SIZE;y++)
	    for(int x=0;x<PIC_SIZE;x++)
	      data[x+(y*PIC_SIZE)+(z*PIC_SIZE*PIC_SIZE)]=
		((x> PIC_SIZE/2 - 5 && x < PIC_SIZE/2+5)||
		(y> PIC_SIZE/2 - 5 && y < PIC_SIZE/2+5)||
		 (z> PIC_SIZE/2 - 5 && z < PIC_SIZE/2+5)) ? 255:PIC_SIZE;
	
	
	VAppendAttr(out_list,"image",NULL,VImageRepn,dst);
	if (! VWriteFile (out_file, out_list)) exit (1);
	fprintf(stderr,"\ndone.\n");
	return EXIT_SUCCESS;
}
