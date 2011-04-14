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

#include <libsgl/util/sgltextur.h>
#include <isis/DataStorage/image.hpp>

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
		float inner_mm_size;
	public:
		dimData(const isis::data::Image &img,isis::data::dimensions dim):m_img(img),m_dim(dim){
			v_size = m_img.getPropertyAs<isis::util::fvector4>("voxelSize")[dim];
			v_gap = m_img.hasProperty("voxelGap") ? m_img.getPropertyAs<isis::util::fvector4>("voxelGap")[dim]:0;

			inner_mm_size = img.getFoV()[dim];
			calcGap(1,1);
		}


		unsigned short startgap_cnt,endgap_cnt;
		float startgap_mm,endgap_mm;
		inline void calcGap(int start,int end)
		{
			startgap_cnt=start;
			endgap_cnt=end;

			//the borders size is bordervoxels (whose size is voxelsize plus voxel gap) plus the outer gap of the first inner voxel
			startgap_mm=(v_size+v_gap)*startgap_cnt+v_gap/2;
			endgap_mm=(v_size+v_gap)*endgap_cnt+v_gap/2;
		}
		inline float outer_mm_size()const{return startgap_mm + inner_mm_size + endgap_mm;}
		inline unsigned short wholeSize(){return startgap_cnt +m_img.getSizeAsVector()[m_dim]+endgap_cnt;}
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
