#include "bild.h"

Bild::Bild(const isis::data::Image& src):isis::data::Image(src),frame(new ImgFrame)
{
	const isis::util::FixedVector<size_t,4> voxels=this->getSizeAsVector();
	const isis::util::fvector3 voxelsize=isis::util::PropertyMap::getPropertyAs<isis::util::fvector3>("voxelSize");
	xsize.setCnt(voxels[0]);
	ysize.setCnt(voxels[1]);
	zsize.setCnt(voxels[2]);
	xsize.setElsize(voxelsize[0]);
	ysize.setElsize(voxelsize[1]);
	zsize.setElsize(voxelsize[2]);
	
	const isis::util::fvector3 fov=src.getFoV();
	const isis::util::fvector3 origin=isis::util::PropertyMap::getPropertyAs<isis::util::fvector3>("indexOrigin");
	const isis::util::fvector3 rowVec=isis::util::PropertyMap::getPropertyAs<isis::util::fvector3>("rowVec");
	const isis::util::fvector3 colVec=isis::util::PropertyMap::getPropertyAs<isis::util::fvector3>("columnVec");
	const isis::util::fvector3 sliceVec=isis::util::PropertyMap::getPropertyAs<isis::util::fvector3>("sliceVec");
	for(int i=0;i<3;i++){
		frame->rowVec[i]=rowVec[i]*fov[isis::data::rowDim]+origin[i];
		frame->colVec[i]=colVec[i]*fov[isis::data::columnDim]+origin[i];
		frame->sliceVec[i]=sliceVec[i]*fov[isis::data::sliceDim]+origin[i];
		frame->origin[i]=origin[i];
	}
	
	//fill the orientation matrix
	GLdouble mat[4][4];
	memset(mat,0,sizeof(GLdouble)*4*4);
	mat[3][3]=1;
	
	for(int i=0;i<3;i++){
		mat[i][0]=rowVec[i];
		mat[i][1]=colVec[i];
		mat[i][2]=sliceVec[i];
		mat[3][i]=origin[i];
	}
	GLdouble with,height;
	label=SGLshPtr_new<SGL3DText>(genName().c_str());
	label->getDim(&with,&height);
	label->FrontFace=GL_CW; // TODO workaround bug in simpleGL
	label->ResetTransformMatrix(&mat[0][0]);
	label->Scale(3);
	label->Move(with/2,-height/2,0);
}

const SGLshPtr< SGLBaseTex > Bild::getTex()
{
	if(!tex)
		tex.reset(new GLvlVolumeTex(*this));
	return boost::static_pointer_cast<SGLBaseTex>(tex);
}

std::string Bild::genName() const
{
	std::string ret("S");
	ret+=getPropertyAs<std::string>("sequenceNumber");
	if(hasProperty("sequenceDescription"))
		ret+="_"+getPropertyAs<std::string>("sequenceDescription");
	return ret;
}