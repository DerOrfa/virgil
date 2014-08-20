//
// C++ Interface: glvlvolumetex
//
// Description:
//
//
// Author: Enrico Reimer,,, <enni@Akira>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GLVLVOLUMETEX_H
#define GLVLVOLUMETEX_H

#include <util/sgltextur.h>
#include <DataStorage/image.hpp>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

// #include "glvlsegment.h"

/**
@author Enrico Reimer,,,
*/

class GLvlVolumeTex : public SGLBaseTex
{
public:
	class dimData{
		const isis::data::Image &m_img;
		const isis::data::dimensions m_dim;
		float v_size;
		float v_gap;
	public:
		dimData(const isis::data::Image &img,isis::data::dimensions dim):m_img(img),m_dim(dim){
			v_size = m_img.getPropertyAs<isis::util::fvector3>("voxelSize")[dim];
			v_gap = m_img.hasProperty("voxelGap") ? m_img.getPropertyAs<isis::util::fvector3>("voxelGap")[dim]:0;
		}

		inline unsigned short TexCoord2Index(const double Coord)
		{
			const double ret=(Coord+v_gap/2)/(v_size+v_gap); // if the coord is inside the gap of a voxel its index should be used
			return ret<0 ? std::numeric_limits<unsigned short>::max():(unsigned short)ret;
		}
		inline double Index2TexKoord(const unsigned short idx)
		{
			return (v_size+v_gap)*idx+v_size/2;
		}
	}rowDim,colDim,sliceDim;

	GLvlVolumeTex(const isis::data::Image &img);

	unsigned int texKoord2texIndex(const SGLVektor &koord);

private:
	bool load(const isis::data::Image &img);
};

#endif
