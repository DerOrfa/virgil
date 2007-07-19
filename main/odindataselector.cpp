//
// C++ Implementation: 
//
// Description: 
//
//
// Author: Enrico Reimer,,, <reimer@cbs.mpg.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "odindataselector.h"
#include <odinqt/jdxwidget.h>
#include <qcombobox.h>

OdinDataSelector::OdinDataSelector(QWidget* parent, FileIO::ProtocolDataMap _data):
		OdinDataSelectorBase(parent),data(_data)
{
	bool selected=false;
	for(FileIO::ProtocolDataMap::iterator i=data.begin();i!=data.end();i++)
	{
		Protocol prot=i->first;
		STD_string id;
		STD_string full_name;
		char  	birth_date[ODIN_DATE_LENGTH];
		char   	sex;
		float  	weight;
		const short iX=i->second.shape()[3];
		const short iY=i->second.shape()[2];
		const short iZ=i->second.shape()[1];
		const float rX=prot.geometry.get_FOV(readChannel);
		const float rY=prot.geometry.get_FOV(phaseChannel);
		const float rZ=prot.geometry.get_FOV(sliceChannel);
		
		const QString rDim=
				QString::number(rX)+"mm x "+
				QString::number(rY)+"mm x "+
				QString::number(rZ)+"mm";
		const QString res=
				QString::number(rX/iX,'f',2)+"x"+
				QString::number(rY/iY,'f',2)+"x"+
				QString::number(rZ/iZ,'f',2);
		prot.study.get_Patient(id,full_name,birth_date,sex,weight);
		
		DataCombo->insertItem(id+" "+full_name+": "+rDim+" Voxelsize:"+res);
		if(!selected && i->second.shape()[1]>1)
		{
			DataCombo->setCurrentItem(DataCombo->count()-1);
			JDXwidget *w=new JDXwidget(prot.geometry,1,this);
			selected=true;
		}
	}
}

OdinDataSelector::~OdinDataSelector()
{
}

void OdinDataSelector::selected(int index)
{
	FileIO::ProtocolDataMap::iterator i=getSelected();
	onSelect(i->first,i->second);
}

FileIO::ProtocolDataMap::iterator OdinDataSelector::getAt(int index)
{
	assert(!data.empty());
	FileIO::ProtocolDataMap::iterator i=data.begin();
	while(index-- && i!=data.end())i++;
	return i;
}
FileIO::ProtocolDataMap::iterator OdinDataSelector::getSelected()
{return getAt(DataCombo->currentItem());}
