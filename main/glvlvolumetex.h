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
#include <vista/VImage.h>

/**
@author Enrico Reimer,,,
*/
class GLvlVolumeTex : public SGLBaseTex
{
public:
	GLvlVolumeTex();
	bool Load3DImage(VImage src);
	struct dimData{
		int cnt;
		float Elsize;
		double mm_size,outer_mm_size;
		int startgap_cnt,endgap_cnt;
		double startgap_mm,endgap_mm;
		inline void calcGap(int start,int end)
		{
			startgap_cnt=start;startgap_mm=startgap_cnt*Elsize;
			endgap_cnt=end;endgap_mm=endgap_cnt*Elsize;
			outer_mm_size=(start+cnt+end)*Elsize;
		}
		inline unsigned int holeSize()
		{
			return startgap_cnt +cnt+endgap_cnt;
		}
		inline unsigned int outerTexKoord2Index(double Koord)
		{
			return  uint(Koord*outer_mm_size*holeSize());
		}
	};
	struct texInfo
	{
		union {dimData X;dimData Columns;};
		union {dimData Y;dimData Rows;};
		union {dimData Z;dimData Bands;dimData layer;};
		SGLVektor top_left_front,bottom_right_back;
		SGLVektor outer_top_left_front,outer_bottom_right_back;
		SGLVektor outer2inner_trans_mm;
		void calcGaps(int startx,int endx,int starty,int endy,int startz,int endz)
		{
			X.calcGap(startx,endx);
			Y.calcGap(starty,endy);
			Z.calcGap(startz,endz);

			outer_top_left_front=SGLVektor(
				(-X.mm_size/2-X.startgap_mm),
				(Y.mm_size/2+Y.startgap_mm),
				(Z.mm_size/2+Z.startgap_mm)
			);

			outer_bottom_right_back=SGLVektor(
				(X.mm_size/2+X.endgap_mm),
				(-Y.mm_size/2-Y.endgap_mm),
				(-Z.mm_size/2-Z.endgap_mm)
			);
			outer2inner_trans_mm=SGLVektor(
				X.endgap_mm-X.startgap_mm,//platz rechts (positiver quadrant)
				Y.endgap_mm-Y.startgap_mm, //platz unten (negativer quadrant)
				Z.endgap_mm-Z.startgap_mm //platz hinten (pos quadrant)
			);
		}
	}dim;

	void loadImageInfo(VImage &src);
	SGLVektor weltKoord2texKoord(SGLVektor welt);
	SGLVektor texKoord2weltKoord(SGLVektor welt);
	unsigned int texKoord2texIndex(SGLVektor koord,unsigned int *x=NULL,unsigned int *y=NULL,unsigned int *z=NULL);
    void calcMatr();
    void loadTint(VImage i);
    GLdouble mm2tex_Matrix[4][4];
//    GLdouble gl2mm_Matrix[4][4];
private:
	template<class T> bool GLvlVolumeTex::fillIndexData(GLenum gl_type,VImage &src);
	template<class T,class ST> bool GLvlVolumeTex::fillFloatData(GLenum gl_type,VImage &src,EVektor<T> PosColor=EVektor<T>(),EVektor<T> NegColor=EVektor<T>());
};

#endif
