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
#include "vincent_punkt.h"
#include "vincent_bild.h"

#define WSHED_INIT	numeric_limits<unsigned short>::max()
#define WSHED_MASK	numeric_limits<unsigned short>::max()-1
#define WSHED_WSHED	0

namespace vincent
{

typedef Bild_vimage<VUByte> VBild;
typedef Bild_mem<unsigned short> MBild;

unsigned short Bild<VUByte>::xsize=numeric_limits<VUByte>::max();
unsigned short Bild<VUByte>::ysize=numeric_limits<VUByte>::max();
unsigned short Bild<VUByte>::zsize=numeric_limits<VUByte>::max();

unsigned short Bild<unsigned short>::xsize=numeric_limits<unsigned short>::max();
unsigned short Bild<unsigned short>::ysize=numeric_limits<unsigned short>::max();
unsigned short Bild<unsigned short>::zsize=numeric_limits<unsigned short>::max();


transform::transform(VImage src) : QObject(),im(src),D(im)
{
	printf("WShed-Graph mit %g Millionen Knoten initialisiert, %g MB belegt\n",
	im.size()/1000000.,(im.size()/1048576.)*sizeof(iPunkt<VUByte>));
	printf("Sortiere\n");
	sort_q<VUByte> comp;
	std::sort(D.m, D.m + im.size(),comp);
	printf("fertsch\n");
}

void transform::test()	{
	iPunkt<VUByte> r=D[5000];
	iPunkt<VUByte> l[6];
	unsigned short len=r.getNachb(l,im);
	for(int i=0;i<len;i++)
		printf("%d-%d-%d:%d\n",l[i].x(),l[i].y(),l[i].z(),im[l[i]]);
}

Bild_mem<unsigned short> transform::operator()()
{
	MBild dist(VBild::xsize,VBild::ysize,VBild::zsize,0);
	MBild lab(VBild::xsize,VBild::ysize,VBild::zsize,WSHED_INIT);
	
	printf("Rechne\n");
	PunktFifo<VUByte> fifoA;
	unsigned short curlab=0;
	unsigned short curdistA;

// 	Start Flooding
	unsigned int aktP=0;
	VUByte h_min=D[0].wert;
	VUByte h_max=D[im.size()-1].wert;
	
	printf("%d-%d\n",h_min,h_max);

	for(VUByte h=h_min;h<=h_max;h++)
	{
		if(D[aktP].wert!=h)
			printf("leeres h=%d\n",h);
		//Alle Punkte mit h markieren
		for(unsigned int p=aktP;p<im.size() && D[p].wert==h;p++)
		{
			lab[D[p]]=WSHED_MASK;
			iPunkt<VUByte> nachb[6];
			unsigned short nLen=D[p].getNachb(nachb,im);
			for(unsigned short i=0;i<nLen;i++)
//			wenn p einen nachbar q hat, der zu mindestens einem erkannten Basin gehört (zu einem unbekannten kann er nich gehören)
//			(gehört der Nachb zu einem Basin, hat per def ein niedrigeres h, da er schon vorher bearbeitet worden sin muss)
//			(steilster anstieg ist Implizit, da niedrigste Pkt zuerst gelabelt werden)
				if(lab[nachb[i]]>0 || lab[nachb[i]]==WSHED_WSHED)
				{
					dist[nachb[i]]=1;//Abstand zum nächsten Punkt, der schon "unter wasser" steht
					fifoA.push(nachb[i]);
					break;
				}
		}

		//Alle Wasserkanten (Punkte an Grenze zu Basins) sind in fifo
		curdistA=1;
		fifoA.push_null();
		//Basins volllaufen lassen
		for(iPunkt<VUByte> p=fifoA.pop();fifoA.size();p=fifoA.pop())
		{
			//wenn keine Wasserscheiden mehr da, lassen wir das Wasser IN DER EBENE weiterlaufen 
			//(der Wasserspiegel wird hier NICHT erhöht)
			if(p.invalid())//Marker
			{
				fifoA.push_null();
				curdistA++;
				continue;
			}
			
			iPunkt<VUByte> nachb[6];
			unsigned short nLen=p.getNachb(nachb,im);
			for(unsigned short i=0;i<nLen;i++)
			{
				if(dist[nachb[i]]<curdistA && (lab[nachb[i]]>0 || lab[nachb[i]] == WSHED_WSHED)) //q belongs to existing basin or WSHED
				{
					if(lab[nachb[i]]>0)
					{
						if(lab[p] == WSHED_MASK || lab[p] == WSHED_WSHED)lab[p]=lab[nachb[i]];
						else if(lab[p]!=lab[nachb[i]])lab[p]=WSHED_WSHED;
					}
					else if(lab[p]==WSHED_MASK)lab[p]=WSHED_WSHED;
				}
				else if(lab[nachb[i]]==WSHED_MASK && dist[nachb[i]]==0) //q hat selbe höhe wie p, ist aber nicht teil der aktuellen wasserkante => Plateau
				{
					dist[nachb[i]]=curdistA+1;//Punkte innerhalb eines Plateaus sind weiter weg vom Wasser
					fifoA.push(nachb[i]);
				}
			}
		}
		//neue lokale Minima erkennen
		for(;aktP<im.size() && im[D[aktP]]==h;aktP++)
		{
			dist[D[aktP]]=0;
			if(lab[D[aktP]]==WSHED_MASK)//Alle Punkte, die jetzt noch keinem min zugeordnet werden konnten, sind lokales Min
			{
				lab[D[aktP]]=++curlab;
				fifoA.push(D[aktP]);
				while(fifoA.size())
				{
					iPunkt<VUByte> q=fifoA.pop();
					iPunkt<VUByte> nachb[6];
					unsigned short nLen=q.getNachb(nachb,im);
					for(unsigned short i=0;i<nLen;i++)
						if(lab[nachb[i]]==WSHED_MASK) //inspect neighbours of q
						{
							fifoA.push(nachb[i]);
							lab[nachb[i]]=curlab;
						}
				}
			}
		}
	}
	return lab;
}
}


