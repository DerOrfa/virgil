//
// C++ Interface:
//
// Description:
//
//
// Author: Enrico Reimer,,, <reimer@cbs.mpg.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef ODINDATASELECTOR_H
#define ODINDATASELECTOR_H

#include <libsgl/sglsignal.h>
#include "odindataselectorbase.h"
#include <odindata/fileio.h>
#include <qfileinfo.h>

class OdinDataSelector : public OdinDataSelectorBase
{
  Q_OBJECT
public:
	FileIO::ProtocolDataMap data;
	OdinDataSelector(QFileInfo file,FileReadOpts ropts,Protocol prot,QWidget* parent=NULL);
  ~OdinDataSelector();
	FileIO::ProtocolDataMap::iterator getSelected();
	FileIO::ProtocolDataMap::iterator getAt(int);
	bool save(FileIO::ProtocolDataMap::iterator i, QString target);
	unsigned short open(QFileInfo file,FileReadOpts ropts,Protocol prot);
	/*$PUBLIC_FUNCTIONS$*/
	SGLSignal<void(Protocol,Data<float,4>)> onSelect;
public slots:
    virtual void selected(int);
    virtual void save();
};

#endif

