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
#include <vista/VImage.h>
#include <bild.h>
#include <GL/glu.h>

// #include "glvlsegment.h"
#include <bild_odin.h>

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
	template<class T> bool Load3DImage(Protocol prot, Data<T,4> dat)
	{
		valid=Load3DImage(Bild_odin<T>(prot,dat));
	}
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

	template<class T> bool Load3DImage(Bild<T> &img)
	{
		if(ID!=0)freeTexture();
		sglChkExt("GL_EXT_texture3D","Höchstwarscheinlich lassen sich keine nennenswerten Datenmengen laden.",2);
		if(sglChkExt("GL_ARB_texture_non_power_of_two","Es können keine NPOT-Texturen erzeugt werden, schade eigentlich :-(.",0))
			SGLprintInfo("Super, GL_ARB_texture_non_power_of_two ist unterstüzt, jetzt müsste ich es nur noch implementieren ...\n");
		//@todo GL_ARB_texture_non_power_of_two implementieren
	
		loadImageInfo(img);
	
		glGenTextures(1, &ID);
		glBindTexture(TexType, ID);
	
	
//		if(this->renderMode==SGL_MTEX_MODE_COLORMASK)valid=loadMask(img);//@todo wenn loadMask fehlschlägt (warum auch immer) muss das behandelt werden
	
		if(!valid)//Fallback wenn Palette nich tut
	//@todo tut garantiert nicht - GL2 kennt paletted textures nicht mehr :-((
		{
			EVektor<GLfloat> PosColor,NegColor;
			switch(this->renderMode)
			{
				case SGL_MTEX_MODE_TINT:
					PosColor.resize(3);
					NegColor.resize(3);
					PosColor[2]=PosColor[1]=.1;
					NegColor[2]=NegColor[0]=.2;
					break;
				case SGL_MTEX_MODE_OVERLAY:
					PosColor.resize(4);
					NegColor.resize(4);
					PosColor[0]=.25;
					NegColor[1]=.25;
					PosColor[3]=NegColor[3]=.25;
					break;
			}

			if(typeid(T)==typeid(GLubyte))
				valid=loadCommon<T,GLubyte>(GL_UNSIGNED_BYTE,img,PosColor,NegColor);
			else if(typeid(T)==typeid(GLbyte))
				valid=loadCommon<T,GLbyte>(GL_BYTE,img,PosColor,NegColor);
			else if(typeid(T)==typeid(GLushort))
				valid=loadCommon<T,GLushort>(GL_UNSIGNED_SHORT,img,PosColor,NegColor);
			else if(typeid(T)==typeid(GLshort))
				valid=loadCommon<T,GLshort>(GL_SHORT,img,PosColor,NegColor);
			else if(typeid(T)==typeid(GLuint))
				valid=loadCommon<T,GLuint>(GL_UNSIGNED_INT,img,PosColor,NegColor);
			else if(typeid(T)==typeid(GLint))
				valid=loadCommon<T,GLint>(GL_INT,img,PosColor,NegColor);
			else if(typeid(T)==typeid(GLfloat))
				valid=loadCommon<T,GLfloat>(GL_FLOAT,img,PosColor,NegColor);
			else 
			{
				SGLprintWarning("Rechne %s auf %s um",typeid(T).name(),typeid(GLfloat).name());
				valid=loadCommon<T,GLfloat>(GL_FLOAT,img,PosColor,NegColor);
			}
		}
	
		return valid;
	}

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
		
		struct Bild<T>::ValRange r=src.getValRange();
		const GLfloat scale=1./(r.max-r.min);
		glPixelTransferf(GL_RED_SCALE,scale);
		glPixelTransferf(GL_GREEN_SCALE,scale);
		glPixelTransferf(GL_BLUE_SCALE,scale);
		glPixelTransferf(GL_ALPHA_SCALE,scale);
		
		glPixelTransferf(GL_RED_BIAS,-r.min);
		glPixelTransferf(GL_GREEN_BIAS,-r.min);
		glPixelTransferf(GL_BLUE_BIAS,-r.min);
		glPixelTransferf(GL_ALPHA_BIAS,-r.min);
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
	template<class T,class DT> bool loadCommon(GLenum gl_type,Bild<T> &src,EVektor<T> PosColor,EVektor<T> NegColor)
	{
#define xsize	size[0]
#define ysize	size[1]
#define zsize	size[2]
	
		unsigned short voxelElemSize;
		GLint intFormat;
		switch(PosColor.size())
		{
			case 1:	voxelElemSize=1;
			intFormat=GL_INTENSITY;
			break;
			case 2:SGLprintError("Ungültiger Farbvektor");break;
			case 3:	voxelElemSize=3;
			intFormat=GL_RGB;
			break;
			case 4:	intFormat=GL_RGBA;
			voxelElemSize=4;
			break;
			default:intFormat=GL_LUMINANCE_ALPHA;
			voxelElemSize=2;
			break;
		}
	//Größe der Textur ("+2" ist für den Rand)
		GLint size[3];
		xsize=Info.X.getCnt('X')+2;
		ysize=Info.Y.getCnt('Y')+2;
		zsize=Info.Z.getCnt('Z')+2;
		if(!genValidSize(intFormat,size,3, intFormat == GL_INTENSITY ? GL_LUMINANCE:intFormat,gl_type,false))return false;
	//@todo nützt nichts - er glaubt er bekäme die Tex rein bekommt aber unten trotzem "out of memory"
	
		T *pixels=(T*)calloc(xsize*ysize*zsize*voxelElemSize,sizeof(T));
		T *pixels_=pixels;
		pixels+=xsize*voxelElemSize*ysize;//die erste Ebene
	
		int z=0;
	
		for(;z<Info.Z.getCnt('Z') ;z++)
		{
			T *pix= &src.at(0,0,z);
		
			pixels+=xsize*voxelElemSize;//erste Zeile leer lassen
			for(int y=0;y<Info.Y.getCnt('Y');y++)
			{
				pixels+=voxelElemSize;//ersten Voxel (und seinen alpha) leer lassen
			
				if(intFormat== GL_LUMINANCE_ALPHA)
					copyXline(pixels,pix,Info.X.getCnt('X'));
				else 
					mapXline(pixels,pix,Info.X.getCnt('X'),PosColor,NegColor);
				if(xsize-Info.X.getCnt('X')-1 <= 0){SGLprintError("Das Bild ist zu groß");}
				pixels+=(xsize-Info.X.getCnt('X')-1)*voxelElemSize;//Wenn das Bild zu groß is, geht der Zeiger wieder zurück (addition neg. werte) und überschreibt nächtes mal, das was falsch war
			//@todo aber nicht den ersten voxel
			}
			pixels+=xsize*voxelElemSize*(ysize-Info.Y.getCnt('Y')-1);//die restlichen y-Zeilen
		}
		glTexImage3DEXT(TexType,0,intFormat,xsize,ysize,zsize,0,intFormat == GL_INTENSITY ? GL_LUMINANCE:intFormat,gl_type,pixels_);
		free(pixels_);
		GLuint gluerr = glGetError();
		if(gluerr)
		{
			SGLprintError("%s beim Laden der Textur [GLerror]",gluErrorString(gluerr));
			return GL_FALSE;
		}
		else
		{
			loaded=true;//autolademechanismus austrixen (die Tex is geladen, schließlich habe ich grad Daten reingelesen - nur weiß sie das selbst nich)
			float MBSize=getTexByteSize()/float(1024*1024);
			if(MBSize>1){ SGLprintState("%G MB Bilddaten gelesen, %G MB Texturspeicher für eine %dx%dx%d-Textur belegt",Info.X.getCnt('X')*Info.Y.getCnt('Y')*Info.Z.getCnt('Z')*sizeof(T)/float(1024*1024),MBSize,xsize,ysize,zsize);}
			loaded=false;
		}
		Info.calcGaps(1,xsize-Info.X.getCnt('X')-1,1,ysize-Info.Y.getCnt('Y')-1,1,zsize-Info.Z.getCnt('Z')-1);
		return true;
#undef xsize
#undef ysize
#undef zsize
	}

	template<class T> bool loadMask(Bild<T> &src);
};

#endif
