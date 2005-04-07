//
// C++ Interface: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef GLVLPINSDLG_H
#define GLVLPINSDLG_H

#include <libsgl/text/sgl3dtext.h>
#include "glvlpin.h"
#include "pins.h"
#include <qlistview.h>

class SGLqtSpace;

class GLvlPinsDlg : public pins
{
	Q_OBJECT

public:
	GLvlPinsDlg(QWidget* parent = 0);
	class pinItem:public QListViewItem,public boost::shared_ptr<GLvlPin>
	{
	public:
		SGLqtSpace	&mySpace;
		pinItem(SGLqtSpace * space,const SGLVektor &pos,const QString text);
	};
	GLvlPinsDlg::pinItem* newPin(SGLqtSpace * space,const SGLVektor &pos,const QString text);

public slots:
	virtual void          deletePin();
	virtual void          onSelChange();

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

protected:
    list<GLvlPinsDlg::pinItem*> selPinList;
};


#endif
