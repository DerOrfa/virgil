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

	void displayMinimaData(const GLvlMinima &m);
	boost::shared_ptr<vincent::transform> v_transform;
public:
	class MinimaItem:public QListViewItem, public boost::shared_ptr<GLvlMinima>
	{
	public:
		MinimaItem(QListViewItem *parent,boost::shared_ptr<GLvlMinima> min);
		void showInf();
	};
	class SegmentItem:public QListViewItem,public GLvlSegment
	{
	public:
		SegmentItem(QListView *parent,boost::shared_ptr<GLvlMinima> min);
		void addMinima(boost::shared_ptr<GLvlMinima> min);
		void showInf();
		
	};
	GLvlSegmentDialog(QWidget* parent,VImage &Img);
	~GLvlSegmentDialog();
	/*$PUBLIC_FUNCTIONS$*/
	void registerSegment(boost::shared_ptr<GLvlSegment> seg);
	void selectMinima(boost::shared_ptr<GLvlSegment> min);
    void redisplayMinima(boost::shared_ptr<GLvlSegment> &min);
	bool isMinimaInSegm(const boost::shared_ptr<GLvlSegment> &min);

	VImage Img;

	map<vincent::lab_value,boost::shared_ptr<GLvlSegment> > objs;
	boost::shared_ptr<GLvlSegment> aktMinima;
	SegmentItem* aktSegment;
	

public slots:
	/*$PUBLIC_SLOTS$*/
	virtual void          onSelectItem(QListViewItem*);
	virtual void          startTransform();
	virtual void          onWatershedReady(bool=false);
	virtual void findMinima(unsigned int index);
	virtual void findMinima(vincent::lab_value id);
	virtual void addCurrMinima();
	virtual void chCapRelCurrMinima(signed char topdelta,signed char bottomdelta);
	virtual void onReached(vincent::VBild_value,unsigned short );
    virtual void setTopCapAbsCurrMinima(int);
    virtual void setBottomCapAbsCurrMinima(int);
	virtual void onMsg(QString,bool);
    void newSegment();
    void toggleWShed(bool);

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

};

#endif

