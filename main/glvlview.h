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
	virtual bool loadCfg();
	virtual bool saveCfg();
	SGLqtSpace * glview;
	void showOthersHere(bool toggle);
	void showObjList();
	SGLshPtr<GLvlVolumeTex> tex;
//	static GLvlSegmentDialog* wshed;
	static SGLshPtr<SGLBaseCam> activeCam;
	void	selectView(const SGLVektor dir[3]);

protected:
	void setupSpace(QWidget *parent=NULL);
	void closeEvent(QCloseEvent *e);
	SGLshPtr<SGLBaseCam> myCam;

private:
	class SGLgotFocusSlot:public  SGLSlot
	{
		const SGLshPtr<SGLBaseCam> &myCam;
	public:
		SGLgotFocusSlot(const SGLshPtr<SGLBaseCam> &_myCam):myCam(_myCam){}
		void operator()(int reason){
			if(GLvlView::activeCam!=myCam)
			{
				std::cout << "aktiveCam:" << GLvlView::activeCam << std::endl << GLvlView::activeCam->Pos << std::endl;
				GLvlView::activeCam=myCam;
			}

		}
	}onGotFocus;

	bool selfChange;
	static SGLVektor default_oben[3],default_unten[3],default_vorn[3],default_hinten[3],default_rechts[3],default_links[3];

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
	void sichtVonHinten();
	void sichtVonVorn();
	void sichtVonOben();
	void sichtVonUnten();
	void sichtVonRechts();
	void sichtVonLinks();

	virtual void onMsg(QString msg,bool canskip);
private Q_SLOTS:
	void selectDataDlg();
};

#endif // GLVLVIEW_H
