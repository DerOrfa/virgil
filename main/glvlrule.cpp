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
#include "glvlrule.h"
#include "glvlplaneview.h"
#include <libsgl/util/sglmaterial.h>

GLvlRuler::GLvlRuler(SGLVektor _from,SGLVektor _to):
from(_from),to(_to),
Masslinie(from,to),
Bemassung(QString::number((to-from).Len()).latin1())
{
	Bemassung.tiefe=10;
	Bemassung.MoveTo(getCenter());
	Bemassung.Mat->SetColor(255,0,0);
	Bemassung.FaceAt=&GLvlView::activeCam->Pos;
	GLvlView::activeCam->link(Bemassung);
}

SGLVektor GLvlRuler::getCenter()const
{
	return from+(to-from)*.5;
}

/*!
    \fn GLvlPlaneCursor::compileSubObjects()
 */
void GLvlRuler::compileSubObjects()
{
	Bemassung.DrahtGitter((GLvlView::activeCam->Pos-getCenter()).Len()<20);
	Objs.clear();
	TrObjs.clear();
	Objs.push_back(Bemassung.Compile(false));
	Objs.push_back(Masslinie.Compile(false));
}

