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

#include "glvlminima3d.h"

/**
@author Enrico Reimer,,,
*/
class GLvlVolumeTex : public SGLBaseTex
{
public:
	GLvlVolumeTex();
	bool loadMinimaMask(GLvlMinima3D &src);
	bool Load3DImage(VImage src);
	template<class T> bool Load3DImage(Bild<T> &img);
	class dimData:public dim{
	public:
		float inner_mm_size;
		
		unsigned short startgap_cnt,endgap_cnt;
		float startgap_mm,endgap_mm;
		inline void calcGap(int start,int end,char dir)
		{
			startgap_cnt=start;
			startgap_mm=idx2mm(startgap_cnt);
			
			endgap_cnt=end;
			endgap_mm=idx2mm(endgap_cnt);
			
			inner_mm_size=idx2mm(getCnt(dir));
		}
		inline float outer_mm_size()const{return startgap_mm + inner_mm_size + endgap_mm;}
		inline unsigned short holeSize(char dir){return startgap_cnt +getCnt(dir)+endgap_cnt;}
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
			X.calcGap(startx,endx,'X');
			Y.calcGap(starty,endy,'Y');
			Z.calcGap(startz,endz,'Z');
		}
		void setCnt(unsigned short x,unsigned short y,unsigned short z){X.setCnt(x);Y.setCnt(y);Z.setCnt(z);}
		void setElsize(float x,float y,float z){X.setElsize(x);Y.setElsize(y);Z.setElsize(z);}
	}Info;

	template<class T> void loadImageInfo(Bild<T> &src);
	SGLVektor texIndex2texKoord(const unsigned int &idx);
	unsigned int texKoord2texIndex(const SGLVektor &koord);
    void calcMatr(SGLVektor offset=SGLVektor(0,0,0));
    void loadTint(VImage i);
    GLdouble mm2tex_Matrix[4][4];
    
	unsigned short setupPal(unsigned short start,unsigned short end,bool scale=false);
    void loadColorMask(Bild<VBit> &img,EVektor<unsigned short> pos,GLfloat color[3]);
	void loadColorMask(GLvlMinima3D &img,EVektor<unsigned short> pos,GLfloat color[3]);
	static SGLVektor masteroffset;
private:
	template<class T> bool GLvlVolumeTex::loadPaletted(Bild<T> &src);
	template<class T,class ST> bool GLvlVolumeTex::loadCommon(GLenum gl_type,Bild<T> &src,EVektor<T> PosColor=EVektor<T>(),EVektor<T> NegColor=EVektor<T>());
	template<class T> bool GLvlVolumeTex::loadMask(Bild<T> &src);
};

#endif
