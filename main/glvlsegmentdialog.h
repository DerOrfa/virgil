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

#ifndef GLVLSEGMENTDIALOG_H
#define GLVLSEGMENTDIALOG_H

#include <boost/shared_ptr.hpp>
#include "glvlsegment.h"
#include "glvlminima.h"
#include <vista/VImage.h>
#include <map>

#include <qlistview.h> 
#include "segments.h"

class GLvlSegmentDialog : public SegmentDialog
{
  Q_OBJECT

	boost::shared_ptr<vincent::transform> v_transform;
	void setMinCapData(VUByte minValue,VUByte Value,VUByte maxValue);
public:
	class MinimaItem:public QListViewItem, public boost::shared_ptr<GLvlMinima>
	{
	public:
		MinimaItem(QListViewItem *parent,boost::shared_ptr<GLvlMinima> min);
	};
	class SegmentItem:public QListViewItem,public boost::shared_ptr<GLvlSegment>
	{
	public:
		SegmentItem(QListView *parent,boost::shared_ptr<GLvlSegment> seg);
	};
	GLvlSegmentDialog(VImage &Img);
	~GLvlSegmentDialog();
	/*$PUBLIC_FUNCTIONS$*/
	void registerSegment(boost::shared_ptr<GLvlSegment> seg);
	void selectMinima(boost::shared_ptr<GLvlSegment> min);

	VImage Img;

	map<vincent::lab_value,boost::shared_ptr<GLvlSegment> > objs;
	boost::shared_ptr<GLvlSegment> aktMinima;
	boost::shared_ptr<GLvlSegment> aktSegment;
	

public slots:
	/*$PUBLIC_SLOTS$*/
	virtual void          onSelectItem(QListViewItem*);
	virtual void          startTransform();
	virtual void          bottomCapChanged(int);
	virtual void          topCapChanged(int);
	virtual void          onWatershedReady(bool=false);
	virtual void findMinima(unsigned int index);
	virtual void addCurrMinima();
	virtual void resizeCurrMinima(short topdelta,short bottomdelta);
	virtual void onReached(vincent::VBild_value,unsigned short );

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

};

#endif

