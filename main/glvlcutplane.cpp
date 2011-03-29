//
// C++ Implementation: glvlcutplane
//
// Description:
//
//
// Author: Enrico Reimer,,, <enni@Akira>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "glvlcutplane.h"
#include <assert.h>

GLvlCutPlane::GLvlCutPlane():SGL3DPlane(0,0),onDataSelect(this)
{
	useCenter=false;
	IgnoreLight=true;;
	bool old_free=is_free;
	is_free=true;
	resetTexKoord();
	is_free=old_free;
#warning implement me
//	volumeTex->changed.connect(compileNextTime,boost::signals::at_front);

}

void GLvlCutPlane::resetTexKoord()
{
	for(int i=0;i<4;i++)
		setTexKoord(
			i,
			EckVektoren[i]->SGLV_X,
			EckVektoren[i]->SGLV_Y,
			EckVektoren[i]->SGLV_Z
			);
	compileNextTime();
}

GLvlCutPlane::SelectSlot::SelectSlot(GLvlCutPlane* p):plane(p)
{}

void GLvlCutPlane::SelectSlot::operator()()
{
}
