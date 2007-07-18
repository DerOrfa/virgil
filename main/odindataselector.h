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

class OdinDataSelector : public OdinDataSelectorBase
{
  Q_OBJECT
public:
	FileIO::ProtocolDataMap data;
	OdinDataSelector(QWidget* parent, FileIO::ProtocolDataMap data);
  ~OdinDataSelector();
	FileIO::ProtocolDataMap::iterator getSelected();
	FileIO::ProtocolDataMap::iterator getAt(int);
	/*$PUBLIC_FUNCTIONS$*/
	SGLSignal<void(Protocol,Data<float,4>)> onSelect;
public slots:
    virtual void selected(int);
};

#endif

