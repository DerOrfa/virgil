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
#include <assert.h>
#include <qstring.h>

namespace vincent
{

const lab_value WSHED_INIT=numeric_limits<lab_value >::max();
const lab_value WSHED_MASK=numeric_limits<lab_value >::max()-1;
const lab_value WSHED_WSHED=numeric_limits<lab_value >::min();

template<> unsigned short Bild<VBild_value>::xsize=numeric_limits<unsigned short>::max();
template<> unsigned short Bild<VBild_value>::ysize=numeric_limits<unsigned short>::max();
template<> unsigned short Bild<VBild_value>::zsize=numeric_limits<unsigned short>::max();

template<> unsigned short Bild<lab_value>::xsize=numeric_limits<unsigned short>::max();
template<> unsigned short Bild<lab_value>::ysize=numeric_limits<unsigned short>::max();
template<> unsigned short Bild<lab_value>::zsize=numeric_limits<unsigned short>::max();

template<> unsigned short Bild<unsigned short>::xsize=numeric_limits<unsigned short >::max();
template<> unsigned short Bild<unsigned short>::ysize=numeric_limits<unsigned short >::max();
template<> unsigned short Bild<unsigned short>::zsize=numeric_limits<unsigned short >::max();

transform::transform(VImage src) : im(src),D(im){}

boost::shared_ptr<  Bild_vimage<lab_value>  > transform::operator()()
{
	Bild_mem<unsigned short> dist(VBild::xsize,VBild::ysize,VBild::zsize,0);
	Bild_vimage<lab_value> lab(VCreateImage(VBild::zsize,VBild::ysize,VBild::xsize,VShortRepn));
	lab.reset(WSHED_INIT);
	
	PunktFifo<VBild_value> fifoA;
	lab_value curlab=numeric_limits<lab_value >::min();
	unsigned short curdistA;

// 	Start Flooding
	unsigned int aktP=0;
	VBild_value h_min=D[0].wert;
	VBild_value h_max=D[im.size()-1].wert;
	
	VBild_value h=h_min;
	do
	{
/*		if(D[aktP].wert!=h)
			printf("leeres h=%d\n",h);*/
		//Alle Punkte mit h markieren
		for(unsigned int p=aktP;p<im.size() && D[p].wert==h;p++)
		{
			lab[D[p]]=WSHED_MASK;
			iPunkt<VBild_value> nachb[6];
			unsigned short nLen=D[p].getNachb(nachb,im);
			for(unsigned short i=0;i<nLen;i++)
//			wenn p einen nachbar q hat, der zu mindestens einem erkannten Basin gehört (zu einem unbekannten kann er nich gehören)
//			(gehört der Nachb zu einem Basin, hat per def ein niedrigeres h, da er schon vorher bearbeitet worden sin muss)
//			(steilster anstieg ist Implizit, da niedrigste Pkt zuerst gelabelt werden)
				if(lab[nachb[i]]!= WSHED_MASK && lab[nachb[i]]!=WSHED_INIT)
				{
					dist[D[p]]=1;//Abstand zum nächsten Punkt, der schon "unter wasser" steht
					fifoA.push(D[p]);
					break;
				}
		}
		//Alle Wasserkanten (Punkte an Grenze zu Basins) sind in fifo
		
		curdistA=1;
		fifoA.push_null();
		//Plateaus volllaufen lassen
		for(iPunkt<VBild_value> p=fifoA.pop();fifoA.size();p=fifoA.pop())
		{
			//wenn keine Wasserscheiden mehr da, lassen wir das Wasser IN DER EBENE weiterlaufen 
			//(der Wasserspiegel wird hier NICHT erhöht)
			if(p.invalid())//Marker
			{
				fifoA.push_null();
				assert(curdistA<numeric_limits<unsigned short>::max());
				curdistA++;
				continue;
			}
			
			iPunkt<VBild_value> nachb[6];
			unsigned short nLen=p.getNachb(nachb,im);
			for(unsigned short i=0;i<nLen;i++)
			{
				if(dist[nachb[i]]<curdistA && (lab[nachb[i]]!= WSHED_MASK && lab[nachb[i]]!=WSHED_INIT)) //q belongs to existing basin or WSHED
				{
					if(lab[nachb[i]]!= WSHED_MASK && lab[nachb[i]]!=WSHED_INIT && lab[nachb[i]]!=WSHED_WSHED)
					{
						if(lab[p] == WSHED_MASK || lab[p] == WSHED_WSHED)lab[p]=lab[nachb[i]];
						else if(lab[p]!=lab[nachb[i]])lab[p]=WSHED_WSHED;
					}
					else if(lab[p]==WSHED_MASK)lab[p]=WSHED_WSHED;
				}
				else 
					if(lab[nachb[i]]==WSHED_MASK && dist[nachb[i]]==0) //q hat selbe höhe wie p, ist aber nicht teil der aktuellen wasserkante => Plateau
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
				assert(curlab<numeric_limits<lab_value>::max());
				lab[D[aktP]]=++curlab;
				fifoA.push(D[aktP]);
				while(fifoA.size())
				{
					iPunkt<VBild_value> q=fifoA.pop();
					iPunkt<VBild_value> nachb[6];
					unsigned short nLen=q.getNachb(nachb,im);
					for(unsigned short i=0;i<nLen;i++)
					{
						
						if(lab[nachb[i]]==WSHED_MASK) //inspect neighbours of q
						{
							fifoA.push(nachb[i]);
							lab[nachb[i]]=curlab;
						}
					}
				}
			}
		}
		reached(h,curlab-numeric_limits<lab_value >::min());
	}while((h++)<h_max);
	return last_erg = boost::shared_ptr< Bild_vimage<lab_value> >(new Bild_vimage<lab_value>(lab));
}

void transform::run()
{
	init();
	this->operator()();
	end();
}

boost::shared_ptr< PunktList<lab_value> > transform::getVoxels(const Bild_vimage<lab_value> &im)
{
	boost::shared_ptr< PunktList<lab_value> > D(new PunktList<lab_value>(im,WSHED_WSHED));
	sort_q<lab_value> comp;
	std::sort(D->m, D->m + D->size,comp);
	return D;
}

}

/*!
    \fn vincent::transform::init()
 */
void vincent::transform::init()
{
	msg("WShed-Graph mit " +QString::number(im.size()/1000000.)+ " Millionen Knoten wird initialisiert, "+ QString::number((im.size()/1048576.)*sizeof(iPunkt<VBild_value>))+ "MB",false);
	sort_q<VBild_value> comp;
	std::sort(D.m, D.m + im.size(),comp);
}
