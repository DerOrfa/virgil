#ifndef GLVLMULTIVIEWMANAGER_H
#define GLVLMULTIVIEWMANAGER_H

#include <libsgl/qt4_glue/sglqtmultispacemgr.h>
#include <libsgl/sglsignal.h>

#include "glvlplaneview.h"
#include <isis/DataStorage/image.hpp>

class GLvlMultiviewManager : public SGLqtMultiSpaceMgr
{
Q_OBJECT
public:
	QList<Bild<GLubyte> > master_images;
	GLvlMultiviewManager();
	QLinkedList<GLvlPlaneView *> planeViews;
	void addImage(const isis::data::Image &img);
Q_SIGNALS:
	void newData();
};

#endif // GLVLMULTIVIEWMANAGER_H
