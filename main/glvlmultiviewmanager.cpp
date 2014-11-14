#include "glvlmultiviewmanager.h"

GLvlMultiviewManager::GLvlMultiviewManager()
{
}


const Bild &GLvlMultiviewManager::addImage(const isis::data::Image &img){
	master_images.append(Bild(img));
	newData();
	return master_images.back();
}
void GLvlMultiviewManager::removeImage(int index)
{
	master_images.removeAt(index);
	newData();
}
void GLvlMultiviewManager::clear()
{
	master_images.clear();
	newData();
}
