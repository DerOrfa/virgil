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
		unsigned short cnt;
		float Elsize;
		double outer_mm_size;
		inline double mm_size(const unsigned short div){return (idx2mm(cnt/div));}
		inline double minus_mm_size(const unsigned short div){return (idx2mm(cnt/div-cnt));}
		inline double idx2mm(const unsigned short tex_koord){return Elsize*tex_koord;}
		inline unsigned short mm2idx(const double tex_koord){return tex_koord/Elsize;}
		
		unsigned short startgap_cnt,endgap_cnt;
		double startgap_mm,endgap_mm;
		inline void calcGap(int start,int end)
		{
			startgap_cnt=start;
			startgap_mm=idx2mm(startgap_cnt);
			
			endgap_cnt=end;
			endgap_mm=idx2mm(endgap_cnt);
			outer_mm_size=idx2mm(start+cnt+end);
		}
		inline unsigned short holeSize(){return startgap_cnt +cnt+endgap_cnt;}
		inline unsigned short outerTexKoord2Index(const double Koord)
		{
			return mm2idx(Koord)-startgap_cnt;
		}
		inline double Index2TexKoord(const unsigned short idx)
		{
			return idx2mm(idx);
		}
	};
	struct texInfo
	{
		union {dimData X;dimData Columns;};
		union {dimData Y;dimData Rows;};
		union {dimData Z;dimData Bands;dimData layer;};
		SGLVektor size;
		void calcGaps(unsigned short startx,unsigned short endx,unsigned short starty,unsigned short endy,unsigned short startz,unsigned short endz)
		{
			X.calcGap(startx,endx);
			Y.calcGap(starty,endy);
			Z.calcGap(startz,endz);
		}
	}dim;

	void loadImageInfo(VImage &src);
	SGLVektor texIndex2texKoord(const unsigned int &idx);
	unsigned int texKoord2texIndex(const SGLVektor &koord);
    void calcMatr();
    void loadTint(VImage i);
    GLdouble mm2tex_Matrix[4][4];
    
	unsigned short setupPal(unsigned short start,unsigned short end,bool scale=false);
private:
	template<class T> bool GLvlVolumeTex::fillIndexData(GLenum gl_type,VImage &src);
	template<class T,class ST> bool GLvlVolumeTex::fillFloatData(GLenum gl_type,VImage &src,EVektor<T> PosColor=EVektor<T>(),EVektor<T> NegColor=EVektor<T>());
};

#endif
