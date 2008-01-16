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
//#include "glvlminima.h"

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

// bool GLvlVolumeTex::loadSegment(GLvlSegment &src)
// {
// 	if(ID!=0)freeTexture();
// 	assert(TexType==GL_TEXTURE_3D);//Textype muss 3D sein
//
// 	const GLenum gl_type=GL_UNSIGNED_BYTE; //gl_type is hier auch fest
// 	GLint size[3];
//
// 	//Dim dieser Tex werden direkt ermittelt
// 	src.getDim(*static_cast<dim*>(&Info.X),*static_cast<dim*>(&Info.Y),*static_cast<dim*>(&Info.Z));
//
// 	Info.size=SGLVektor(Info.X.mm_size(1),Info.Y.mm_size(1),Info.Z.mm_size(1));
//
// 	size[0]=Info.X.getCnt('X')+2;
// 	size[1]=Info.Y.getCnt('Y')+2;
// 	size[2]=Info.Z.getCnt('Z')+2;//"+2" ist für den Rand (border tut irgendwie nich)
//
// 	//Textur initialisieren
// 	glGenTextures(1, &ID);
// 	glBindTexture(TexType, ID);
// 	//und prüfen
// 	if(!genValidSize(GL_ALPHA4,size,3, GL_ALPHA,gl_type,false))return false;
// 	//@todo nützt nicht viel - er glaubt er bekäme die Tex rein bekommt aber unten trotzem "out of memory"
// 	if(!size[0])return false;
//
// 	//Pufferbild anlegen, wenn tex vorraussichtlich passt
// 	Bild_mem<GLubyte> pixels(size[0],size[1],size[2],0);
//
// 	//Pufferbild schreibem
// 	for(GLvlSegment::iterator i=src.begin();i!=src.end();i++)
// 	{
// //		if((*i)->volume() <= MAX_MINIMA_SIZE)
// 		//@todo wenn ich zu große tex nicht schreibe, brauch ich sie auch nicht anlegen
// 		{
// 			unsigned short offset[3]={1,1,1};
// 			src.getOffset(offset,i);
// 			(*i)->writeTex(offset,pixels);
// 		}
// 	}
//
// 	glTexImage3DEXT(TexType,0,GL_ALPHA4,size[0],size[1],size[2],0,GL_ALPHA,gl_type,&pixels.at(0));
//
// 	GLenum gluerr;
// 	if(gluerr = glGetError())
// 	{
// 		SGLprintError("%s beim Laden der Textur [GLerror]",gluErrorString(gluerr));
// 		return GL_FALSE;
// 	}
// 	assert(size[0]>Info.X.getCnt('X')-1);
// 	assert(size[1]>Info.Y.getCnt('Y')-1);
// 	assert(size[2]>Info.Z.getCnt('Z')-1);
//
// 	Info.calcGaps(1,size[0]-Info.X.getCnt('X')-1,1,size[1]-Info.Y.getCnt('Y')-1,1,size[2]-Info.Z.getCnt('Z')-1);
// 	return true;
// }

GLvlVolumeTex::GLvlVolumeTex():SGLBaseTex()
{
	weich=true;
	repeat=MipMap=false;
//	identity=true;
	TexType=GL_TEXTURE_3D;
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

SGLVektor GLvlVolumeTex::masteroffset(0,0,0);
