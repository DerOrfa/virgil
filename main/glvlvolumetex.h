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
#include <bild.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

// #include "glvlsegment.h"

/**
@author Enrico Reimer,,,
*/
template<class T,class DT> inline bool copyXline(DT *&dst,T &src,int xcnt)
{
	if(xcnt<=0)
		return false;
	const DT MAX=numeric_limits<DT>::max();
	while(xcnt--)
	{
		if(*src)//Wenn der Farb Wert nicht null ist -
		{
			*(dst++)=*(src++); // - ihn setzen
			*(dst++)=MAX; // - und ihn anzeigen
		}
		else
		{
			src++;
			dst+=2;
		}
	}
	return true;
}

//"Mapt" den übergebenen Farbvektor über den übergebenen Wert in den Zeiger
template<class T,class DT> inline bool mapXline(DT *&dst,T &src,int xcnt,EVektor<DT> mapPosVektor,EVektor<DT> mapNegVektor)
{
	if(xcnt<=0)
		return false;
	DT MAX=numeric_limits<DT>::max();
	while(xcnt--)
	{
		EVektor<DT> ergVekt;
		if(*src > 0)
			ergVekt = mapPosVektor* *(src++);
		else
			ergVekt= mapNegVektor* -*(src++);
		for(int i=0;i<ergVekt.size();i++)
			*(dst++)=ergVekt[i];

	}
	return true;
}

template<class T,class DT> inline bool fillXline( DT *&dst, int xcnt)
{
	if(xcnt<=0)
		return false;
	else dst+=xcnt;//calloc setzt schon alle auf NULL - brauch hier nix machen
	return true;
}



class GLvlVolumeTex : public SGLBaseTex
{
public:
	GLvlVolumeTex();
//	bool loadSegment(GLvlSegment &src);
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

	bool Load3DImage(Bild<GLubyte> &img);
	template<class T> void loadImageInfo(Bild<T> &src)
	{
		(*static_cast<dim*>(&Info.X))=src.xsize;
		(*static_cast<dim*>(&Info.Y))=src.ysize;
		(*static_cast<dim*>(&Info.Z))=src.zsize;

		Info.X.getCnt('X');
		Info.Y.getCnt('Y');
		Info.Z.getCnt('Z');

		Info.X.getElsize('X');
		Info.Y.getElsize('Y');
		Info.Z.getElsize('Z');

		Info.size=SGLVektor(
				Info.X.mm_size(1),
			Info.Y.mm_size(1),
			Info.Z.mm_size(1)
		);

/*		struct Bild<T>::ValRange r=src.getValRange();
		const GLfloat scale=1./(r.max-r.min);
		glPixelTransferf(GL_RED_SCALE,scale);
		glPixelTransferf(GL_GREEN_SCALE,scale);
		glPixelTransferf(GL_BLUE_SCALE,scale);
		glPixelTransferf(GL_ALPHA_SCALE,scale);

		glPixelTransferf(GL_RED_BIAS,-r.min*scale);
		glPixelTransferf(GL_GREEN_BIAS,-r.min*scale);
		glPixelTransferf(GL_BLUE_BIAS,-r.min*scale);
		glPixelTransferf(GL_ALPHA_BIAS,-r.min*scale);*/
	}
	SGLVektor texIndex2texKoord(const unsigned int &idx);
	unsigned int texKoord2texIndex(const SGLVektor &koord);
	void calcMatr(SGLVektor offset=SGLVektor(0,0,0));
//     void loadTint(VImage i);
	GLdouble mm2tex_Matrix[4][4];

	unsigned short setupPal(unsigned short start,unsigned short end,bool scale=false);
/*    void loadColorMask(Bild<VBit> &img,EVektor<unsigned short> pos,GLfloat color[3]);
	void loadColorMask(GLvlMinima &img,EVektor<unsigned short> pos,GLfloat color[3]);*/
	static SGLVektor masteroffset;
private:
  bool loadCommon(GLenum gl_type,Bild<GLubyte> &src,EVektor<GLubyte> PosColor,EVektor<GLubyte> NegColor);
  template<class T> bool loadMask(Bild<T> &src);
protected:
	bool npot;
};

#endif
