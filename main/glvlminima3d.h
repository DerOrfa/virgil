#ifndef GLVLMINIMA3D_H
#define GLVLMINIMA3D_H

#include <glvlminima.h>
#include <list>

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
		const vincent::transform &transform,
		VImage &src
	);
    SGLVektor getCenter();
    shared_ptr<Bild_mem<VBit> > genTex();
	void writeTex(const unsigned short offset[3],Bild<GLubyte> &textur)const;
};

class GLvlMinima3DList : public list<shared_ptr<GLvlMinima3D> >
{
public:
	union {
		struct {unsigned short x,y,z;};
		unsigned short koord[3];
	}minEdge,maxEdge;
	GLvlMinima3DList(shared_ptr<GLvlMinima3D> img);
	GLvlMinima3DList();
	void getDim(dim &X,dim &Y, dim &Z);
	void getOffset(unsigned short offset[3],GLvlMinima3DList::iterator i);
};

#endif
