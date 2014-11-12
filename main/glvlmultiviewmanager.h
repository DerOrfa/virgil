#ifndef GLVLMULTIVIEWMANAGER_H
#define GLVLMULTIVIEWMANAGER_H

#include <libsgl/qt4_glue/sglqtmultispacemgr.h>
#include <libsgl/sglsignal.h>
#include <DataStorage/image.hpp>

#include "glvlplaneview.h"
#include "bild.h"

class GLvlMultiviewManager : public SGLqtMultiSpaceMgr
{
Q_OBJECT
public:
	QList<Bild> master_images;
	GLvlMultiviewManager();
	QLinkedList<GLvlPlaneView *> planeViews;
	const Bild& addImage(const isis::data::Image& img);
Q_SIGNALS:
	void newData();
};

#endif // GLVLMULTIVIEWMANAGER_H
