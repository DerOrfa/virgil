#ifndef GLVLMULTIVIEWMANAGER_H
#define GLVLMULTIVIEWMANAGER_H

#include <libsgl/qt4_glue/sglqtmultispacemgr.h>
#include <libsgl/sglsignal.h>

#include "glvlplaneview.h"
#include <isis/DataStorage/image.hpp>

class GLvlMultiviewManager : public SGLqtMultiSpaceMgr
{
public:
    GLvlMultiviewManager();
	SGLshPtr<Bild<GLubyte> > masterImg;
	QLinkedList<GLvlPlaneView *> planeViews;
	void addImage(const isis::data::Image &img);
};

#endif // GLVLMULTIVIEWMANAGER_H
