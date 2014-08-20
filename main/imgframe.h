#ifndef IMGFRAME_H
#define IMGFRAME_H

#include <sglobj.h>

class ImgFrame : public SGLObj{
public:
	SGLVektor origin,rowVec,colVec,sliceVec;
	void generate();
	SGLVektor getCenter()const;
	ImgFrame();
};

#endif // IMGFRAME_H
