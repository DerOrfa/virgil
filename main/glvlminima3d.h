#ifndef GLVLMINIMA3D_H
#define GLVLMINIMA3D_H

#include <glvlminima.h>

/**
@author Enrico Reimer,,,
*/
class GLvlMinima3D: public GLvlMinimaBase, public SGLFlObj{
	static GLuint caps;
	static SGLVektor scale;
	inline static void QuadBegin(GLuint id)
	{
		assert(glIsList(id));
		glNewList(id,GL_COMPILE);
	}
	inline static void QuadEnd()
	{
		glEndList();
		SGLcheckGLError;
	}
public:
	GLvlMinima3D(unsigned int pos);
	void generate();
	static void setup(
		SGLVektor norm,
		boost::shared_ptr< vincent::Bild_vimage<vincent::lab_value>  > img
	);
    SGLVektor getCenter();
    shared_ptr<Bild_mem<VBit> > genTex();
};

#endif
