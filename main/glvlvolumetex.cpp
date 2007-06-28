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
#include "glvlminima.h"

template<class T> bool GLvlVolumeTex::loadMask(Bild<T> &src)
{
	GLint size[3];
	#define xsize	size[0]
	#define ysize	size[1]
	#define zsize	size[2]
	xsize=Info.X.getCnt('X')+2;
	ysize=Info.Y.getCnt('Y')+2;
	zsize=Info.Z.getCnt('Z')+2;//"+2" ist für den Rand (border tut irgendwie nich)
	
	GLenum gl_type;
	if(sizeof(T)==sizeof(GLubyte))gl_type=GL_UNSIGNED_BYTE;
	else if(sizeof(T)==sizeof(GLushort))gl_type=GL_UNSIGNED_SHORT;
	else if(sizeof(T)==sizeof(GLuint))gl_type=GL_UNSIGNED_INT;
	else return false;

	if(!genValidSize(GL_ALPHA4,size,3, GL_ALPHA,gl_type,false))return false;
	//@todo nützt nicht viel - er glaubt er bekäme die Tex rein bekommt aber unten trotzem "out of memory"
	if(!xsize)return false;
	
	T *pixels_=(T*)calloc(xsize*ysize*zsize,sizeof(T));
	register T *pixels=pixels_;

	pixels+=xsize*ysize;//die erste Ebene
	for(int z=0;z<Info.Z.getCnt('Z');z++)
	{
		pixels+=xsize;//erste Zeile leer lassen
		for(int y=0;y<Info.Y.getCnt('Y');y++)
		{
			pixels++;//ersten pixel leer lassen
			if(xsize-Info.X.getCnt('X')-1 <= 0){SGLprintError("Das Bild ist zu groß");}
			pixels=src.copy_line(y,z,pixels);
			pixels+=(xsize-Info.X.getCnt('X')-1);//Wenn das Bild zu groß is, geht der Zeiger wieder zurück (addition neg. werte) und überschreibt nächtes mal, das was falsch war
			//@todo müsste das nich "0" gesetzt werden (hinterer Rand wurde ja überschrieben)
		}
		pixels+=xsize*(ysize-Info.Y.getCnt('Y')-1);//die restlichen y-Zeilen
	}
	
	glTexImage3DEXT(TexType,0,GL_ALPHA4,xsize,ysize,zsize,0,GL_ALPHA,gl_type,pixels_);
	free(pixels_);
	GLenum gluerr;
	if(gluerr = glGetError())
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

bool GLvlVolumeTex::loadSegment(GLvlSegment &src)
{
	if(ID!=0)freeTexture();
	assert(TexType==GL_TEXTURE_3D);//Textype muss 3D sein
	
	const GLenum gl_type=GL_UNSIGNED_BYTE; //gl_type is hier auch fest
	GLint size[3];
	
	//Dim dieser Tex werden direkt ermittelt
	src.getDim(*static_cast<dim*>(&Info.X),*static_cast<dim*>(&Info.Y),*static_cast<dim*>(&Info.Z));
	
	Info.size=SGLVektor(Info.X.mm_size(1),Info.Y.mm_size(1),Info.Z.mm_size(1));

	size[0]=Info.X.getCnt('X')+2;
	size[1]=Info.Y.getCnt('Y')+2;
	size[2]=Info.Z.getCnt('Z')+2;//"+2" ist für den Rand (border tut irgendwie nich)
	
	//Textur initialisieren
	glGenTextures(1, &ID);
	glBindTexture(TexType, ID);
	//und prüfen	
	if(!genValidSize(GL_ALPHA4,size,3, GL_ALPHA,gl_type,false))return false;
	//@todo nützt nicht viel - er glaubt er bekäme die Tex rein bekommt aber unten trotzem "out of memory"
	if(!size[0])return false;
	
	//Pufferbild anlegen, wenn tex vorraussichtlich passt
	Bild_mem<GLubyte> pixels(size[0],size[1],size[2],0);
	
	//Pufferbild schreibem
	for(GLvlSegment::iterator i=src.begin();i!=src.end();i++)
	{
//		if((*i)->volume() <= MAX_MINIMA_SIZE)
		//@todo wenn ich zu große tex nicht schreibe, brauch ich sie auch nicht anlegen
		{
			unsigned short offset[3]={1,1,1};
			src.getOffset(offset,i);
			(*i)->writeTex(offset,pixels);
		}
	}

	glTexImage3DEXT(TexType,0,GL_ALPHA4,size[0],size[1],size[2],0,GL_ALPHA,gl_type,&pixels.at(0));
	
	GLenum gluerr;
	if(gluerr = glGetError())
	{
		SGLprintError("%s beim Laden der Textur [GLerror]",gluErrorString(gluerr));
		return GL_FALSE;
	}
	assert(size[0]>Info.X.getCnt('X')-1);
	assert(size[1]>Info.Y.getCnt('Y')-1);
	assert(size[2]>Info.Z.getCnt('Z')-1);
	
	Info.calcGaps(1,size[0]-Info.X.getCnt('X')-1,1,size[1]-Info.Y.getCnt('Y')-1,1,size[2]-Info.Z.getCnt('Z')-1);
	return true;
}



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

template<class T,class DT> bool GLvlVolumeTex::loadCommon(GLenum gl_type,Bild<T> &src,EVektor<T> PosColor,EVektor<T> NegColor)
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

GLvlVolumeTex::GLvlVolumeTex(): SGLBaseTex()
{
	weich=true;
	repeat=MipMap=false;
//	identity=true;
	TexType=GL_TEXTURE_3D;
}

template<class T> bool GLvlVolumeTex::Load3DImage(Bild<T> &img)
{
	if(ID!=0)freeTexture();
	sglChkExt("GL_EXT_texture3D","Höchstwarscheinlich lassen sich keine nennenswerten Datenmengen laden.",2);
	sglChkExt("GL_ARB_texture_non_power_of_two","Es können keine NPOT-Texturen erzeugt werden, schade eigentlich :-(.",0);
	//@todo GL_ARB_texture_non_power_of_two implementieren
	
	loadImageInfo(img);
	
	glGenTextures(1, &ID);
	glBindTexture(TexType, ID);
	
	
	if(this->renderMode==SGL_MTEX_MODE_COLORMASK)valid=loadMask(img);//@todo wenn loadMask fehlschlägt (warum auch immer) muss das behandelt werden
	
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
	(*static_cast<dim*>(&Info.X))=src.xsize;
	(*static_cast<dim*>(&Info.Y))=src.ysize;;
	(*static_cast<dim*>(&Info.Z))=src.zsize;;
	
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
}

unsigned int GLvlVolumeTex::texKoord2texIndex(const SGLVektor &koord)//Liefert Voxelindexe aus Texturraumkoordinaten
{
	const unsigned short xindex=Info.X.TexKoord2Index(koord.SGLV_X);
	const unsigned short yindex=Info.Y.TexKoord2Index(koord.SGLV_Y);
	const unsigned short zindex=Info.Z.TexKoord2Index(koord.SGLV_Z);
	
	//Ungültige Koord werden numeric_limits<unsigned short>::max(), was ja auch gößer als Info.X.getCnt('X') is
	//@todo hab ich jetzt beschlossen ...
	if(	xindex >= Info.X.getCnt('X') ||
		yindex >= Info.Y.getCnt('Y') ||
		zindex >= Info.Z.getCnt('Z'))
		return std::numeric_limits<unsigned int>::max();
	else return xindex+
		yindex*Info.X.getCnt('X')+ 
		zindex*Info.X.getCnt('X')*Info.Y.getCnt('Y');
}

SGLVektor GLvlVolumeTex::texIndex2texKoord(const unsigned int &idx)//Liefert Texturraumkoordinaten aus Voxelindexen
{
	const double x=Info.X.Index2TexKoord(idx%Info.X.getCnt('X'));
	const double y=Info.Y.Index2TexKoord((idx/Info.X.getCnt('X'))%Info.Y.getCnt('Y'));
	const double z=Info.Z.Index2TexKoord(idx/(Info.X.getCnt('X')*Info.Y.getCnt('Y')));
	return SGLVektor(x,y,z);
}

/*!
    \fn GLvlVolumeTex::calcMatr()
 */
void GLvlVolumeTex::calcMatr(SGLVektor offset)
{
	for(int i=0;i<15;i++)
		((GLdouble*)mm2tex_Matrix)[i]=0;
	((GLdouble*)mm2tex_Matrix)[15]=1;

	offset =
		SGLVektor(Info.X.startgap_mm,Info.Y.startgap_mm,Info.Z.startgap_mm)
		+SGLVektor(Info.X.getElsize('X')/2,Info.Y.getElsize('Y')/2,Info.Z.getElsize('Z')/2)//@todo warum muss das um nen halben Pixel verschoben werden 
		//eig müsste GL_NEAREST doch von -.5 bis .5 in der Farbe des Eintrages zeichnen, und nicht 0 bis 1
		-offset-GLvlVolumeTex::masteroffset;
	
	mm2tex_Matrix[0][0]=1/Info.X.outer_mm_size();
	mm2tex_Matrix[1][1]=1/Info.Y.outer_mm_size();
	mm2tex_Matrix[2][2]=1/Info.Z.outer_mm_size();
	
	mm2tex_Matrix[3][0]=mm2tex_Matrix[0][0]*offset.SGLV_X;
	mm2tex_Matrix[3][1]=mm2tex_Matrix[1][1]*offset.SGLV_Y;
	mm2tex_Matrix[3][2]=mm2tex_Matrix[2][2]*offset.SGLV_Z;
}

/*!
    \fn GLvlVolumeTex::loadTint(VImage i)
 */
void GLvlVolumeTex::loadTint(VImage i)
{
	SGLprintState("lade Aktivierungsmap");
	SGLshPtr<GLvlVolumeTex> p(new GLvlVolumeTex());
	p->renderMode=SGL_MTEX_MODE_TINT;
	p->Load3DImage(i);
	p->calcMatr();
	p->ResetTransformMatrix((const GLdouble*)p->mm2tex_Matrix);
	p->weich=true;
	multitex=p;
}


/*!
    \fn GLvlVolumeTex::setupPal(unsigned short start,unsigned short end)
 */
unsigned short GLvlVolumeTex::setupPal(unsigned short start,unsigned short end,bool scale)
{
	GLuint gluerr;
	unsigned short size=1;
	while(size<(end+1))size<<=1;
	if(start==0){
		SGLprintWarning("Ignoriere Paletteneintrag 0");start=1;
	}
	GLfloat *paletteL= (GLfloat*)calloc(size,sizeof(GLfloat));
	GLfloat *paletteA= (GLfloat*)calloc(size,sizeof(GLfloat));
	for(unsigned short i=start;i<=end;i++)
	{
		paletteL[i]=scale ? (i-start)/float(end-start):i/float(end);
		paletteA[i]=1;
	}
	glPixelTransferi(GL_MAP_COLOR,true);
	glPixelMapfv(GL_PIXEL_MAP_I_TO_R,size,paletteL);
	glPixelMapfv(GL_PIXEL_MAP_I_TO_G,size,paletteL);
	glPixelMapfv(GL_PIXEL_MAP_I_TO_B,size,paletteL);
	glPixelMapfv(GL_PIXEL_MAP_I_TO_A,size,paletteA);
//	glColorTable(TexType,GL_LUMINANCE_ALPHA,size,GL_LUMINANCE_ALPHA,GL_FLOAT,palette);
	free(paletteL);free(paletteA);
	if(gluerr = glGetError())
	{
		SGLprintError("%s beim Laden der Palette [GLerror]",gluErrorString(gluerr));
		return 0;
	}
	return size;
}

SGLVektor GLvlVolumeTex::masteroffset(0,0,0);
