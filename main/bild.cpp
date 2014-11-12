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
	const isis::util::fvector3 rowVec=isis::util::PropertyMap::getPropertyAs<isis::util::fvector3>("rowVec")*fov[isis::data::rowDim]+origin;
	const isis::util::fvector3 colVec=isis::util::PropertyMap::getPropertyAs<isis::util::fvector3>("columnVec")*fov[isis::data::columnDim]+origin;
	const isis::util::fvector3 sliceVec=isis::util::PropertyMap::getPropertyAs<isis::util::fvector3>("sliceVec")*fov[isis::data::sliceDim]+origin;
	for(int i=0;i<3;i++){
		frame->rowVec[i]=rowVec[i];
		frame->colVec[i]=colVec[i];
		frame->sliceVec[i]=sliceVec[i];
		frame->origin[i]=origin[i];
	}
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