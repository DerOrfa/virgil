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
#include <libsgl/util/sglmaterial.h>

GLvlRule::GLvlRule(SGLVektor _from,SGLVektor _to):from(_from),to(_to),Masslinie(from,to)
{
	Bemassung.MoveTo(getCenter());
	Bemassung.Mat->SetColor(255,0,0);
}

SGLVektor GLvlRule::getCenter()const
{
	return (from+from)*.5*(to-from).Len();
}

/*!
    \fn GLvlPlaneCursor::compileSubObjects()
 */
void GLvlRule::compileSubObjects()
{
	Bemassung.DrahtGitter((camera->Pos-getCenter()).Len()<20);
	Objs.clear();
	TrObjs.clear();
	Objs.push_back(Bemassung.Compile(false));
	Objs.push_back(Masslinie.Compile(false));
}


/*!
    \fn GLvlRule::setCamera(SGLBaseCam *cam)
 */
void GLvlRule::setCamera(SGLBaseCam *cam)
{
	camera=cam;
	Bemassung.FaceAt=&cam->Pos;
	cam->link(Bemassung);
}
