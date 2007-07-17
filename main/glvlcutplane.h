//
// C++ Interface: glvlcutplane
//
// Description: 
//
//
// Author: Enrico Reimer,,, <enni@Akira>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GLVLCUTPLANE_H
#define GLVLCUTPLANE_H

#include <libsgl/sglsignal.h>
#include <libsgl/primitives/sglvieleck.h>
#include "glvlvolumetex.h"

/**
@author Enrico Reimer,,,
*/
class GLvlCutPlane : public SGL3DPlane
{
	class SelectSlot:public SGLSlot
	{
		GLvlCutPlane *plane;
	public:
		void operator()(Protocol,Data<float,4>);
		SelectSlot(GLvlCutPlane* p);
	}onDataSelect;
public:
	GLvlCutPlane(SGLshPtr<GLvlVolumeTex> volumeTex);
	void resetTexKoord();
};

#endif
