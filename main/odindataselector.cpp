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
#include <qfiledialog.h>

OdinDataSelector::OdinDataSelector(QFileInfo file,FileReadOpts ropts,Protocol prot,QWidget* parent):OdinDataSelectorBase(parent)
{
	bool selected=false;
	open( file,ropts,prot);
	for(FileIO::ProtocolDataMap::iterator i=data.begin();i!=data.end();i++)
	{
		Protocol prot=i->first;
		STD_string id;
		STD_string full_name;
    STD_string birth_date;
		char   	sex;
		float  	weight;
		const short iX=i->second.shape()[3];
		const short iY=i->second.shape()[2];
		const short iZ=i->second.shape()[1];
    const float rX=prot.geometry.get_FOV(readDirection);
    const float rY=prot.geometry.get_FOV(phaseDirection);
    const float rZ=prot.geometry.get_FOV(sliceDirection);

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
//			JDXwidget *w=new JDXwidget(prot.geometry,1,this);
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

void OdinDataSelector::save()
{
	QFileDialog dlg(this,"Save Dataset to");
	dlg.setMode(QFileDialog::AnyFile);
	svector formats=FileIO::autoformats(); //fills the list of supportet formats @todo bad hack - ask thies
	for(svector::const_iterator n=formats.begin();n!=formats.end();n++)
	{
/*		const FileFormat* ff=FileFormat::get_format("",*n);
		assert(ff);
		QString const desc(ff->description());*/
		dlg.addFilter (  "(*."+ *n + ")"  );
	}
/*	for(STD_map<STD_string,FileFormat*>::iterator i=FileFormat::formats.begin(); i!=FileFormat::formats.end();i++)
	{
		QString const desc(i->second->description());
		const svector &suff=i->second->suffix();
		for(svector::const_iterator n=suff.begin();n!=suff.end();n++)
			dlg.addFilter ( desc+" (*."+ *n + ")" );
	}*/
	if(dlg.exec()==QDialog::Accepted)save(getAt(DataCombo->currentItem()),dlg.selectedFile());
}

bool OdinDataSelector::save(FileIO::ProtocolDataMap::iterator i, QString target)
{
	FileIO::ProtocolDataMap smap;smap.insert(*i);
	FileWriteOpts wopts;
	FileIO::autowrite(smap,target.latin1(),wopts);
}

unsigned short OdinDataSelector::open(QFileInfo file,FileReadOpts ropts,Protocol prot)
{
	if(FileIO::autoread(data,file.absFilePath(),ropts,prot)<0) exit(-1);
	return data.size();
}
