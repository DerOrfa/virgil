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
template<class T> bool GLvlVolumeTex::fillIndexData(GLenum gl_type,VImage &src)
{
	GLuint gluerr;
	GLint size[3];
	#define xsize	size[0]
	#define ysize	size[1]
	#define zsize	size[2]
	xsize=dim.X.cnt+2;
	ysize=dim.Y.cnt+2;
	zsize=dim.Z.cnt+2;//"+2" ist für den Rand (border tut irgendwie nich)
	if(!genValidSize(GL_COLOR_INDEX8_EXT,size,3, GL_COLOR_INDEX,gl_type,false))return false;
	//@todo nützt nicht viel - er glaubt er bekäme die Tex rein bekommt aber unten trotzem "out of memory"
	if(!xsize)return false;

	T min=numeric_limits<T>::max(),max=numeric_limits<T>::min();
	T *pixels_=(T*)calloc(xsize*ysize*zsize,sizeof(T));
	register T *pixels=pixels_;

	int pixMax;
	VPointer data;
	int z=0;
	
	pixels+=xsize*ysize;//die erste Ebene
	for(;z<dim.Z.cnt && VSelectBand("Vol2Tex",src,z,&pixMax,&data);z++)
	{
		VUByte *pix= (VUByte *)data;
		
		pixels+=xsize;//erste Zeile leer lassen
		for(int y=0;y<dim.Y.cnt;y++)
		{
			pixels++;//ersten pixel leer lassen
			if(xsize-dim.X.cnt-1 <= 0){SGLprintError("Das Bild ist zu groß");}
			for(int x=0;x<dim.X.cnt;x++)
			{
				if(min > *pix)
					min =  *pix;
				if(max < *pix)
					max = *pix;
				(*pixels)=(*pix);
				pixels++;pix++;
				pixMax--;
			}
			pixels+=(xsize-dim.X.cnt-1);//Wenn das Bild zu groß is, geht der Zeiger wieder zurück (addition neg. werte) und überschreibt nächtes mal, das was falsch war
			//@todo müsste das nich "0" gesetzt werden
		}
		pixels+=xsize*(ysize-dim.Y.cnt-1);//die restlichen y-Zeilen
		if(pixMax){SGLprintError("Es wurden nicht alle Voxel gelesen");}
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
		SGLprintState("%G MB Bilddaten gelesen, %G MB Texturspeicher für eine %dx%dx%d-Textur belegt",dim.X.cnt*dim.Y.cnt*dim.Z.cnt*sizeof(T)/float(1024*1024),getTexByteSize()/float(1024*1024),xsize,ysize,zsize);
		loaded=false;
	}
	dim.calcGaps(1,xsize-dim.X.cnt-1,1,ysize-dim.Y.cnt-1,1,zsize-dim.Z.cnt-1);
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
	xsize=dim.X.cnt+2;
	ysize=dim.Y.cnt+2;
	zsize=dim.Z.cnt+2;
	if(!genValidSize(intFormat,size,3, intFormat == GL_INTENSITY ? GL_LUMINANCE:intFormat,gl_type,false))return false;
	//@todo nützt nichts - er glaubt er bekäme die Tex rein bekommt aber unten trotzem "out of memory"
	
	T *pixels=(T*)calloc(xsize*ysize*zsize*voxelElemSize,sizeof(T));
	T *pixels_=pixels;
	pixels+=xsize*voxelElemSize*ysize;//die erste Ebene
	
	int pixMax;
	VPointer data;
	int z=0;
	double min=VPixelMinValue(src),max=VPixelMaxValue(src);
	
	for(;z<dim.Z.cnt && VSelectBand("Vol2Tex",src,z,&pixMax,&data);z++)
	{
		ST *pix= (ST *)data;
		
		pixels+=xsize*voxelElemSize;//erste Zeile leer lassen
		for(int y=0;y<dim.Y.cnt;y++)
		{
			pixels+=voxelElemSize;//ersten Voxel (und seinen alpha) leer lassen
			pixMax-=dim.X.cnt ;
			
			if(intFormat== GL_LUMINANCE_ALPHA)
				copyXline(pixels,pix,dim.X.cnt);
			else 
				mapXline(pixels,pix,dim.X.cnt,PosColor,NegColor);
			if(xsize-dim.X.cnt-1 <= 0){SGLprintError("Das Bild ist zu groß");}
			pixels+=(xsize-dim.X.cnt-1)*voxelElemSize;//Wenn das Bild zu groß is, geht der Zeiger wieder zurück (addition neg. werte) und überschreibt nächtes mal, das was falsch war
		}
		pixels+=xsize*voxelElemSize*(ysize-dim.Y.cnt-1);//die restlichen y-Zeilen
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
		SGLprintState("%G MB Bilddaten gelesen, %G MB Texturspeicher für eine %dx%dx%d-Textur belegt",dim.X.cnt*dim.Y.cnt*dim.Z.cnt*sizeof(T)/float(1024*1024),getTexByteSize()/float(1024*1024),xsize,ysize,zsize);
		loaded=false;
	}
	dim.calcGaps(1,xsize-dim.X.cnt-1,1,ysize-dim.Y.cnt-1,1,zsize-dim.Z.cnt-1);
	return true;
	#undef xsize
	#undef ysize
	#undef zsize
}

GLvlVolumeTex::GLvlVolumeTex(): SGLBaseTex()
{
	weich=false;
	repeat=MipMap=false;
}

bool GLvlVolumeTex::Load3DImage(VImage src)
{
	if(ID!=0)freeTexture();
	sglChkExt("GL_EXT_texture3D","Höchstwarscheinlich lassen sich keine nennenswerten Datenmengen laden.",2);
	sglChkExt("GL_ARB_texture_non_power_of_two","Es können keine NPOT-Texturen erzeugt werden, schade eigentlich :-(.",0);
	
	TexType=GL_TEXTURE_3D;
	loadImageInfo(src);
	
	glGenTextures(1, &ID);
	glBindTexture(TexType, ID);
	EVektor<GLfloat> PosColor(3),NegColor(3);
	switch(VPixelRepn(src))
	{
//		case VBitRepn:		VoxelType=GL_BITMAP;VoxelSize=1;break;//@todo Wie wird das Codiert ??
		case VFloatRepn:	/*PosColor[0]=NegColor[0]=1;*/
							PosColor[0]=PosColor[1]=.1;
							NegColor[0]=NegColor[2]=.2;
							valid=fillFloatData<GLfloat,VFloat>(GL_FLOAT,src,PosColor,NegColor);
							break;
		case VDoubleRepn:	valid=fillFloatData<GLfloat,VDouble>(GL_FLOAT,src);SGLprintWarning("rechne double-Werte auf Float runter");break;
		case VUByteRepn:	
		{
			if(sglChkExt("GL_EXT_paletted_texture","Die Voxelwerte können nicht indiziert werden.",1))
				valid=fillIndexData<GLubyte>(GL_UNSIGNED_BYTE,src);
			else
				valid=fillFloatData<GLubyte,VUByte>(GL_UNSIGNED_BYTE,src);
		}break;
		case VSByteRepn:	valid=fillIndexData<GLbyte>(GL_BYTE,src);break;
		case VShortRepn:	valid=fillIndexData<GLshort>(GL_SHORT,src);break;//@todo is das jetzt signed ??
		case VLongRepn:		valid=fillIndexData<GLint>(GL_INT,src);break;//@todo *grml* GL kennt nur 32Bit-INT ??
	}
	return valid;
}



/*!
    \fn GLvlVolumeTex::calcDim(float VoxelDim[3])
 */
void GLvlVolumeTex::loadImageInfo(VImage &src)
{
	char *AttrStr;
	if(VGetAttr(VImageAttrList(src),"voxel",NULL,VStringRepn,(VPointer)&AttrStr)==VAttrFound)
		sscanf(AttrStr,"%f %f %f",&dim.Columns.Elsize,&dim.Rows.Elsize,&dim.Bands.Elsize);
	else
	{
		dim.Columns.Elsize=dim.Rows.Elsize=dim.Bands.Elsize=1;
		SGLprintWarning("Keine Informationen zur Größe der Voxel gefunden! Nehme 1x1x1mm an.");
	}
	
	dim.Columns.cnt =VImageNColumns(src);
	dim.Rows.cnt =VImageNRows(src);
	dim.Bands.cnt =VImageNBands(src);
	
	dim.size=SGLVektor(
		dim.X.mm_size(1),
		dim.Y.mm_size(1),
		dim.Z.mm_size(1)
	);
}

//@todo noch nich getestet
unsigned int GLvlVolumeTex::texKoord2texIndex(const SGLVektor &koord)
{
	const unsigned short xindex=dim.X.outerTexKoord2Index(koord.SGLV_X);
	const unsigned short yindex=dim.Y.outerTexKoord2Index(koord.SGLV_Y);
	const unsigned short zindex=dim.Z.outerTexKoord2Index(koord.SGLV_Z);
	
	return xindex+
		yindex*dim.X.holeSize()+ //jede Zeile enth dim.X.holeSize() x'e
		zindex*dim.X.holeSize()*dim.Y.holeSize();
}

SGLVektor GLvlVolumeTex::texIndex2texKoord(const unsigned int &idx)
{
	const double x=dim.X.Index2TexKoord(idx%dim.X.cnt);
	const double y=dim.Y.Index2TexKoord((idx/dim.X.cnt)%dim.Y.cnt);
	const double z=dim.Z.Index2TexKoord(idx/(dim.X.cnt*dim.Y.cnt));
/*
X:pos%size_x
Y:(pos/size_x)%size_y
pos/size_xy
*/
	return SGLVektor(x,y,z);
}

/*!
    \fn GLvlVolumeTex::calcMatr()
 */
void GLvlVolumeTex::calcMatr()
{
	printf("Rechne\n");

	for(int i=0;i<15;i++)
		((GLdouble*)mm2tex_Matrix)[i]=0;
	((GLdouble*)mm2tex_Matrix)[15]=1;

	mm2tex_Matrix[0][0]=1/dim.X.outer_mm_size;
	mm2tex_Matrix[1][1]=1/dim.Y.outer_mm_size;
	mm2tex_Matrix[2][2]=1/dim.Z.outer_mm_size;
	
	mm2tex_Matrix[3][0]=mm2tex_Matrix[0][0]*(dim.X.startgap_mm+dim.X.Elsize/2);//@todo warum muss das um nen halben Pixel verschoben werden
	mm2tex_Matrix[3][1]=mm2tex_Matrix[1][1]*(dim.Y.startgap_mm+dim.Y.Elsize/2);
	mm2tex_Matrix[3][2]=mm2tex_Matrix[2][2]*(dim.Z.startgap_mm+dim.Z.Elsize/2);
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
	if(start<1){SGLprintError("Die Palette darf erst bei \"1\" beginnen, nicht bei \"%d\"",start);start=1;}
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
