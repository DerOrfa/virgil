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

#include <libsgl/primitives/sglvieleck.h>
#include "glvlvolumetex.h"

/**
@author Enrico Reimer,,,
*/
class GLvlCutPlane : public SGL3DPlane
{
public:
	GLvlCutPlane(boost::shared_ptr<GLvlVolumeTex> volumeTex);
	void resetTexKoord();
};

#endif
