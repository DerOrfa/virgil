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
#include <bild.h>

/**
@author Enrico Reimer,,,
*/
class GLvlVolumeTex : public SGLBaseTex
{
public:
	GLvlVolumeTex();
	bool Load3DImage(VImage src);
	template<class T> bool Load3DImage(Bild<T> &img);
	class dimData:public dim{
	public:
		double outer_mm_size;
		
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
		inline unsigned short TexKoord2Index(const double Koord)
		{
			return mm2idx(Koord);
		}
		inline double Index2TexKoord(const unsigned short idx)
		{
			return idx2mm(idx);
		}
	};
	struct texInfo{
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
	}Info;

	template<class T> void loadImageInfo(Bild<T> &src);
	SGLVektor texIndex2texKoord(const unsigned int &idx);
	unsigned int texKoord2texIndex(const SGLVektor &koord);
    void calcMatr();
    void loadTint(VImage i);
    GLdouble mm2tex_Matrix[4][4];
    
	unsigned short setupPal(unsigned short start,unsigned short end,bool scale=false);
    void loadBitMask(Bild<VBit> &img);
private:
	template<class T> bool GLvlVolumeTex::loadPaletted(Bild<T> &src);
	template<class T,class ST> bool GLvlVolumeTex::loadCommon(GLenum gl_type,Bild<T> &src,EVektor<T> PosColor=EVektor<T>(),EVektor<T> NegColor=EVektor<T>());
	template<class T> bool GLvlVolumeTex::loadBitmask(Bild<T> &src);
};

#endif
