#ifndef GLVLVIEW_H
#define GLVLVIEW_H

#include <libsgl/sglsignal.h>
#include <libsgl/qt4_glue/sglqtspace.h>
#include "ui_GLvlView.h"
#include "glvlvolumetex.h"


class GLvlView : public QMainWindow,public Ui_GLvlView
{
	Q_OBJECT
public:
	GLvlView();
	SGLqtSpace * glview;
	void showObjList();
	SGLshPtr<GLvlVolumeTex> tex;
//	static GLvlSegmentDialog* wshed;
	static SGLshPtr<SGLBaseCam> activeCam;
	void	selectView(const SGLVektor dir[3]);

protected:
	void setupSpace(QWidget *parent=NULL);
	void closeEvent(QCloseEvent *e);

private:
	class SGLgotFocusSlot:public  SGLSlot
	{
		SGLshPtr<SGLBaseCam> myCam;
	public:
		void setCam(const SGLshPtr<SGLBaseCam> &_myCam){
			Q_ASSERT(_myCam);
			myCam=_myCam;
		}
		void operator()(int reason){
			Q_ASSERT(myCam);
			if(GLvlView::activeCam!=myCam)
			{
				GLvlView::activeCam=myCam;
			}
		}
	}onGotFocus;

	bool selfChange;
	static SGLVektor sagittal[3],axial[3],coronal[3];

public Q_SLOTS:
	/*$PUBLIC_SLOTS$*/
	void	canRoll(bool toggle);
	void	canGier(bool toggle);
	void	canKipp(bool toggle);
	void	setCoordAim();
	void	setRollDeg(int deg);
	void	setCoordCam();
	void	onCamChanged();
	void	selectViewMode(int view);
	void	showOthersHere(bool toggle);
	void	on_actionSagittal_triggered(bool);
	void	on_actionAxial_triggered(bool);
	void	on_actionCoronal_triggered(bool);
// 	void	on_actionOpen_file_triggered(bool);

	virtual void onMsg(QString msg,bool canskip);
	void on_actionNewPlane_triggered(bool);
};

#endif // GLVLVIEW_H
