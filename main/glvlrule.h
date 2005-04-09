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
#ifndef GLVLRULE_H
#define GLVLRULE_H

#include <libsgl/sglmetaobj.h>
#include <libsgl/helper/sglcamera.h>
#include <libsgl/text/sgl3dtext.h>

/**
@author Enrico Reimer, 1.01.2005,hayd,,@[e/ea],-131.-221. 143
 */
class GLvlRule : public SGLMetaObj
{
	public:
		GLvlRule(SGLVektor from,SGLVektor to);
		void compileSubObjects();
		SGLVektor getCenter()const;
		void setCamera(SGLBaseCam *cam);
		SGLVektor from,to;
	private:
		SGL3DText Bemassung;
		SGLStrecke Masslinie;
		SGLBaseCam *camera;
};

#endif
