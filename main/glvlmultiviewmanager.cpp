#include "glvlmultiviewmanager.h"

GLvlMultiviewManager::GLvlMultiviewManager()
{
}


void GLvlMultiviewManager::addImage(const isis::data::Image &img){
	master_images.append(Bild<GLubyte>(img));
}
