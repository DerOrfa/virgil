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

template <class T> class Bild : public isis::data::TypedImage<T>
{
public:
	union {dim xsize;dim Columns;};
	union {dim ysize;dim Rows;};
	union {dim zsize;dim Bands;dim layer;};
	inline unsigned int size()const{return xsize*ysize*zsize;}
private:
	std::pair<T,T> range;
public:
	SGLshPtr< ImgFrame > frame;
/*	ValRange getValRange()
	{
		std::less<T> l;
		if(l(range.max, range.min))
		{
			SGLprintState("Scanning %s data",typeid(T).name());
			for(unsigned int i=0;i<this->size();i++)
			{
				const T& dummy=at(i);
				if(dummy>range.max)range.max=dummy;
				if(dummy<range.min)range.min=dummy;
			}
			SGLprintState("max: %f, min %f",range.max,range.min);
		}
		return range;
	}*/
	Bild(const isis::data::Image &src):isis::data::TypedImage<T>(src),range(isis::data::Image::getMinMaxAs<T>()),frame(new ImgFrame)
			//@todo we actually only have to copy/convert the first volume
	{
		const isis::util::FixedVector<size_t,4> voxels=this->getSizeAsVector();
		const isis::util::fvector4 voxelsize=isis::util::PropertyMap::getPropertyAs<isis::util::fvector4>("voxelSize");
		xsize.setCnt(voxels[0]);
		ysize.setCnt(voxels[1]);
		zsize.setCnt(voxels[2]);
		xsize.setElsize(voxelsize[0]);
		ysize.setElsize(voxelsize[1]);
		zsize.setElsize(voxelsize[2]);

		const isis::util::fvector4 fov=src.getFoV();
		const isis::util::fvector4 origin=isis::util::PropertyMap::getPropertyAs<isis::util::fvector4>("indexOrigin");
		const isis::util::fvector4 rowVec=isis::util::PropertyMap::getPropertyAs<isis::util::fvector4>("rowVec")*fov[isis::data::rowDim]+origin;
		const isis::util::fvector4 colVec=isis::util::PropertyMap::getPropertyAs<isis::util::fvector4>("columnVec")*fov[isis::data::columnDim]+origin;
		const isis::util::fvector4 sliceVec=isis::util::PropertyMap::getPropertyAs<isis::util::fvector4>("sliceVec")*fov[isis::data::sliceDim]+origin;
		for(int i=0;i<3;i++){
			frame->rowVec[i]=rowVec[i];
			frame->colVec[i]=colVec[i];
			frame->sliceVec[i]=sliceVec[i];
			frame->origin[i]=origin[i];
		}
	}
	virtual ~Bild(){}
	inline T& at(unsigned short x,unsigned short y,const unsigned short z){
		return isis::data::Image::voxel<T>(x,y,z);
	}
	inline T *copy_line(const unsigned short y,const unsigned short z,void *dst){
//		return ((T*)memcpy(dst,&at(0,y,z),xsize*sizeof(T)))+xsize;
	}
};


#endif
