//
// C++ Implementation: glvlvolumetex
//
// Description: 
//
//
// Author: Enrico Reimer,,, <enni@Akira>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "glvlvolumetex.h"
#include <GL/glu.h>
#include <assert.h>

//@todo das ding is vieel zu groß :-(
template<class T> bool GLvlVolumeTex::fillIndexData(Bild<T> &src)
{
	GLuint gluerr;
	GLenum gl_type;
	
	if(typeid(T)==typeid(GLubyte))gl_type=GL_UNSIGNED_BYTE;
	else if(typeid(T)==typeid(GLbyte))gl_type=GL_BYTE;
	else if(typeid(T)==typeid(GLushort))gl_type=GL_UNSIGNED_SHORT;
	else if(typeid(T)==typeid(GLshort))gl_type=GL_SHORT;
	else if(typeid(T)==typeid(GLuint))gl_type=GL_UNSIGNED_INT;
	else if(typeid(T)==typeid(GLint))gl_type=GL_INT;
	else return false;
	
	GLint size[3];
	#define xsize	size[0]
	#define ysize	size[1]
	#define zsize	size[2]
	xsize=Info.X.cnt+2;
	ysize=Info.Y.cnt+2;
	zsize=Info.Z.cnt+2;//"+2" ist für den Rand (border tut irgendwie nich)
	if(!genValidSize(GL_COLOR_INDEX8_EXT,size,3, GL_COLOR_INDEX,gl_type,false))return false;
	//@todo nützt nicht viel - er glaubt er bekäme die Tex rein bekommt aber unten trotzem "out of memory"
	if(!xsize)return false;

	T min=numeric_limits<T>::max(),max=numeric_limits<T>::min();
	T *pixels_=(T*)calloc(xsize*ysize*zsize,sizeof(T));
	register T *pixels=pixels_;

	int pixMax;
	int index=0;
	pixels+=xsize*ysize;//die erste Ebene
	for(int z=0;z<Info.Z.cnt;z++)
	{
		pixels+=xsize;//erste Zeile leer lassen
		for(int y=0;y<Info.Y.cnt;y++)
		{
			pixels++;//ersten pixel leer lassen
			if(xsize-Info.X.cnt-1 <= 0){SGLprintError("Das Bild ist zu groß");}
			for(int x=0;x<Info.X.cnt;x++)
			{
				T pix=src.at(index);
				min = min <? pix;
				max = max >? pix;
				(*pixels)=pix;
				pixels++;index++;
				pixMax--;
			}
			pixels+=(xsize-Info.X.cnt-1);//Wenn das Bild zu groß is, geht der Zeiger wieder zurück (addition neg. werte) und überschreibt nächtes mal, das was falsch war
			//@todo müsste das nich "0" gesetzt werden
		}
		pixels+=xsize*(ysize-Info.Y.cnt-1);//die restlichen y-Zeilen
		if(pixMax){
			SGLprintError("Es wurden nicht alle Voxel gelesen");
		}
	}
	
	unsigned short palsize=setupPal(min,max,true);
	if(palsize){SGLprintInfo("%d-Farb-Palette initialisiert",palsize);}
	else {SGLprintError("Palette konnte nicht initialisiert werden");}
	
	glTexImage3DEXT(TexType,0,GL_COLOR_INDEX8_EXT,xsize,ysize,zsize,0,GL_COLOR_INDEX,gl_type,pixels_);
	free(pixels_);
	if(gluerr = glGetError())
	{
		SGLprintError("%s beim Laden der Textur [GLerror]",gluErrorString(gluerr));
		return GL_FALSE;
	}
	else
	{
		loaded=true;//autolademechanismus austrixen (die Tex is geladen, schließlich habe ich grad Daten reingelesen - nur weiß sie das selbst nich)
		SGLprintState("%G MB Bilddaten gelesen, %G MB Texturspeicher für eine %dx%dx%d-Textur belegt",Info.X.cnt*Info.Y.cnt*Info.Z.cnt*sizeof(T)/float(1024*1024),getTexByteSize()/float(1024*1024),xsize,ysize,zsize);
		loaded=false;
	}
	Info.calcGaps(1,xsize-Info.X.cnt-1,1,ysize-Info.Y.cnt-1,1,zsize-Info.Z.cnt-1);
	return true;
	#undef xsize
	#undef ysize
	#undef zsize
}

template<class T,class DT> inline bool copyXline(register DT *&dst,register T &src,register int xcnt)
{
	if(xcnt<=0)
		return false;
	DT MAX=numeric_limits<DT>::max();
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
		for(int i=ergVekt.size()-1;i>=0;i--)
			*(dst++)=ergVekt[i];
	}
	return true;
}

template<class T,class DT> inline bool fillXline(register DT *&dst,register int xcnt)
{
	if(xcnt<=0)
		return false;
	else dst+=xcnt;//calloc setzt schon alle auf NULL - brauch hier nix machen
	return true;
}

template<class T,class ST> bool GLvlVolumeTex::fillFloatData(GLenum gl_type,VImage &src,EVektor<T> PosColor,EVektor<T> NegColor)
{
	#define xsize	size[0]
	#define ysize	size[1]
	#define zsize	size[2]
	
	
	unsigned short voxelElemSize;
	GLint intFormat;
	switch(PosColor.size())
	{
		case 1:
				voxelElemSize=1;
				intFormat=GL_INTENSITY;break;
		case 2:SGLprintError("Ungültiger Farbvektor");break;
		case 3:
				voxelElemSize=3;
				intFormat=GL_RGB;break;
		case 4:
				intFormat=GL_RGBA;
				voxelElemSize=4;
				break;
		default:
			intFormat=GL_LUMINANCE_ALPHA;
			voxelElemSize=2;
			break;
	}
	//Größe der Textur ("+2" ist für den Rand)
	GLint size[3];
	xsize=Info.X.cnt+2;
	ysize=Info.Y.cnt+2;
	zsize=Info.Z.cnt+2;
	if(!genValidSize(intFormat,size,3, intFormat == GL_INTENSITY ? GL_LUMINANCE:intFormat,gl_type,false))return false;
	//@todo nützt nichts - er glaubt er bekäme die Tex rein bekommt aber unten trotzem "out of memory"
	
	T *pixels=(T*)calloc(xsize*ysize*zsize*voxelElemSize,sizeof(T));
	T *pixels_=pixels;
	pixels+=xsize*voxelElemSize*ysize;//die erste Ebene
	
	int pixMax;
	VPointer data;
	int z=0;
	double min=VPixelMinValue(src),max=VPixelMaxValue(src);
	
	for(;z<Info.Z.cnt && VSelectBand("Vol2Tex",src,z,&pixMax,&data);z++)
	{
		ST *pix= (ST *)data;
		
		pixels+=xsize*voxelElemSize;//erste Zeile leer lassen
		for(int y=0;y<Info.Y.cnt;y++)
		{
			pixels+=voxelElemSize;//ersten Voxel (und seinen alpha) leer lassen
			pixMax-=Info.X.cnt ;
			
			if(intFormat== GL_LUMINANCE_ALPHA)
				copyXline(pixels,pix,Info.X.cnt);
			else 
				mapXline(pixels,pix,Info.X.cnt,PosColor,NegColor);
			if(xsize-Info.X.cnt-1 <= 0){SGLprintError("Das Bild ist zu groß");}
			pixels+=(xsize-Info.X.cnt-1)*voxelElemSize;//Wenn das Bild zu groß is, geht der Zeiger wieder zurück (addition neg. werte) und überschreibt nächtes mal, das was falsch war
		}
		pixels+=xsize*voxelElemSize*(ysize-Info.Y.cnt-1);//die restlichen y-Zeilen
		if(pixMax){SGLprintError("Es wurden nicht alle Voxel gelesen");}
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
		SGLprintState("%G MB Bilddaten gelesen, %G MB Texturspeicher für eine %dx%dx%d-Textur belegt",Info.X.cnt*Info.Y.cnt*Info.Z.cnt*sizeof(T)/float(1024*1024),getTexByteSize()/float(1024*1024),xsize,ysize,zsize);
		loaded=false;
	}
	Info.calcGaps(1,xsize-Info.X.cnt-1,1,ysize-Info.Y.cnt-1,1,zsize-Info.Z.cnt-1);
	return true;
	#undef xsize
	#undef ysize
	#undef zsize
}

GLvlVolumeTex::GLvlVolumeTex(): SGLBaseTex()
{
	weich=true;
	repeat=MipMap=false;
	TexType=GL_TEXTURE_3D;
}

template<class T> bool GLvlVolumeTex::Load3DImage(Bild<T> &img)
{
	if(ID!=0)freeTexture();
	sglChkExt("GL_EXT_texture3D","Höchstwarscheinlich lassen sich keine nennenswerten Datenmengen laden.",2);
	sglChkExt("GL_ARB_texture_non_power_of_two","Es können keine NPOT-Texturen erzeugt werden, schade eigentlich :-(.",0);
	
	loadImageInfo(img);
	
	glGenTextures(1, &ID);
	glBindTexture(TexType, ID);
	EVektor<GLfloat> PosColor(3),NegColor(3);
	
	if(sglChkExt("GL_EXT_paletted_texture","Die Voxelwerte können nicht indiziert werden.",1))
	{
		valid=fillIndexData(img);
/*		*/
	}
	
	if(!valid)
	{
/*		if(typeid(T)==typeid(GLubyte))valid=fillIndexData<GLubyte>(GL_UNSIGNED_BYTE,img);
		else if(typeid(T)==typeid(GLbyte))valid=fillIndexData<GLbyte>(GL_BYTE,img);
		else if(typeid(T)==typeid(GLushort))valid=fillIndexData<GLushort>(GL_UNSIGNED_SHORT,img);
		else if(typeid(T)==typeid(GLshort))valid=fillIndexData<GLshort>(GL_SHORT,img);
		else if(typeid(T)==typeid(GLuint))valid=fillIndexData<GLuint>(GL_UNSIGNED_INT,img);
		else if(typeid(T)==typeid(GLint))valid=fillIndexData<GLint>(GL_INT,img);*/
	}
	
/*	switch(typeid(T))
	{
//		case VBitRepn:		VoxelType=GL_BITMAP;VoxelSize=1;break;//@todo Wie wird das Codiert ??
		case VFloatRepn:	PosColor[0]=NegColor[0]=1;
							PosColor[0]=PosColor[1]=.1;
							NegColor[0]=NegColor[2]=.2;
							valid=fillFloatData<GLfloat,T>(GL_FLOAT,img,PosColor,NegColor);
							break;
		case VDoubleRepn:	valid=fillFloatData<GLfloat,T>(GL_FLOAT,img);SGLprintWarning("rechne double-Werte auf Float runter");break;
		case VUByteRepn:	
		{
			if()
				
			else
				valid=fillFloatData<GLubyte,T>(GL_UNSIGNED_BYTE,img);
		}break;
		case VSByteRepn:	valid=fillIndexData<GLbyte>(GL_BYTE,img);break;
		case VShortRepn:	valid=fillIndexData<GLshort>(GL_SHORT,img);break;//@todo is das jetzt signed ??
		case VLongRepn:		valid=fillIndexData<GLint>(GL_INT,img);break;//@todo *grml* GL kennt nur 32Bit-INT ??
	}*/
	return valid;
}

bool GLvlVolumeTex::Load3DImage(VImage src)
{
	switch(VPixelRepn(src))
	{
		case VBitRepn:		{Bild_vimage<VBit> i(src);valid=Load3DImage(i);}break;
		case VFloatRepn:	{Bild_vimage<VFloat> i(src);valid=Load3DImage(i);}break;
		case VDoubleRepn:	{Bild_vimage<VDouble> i(src); valid=Load3DImage(i);}break;
		case VUByteRepn:	{Bild_vimage<VUByte> i(src);valid=Load3DImage(i);}break;
		case VSByteRepn:	{Bild_vimage<VSByte> i(src);valid=Load3DImage(i);}break;
		case VShortRepn:	{Bild_vimage<VShort> i(src);valid=Load3DImage(i);}break;
		case VLongRepn:		{Bild_vimage<VLong> i(src);valid=Load3DImage(i);}break;
	}
	
	return valid;
}

template<class T> void GLvlVolumeTex::loadImageInfo(Bild<T> &src)
{
	Info.X=*((GLvlVolumeTex::dimData*)&src.xsize);
	Info.Y=*((GLvlVolumeTex::dimData*)&src.ysize);;
	Info.Z=*((GLvlVolumeTex::dimData*)&src.zsize);;
	
	Info.size=SGLVektor(
		Info.X.mm_size(1),
		Info.Y.mm_size(1),
		Info.Z.mm_size(1)
	);
}

unsigned int GLvlVolumeTex::texKoord2texIndex(const SGLVektor &koord)//Liefert Voxelindexe aus Texturraumkoordinaten
{
	const unsigned short xindex=Info.X.TexKoord2Index(koord.SGLV_X);
	const unsigned short yindex=Info.Y.TexKoord2Index(koord.SGLV_Y);
	const unsigned short zindex=Info.Z.TexKoord2Index(koord.SGLV_Z);
	
	return xindex+
		yindex*Info.X.cnt+ //jede Zeile enth Info.X.holeSize() x'e
		zindex*Info.X.cnt*Info.Y.cnt;
}

SGLVektor GLvlVolumeTex::texIndex2texKoord(const unsigned int &idx)//Liefert Texturraumkoordinaten aus Voxelindexen
{
	const double x=Info.X.Index2TexKoord(idx%Info.X.cnt);
	const double y=Info.Y.Index2TexKoord((idx/Info.X.cnt)%Info.Y.cnt);
	const double z=Info.Z.Index2TexKoord(idx/(Info.X.cnt*Info.Y.cnt));
	return SGLVektor(x,y,z);
}

/*!
    \fn GLvlVolumeTex::calcMatr()
 */
void GLvlVolumeTex::calcMatr()
{
	for(int i=0;i<15;i++)
		((GLdouble*)mm2tex_Matrix)[i]=0;
	((GLdouble*)mm2tex_Matrix)[15]=1;

	mm2tex_Matrix[0][0]=1/Info.X.outer_mm_size;
	mm2tex_Matrix[1][1]=1/Info.Y.outer_mm_size;
	mm2tex_Matrix[2][2]=1/Info.Z.outer_mm_size;
	
	mm2tex_Matrix[3][0]=mm2tex_Matrix[0][0]*(Info.X.startgap_mm+Info.X.Elsize/2);//@todo warum muss das um nen halben Pixel verschoben werden 
	mm2tex_Matrix[3][1]=mm2tex_Matrix[1][1]*(Info.Y.startgap_mm+Info.Y.Elsize/2);//eig müsste GL_NEAREST doch von -.5 bis .5 in der Farbe des Eintrages zeichnen, und nicht 0 bis 1
	mm2tex_Matrix[3][2]=mm2tex_Matrix[2][2]*(Info.Z.startgap_mm+Info.Z.Elsize/2);
}


/*!
    \fn GLvlVolumeTex::loadTint(VImage i)
 */
void GLvlVolumeTex::loadTint(VImage i)
{
	SGLprintState("lade Aktivierungsmap");
	GLvlVolumeTex *p=new GLvlVolumeTex();//Nich löschen das Teil !
	p->Load3DImage(i);
	p->renderMode=SGL_TEX_MODE_TINT;
	p->envColor[0]=p->envColor[3]=1;
	p->calcMatr();
	p->ResetTransformMatrix((const GLdouble*)p->mm2tex_Matrix);
	p->weich=true;
	multitex.reset(p);
}


/*!
    \fn GLvlVolumeTex::setupPal(unsigned short start,unsigned short end)
 */
unsigned short GLvlVolumeTex::setupPal(unsigned short start,unsigned short end,bool scale)
{
	GLuint gluerr;
	struct d_byte{GLfloat lum;GLfloat alpha;};
	unsigned short size=1;
	while(size<(end+1))size<<=1;
	if(!start){
		SGLprintWarning("Ignoriere Paletteneintrag 0");start=1;
	}
	d_byte *palette= (d_byte*)calloc(size,sizeof(d_byte));
	for(unsigned short i=start;i<=end;i++)
	{
		palette[i].lum=scale ? (i-start)/float(end-start):i/float(end);
		palette[i].alpha=1;
	}
	glColorTable(TexType,GL_LUMINANCE_ALPHA,size,GL_LUMINANCE_ALPHA,GL_FLOAT,palette);
	free(palette);
	if(gluerr = glGetError())
	{
		SGLprintError("%s beim Laden der Palette [GLerror]",gluErrorString(gluerr));
		return 0;
	}
	return size;
}
