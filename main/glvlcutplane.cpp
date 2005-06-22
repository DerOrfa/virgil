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

GLvlCutPlane::GLvlCutPlane(SGLshPtr<GLvlVolumeTex> volumeTex): SGL3DPlane(0,0, volumeTex)
{
	useCenter=false;
	IgnoreLight=true;;
	bool old_free=is_free;
	is_free=true;
	resetTexKoord();
	is_free=old_free;
	volumeTex->changed.connect(compileNextTime,boost::signals::at_front);
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

