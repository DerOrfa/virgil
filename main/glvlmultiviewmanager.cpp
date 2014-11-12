#include "glvlmultiviewmanager.h"

GLvlMultiviewManager::GLvlMultiviewManager()
{
}


const Bild &GLvlMultiviewManager::addImage(const isis::data::Image &img){
	master_images.append(Bild(img));
	newData();
	return master_images.back();
}
