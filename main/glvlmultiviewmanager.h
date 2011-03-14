#ifndef GLVLMULTIVIEWMANAGER_H
#define GLVLMULTIVIEWMANAGER_H

#include <libsgl/qt4_glue/sglqtmultispacemgr.h>
#include <libsgl/sglsignal.h>

#include "glvlplaneview.h"

class GLvlMultiviewManager : public SGLqtMultiSpaceMgr
{
public:
    GLvlMultiviewManager();
	SGLshPtr<Bild<GLubyte> > masterImg;
	std::list<GLvlPlaneView *> planeViews;
};

#endif // GLVLMULTIVIEWMANAGER_H
