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
#include "vincent_transform.h"

namespace vincent
{

transform::transform(VImage src) : QObject(),im(src),D(im)
{
	printf("WShed-Graph mit %g Millionen Knoten initialisiert, %g MB belegt\n",
	im.size()/1000000.,(im.size()/1048576.)*sizeof(iPunkt<VUByte>));
	printf("Sortiere\n");
	sort_q<VUByte> comp;
	std::sort(D.m, D.m + im.size(),comp);
	printf("fertsch\n");
}

transform::~transform()
{
}

void transform::test()	{
	iPunkt<VUByte> r=D[5000];
	iPunkt<VUByte> l[6];
	unsigned short len=r.getNachb(l,im);
	for(int i=0;i<len;i++)
		printf("%d-%d-%d:%d\n",l[i].x(),l[i].y(),l[i].z(),im[l[i]]);
}

unsigned short Bild<VUByte>::xsize=numeric_limits<unsigned short>::max();
unsigned short Bild<VUByte>::ysize=numeric_limits<unsigned short>::max();
unsigned short Bild<VUByte>::zsize=numeric_limits<unsigned short>::max();
}
