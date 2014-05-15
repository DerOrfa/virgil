/***************************************************************************
 *   Copyright (C) 2004 by Enrico Reimer, 1.01.2005,hayd,,@[e/ea],-131.-   *
 *   reimer@santiago                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef BILD_H
#define BILD_H

#include <assert.h>
#include <limits.h>
#include <libsgl/sglmisc.h>
#include <math.h>
#include <typeinfo>
#include <libsgl/sglshptr.h>
#include <limits>
#include <isis/DataStorage/image.hpp>
#include "imgframe.h"
#include "glvlvolumetex.h"

class dim
{
	unsigned short cnt;
	float Elsize;
public:
	inline operator unsigned short()const{return cnt;}
	inline double mm_size(const unsigned short div){return (idx2mm(cnt/div));}
	inline double minus_mm_size(const unsigned short div){return (idx2mm(cnt/div-cnt));}
	inline double idx2mm(const unsigned short tex_koord){return Elsize*tex_koord;}
	inline unsigned short mm2idx(const double tex_koord){
		const double ret=rint(tex_koord/Elsize);
		return ret<0 ? std::numeric_limits<unsigned short>::max():(unsigned short)ret;
	}
	inline unsigned short getCnt(char dir){
		if(cnt==0)
		{SGLprintError("Der Datensatz hat keine Dimension in %c-Richtung. Darstellung ist nicht möglich.",dir);abort();}
		else return cnt;
	}
	inline unsigned short setCnt(unsigned short c){return cnt=c;}
	inline float& ElsizeRef(){return Elsize;};
	inline float getElsize(char dir){
		if(Elsize==0)
		{SGLprintWarning("Die Voxel des Datensatzes haben keine Dimension in %c-Richtung, nehme 1mm an",dir);Elsize=1;}
		return Elsize;
	}
	inline float setElsize(float s){return Elsize=s;}
};

class Bild : public isis::data::Image
{
public:
	union {dim xsize;dim Columns;};
	union {dim ysize;dim Rows;};
	union {dim zsize;dim Bands;dim layer;};
	inline unsigned int size()const{return xsize*ysize*zsize;}
	SGLshPtr< GLvlVolumeTex > tex;
public:
	SGLshPtr< ImgFrame > frame;
	const SGLshPtr<SGLBaseTex> getTex(){
		if(!tex)
			tex.reset(new GLvlVolumeTex(*this));
		return boost::static_pointer_cast<SGLBaseTex>(tex);
	}

	Bild(const isis::data::Image &src):isis::data::Image(src),frame(new ImgFrame)
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
	virtual ~Bild(){}
	std::string genName()const{
		std::string ret("S");
		ret+=getPropertyAs<std::string>("sequenceNumber");
		if(hasProperty("sequenceDescription"))
			ret+="_"+getPropertyAs<std::string>("sequenceDescription");
		return ret;
	}
};


#endif
