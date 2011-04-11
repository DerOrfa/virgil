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
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <assert.h>
#include <qglobal.h>
//#include "glvlminima.h"


GLvlVolumeTex::GLvlVolumeTex(const isis::data::Image &img):SGLBaseTex(),rowDim(img,isis::data::rowDim),colDim(img,isis::data::columnDim),sliceDim(img,isis::data::sliceDim)
{
	weich=true;
	repeat=MipMap=false;
//	identity=true;
	TexType=GL_TEXTURE_3D;

	if(ID!=0)freeTexture();
	sglChkExt("GL_EXT_texture3D","Höchstwarscheinlich lassen sich keine nennenswerten Datenmengen laden.",2);

	glGenTextures(1, &ID);
	valid=load(img);
}

unsigned int GLvlVolumeTex::texKoord2texIndex(const SGLVektor &koord)//Liefert Voxelindexe aus Texturraumkoordinaten
{
/*	const unsigned short xindex=rowDim.TexCoord2Index(koord.SGLV_X);
	const unsigned short yindex=colDim.TexCoord2Index(koord.SGLV_Y);
	const unsigned short zindex=sliceDim.TexCoord2Index(koord.SGLV_Z);

	//UngÃ¼ltige Koord werden numeric_limits<unsigned short>::max(), was ja auch gÃ¶ÃŸer als Info.X.getCnt('X') is
	//@todo hab ich jetzt beschlossen ...
	if(	xindex >= Info.X.getCnt('X') ||
		yindex >= Info.Y.getCnt('Y') ||
		zindex >= Info.Z.getCnt('Z'))

	else return xindex+
		yindex*Info.X.getCnt('X')+
		zindex*Info.X.getCnt('X')*Info.Y.getCnt('Y');
		*/
	qWarning("implement me");
	return std::numeric_limits<unsigned int>::max();
}

SGLVektor GLvlVolumeTex::texIndex2texKoord(const unsigned int &idx)//Liefert Texturraumkoordinaten aus Voxelindexen
{
/*	const double x=Info.X.Index2TexKoord(idx%Info.X.getCnt('X'));
	const double y=Info.Y.Index2TexKoord((idx/Info.X.getCnt('X'))%Info.Y.getCnt('Y'));
	const double z=Info.Z.Index2TexKoord(idx/(Info.X.getCnt('X')*Info.Y.getCnt('Y')));
	*/
	qWarning("implement me");
	return SGLVektor(0,0,0);
}

/*!
	\fn GLvlVolumeTex::calcMatr()
 */
void GLvlVolumeTex::calcMatr(SGLVektor offset)
{
/*	for(int i=0;i<15;i++)
		((GLdouble*)mm2tex_Matrix)[i]=0;
	((GLdouble*)mm2tex_Matrix)[15]=1;

	offset =
		SGLVektor(rowDim.startgap_mm,colDim.startgap_mm,sliceDim.startgap_mm)
		+SGLVektor(Info.X.getElsize('X')/2,Info.Y.getElsize('Y')/2,Info.Z.getElsize('Z')/2)//@todo warum muss das um nen halben Pixel verschoben werden
		//eig mÃ¼sste GL_NEAREST doch von -.5 bis .5 in der Farbe des Eintrages zeichnen, und nicht 0 bis 1
		-offset-GLvlVolumeTex::masteroffset;

	mm2tex_Matrix[0][0]=1/Info.X.outer_mm_size();
	mm2tex_Matrix[1][1]=1/Info.Y.outer_mm_size();
	mm2tex_Matrix[2][2]=1/Info.Z.outer_mm_size();

	mm2tex_Matrix[3][0]=mm2tex_Matrix[0][0]*offset.SGLV_X;
	mm2tex_Matrix[3][1]=mm2tex_Matrix[1][1]*offset.SGLV_Y;
	mm2tex_Matrix[3][2]=mm2tex_Matrix[2][2]*offset.SGLV_Z;
	*/
}

bool GLvlVolumeTex::load(const isis::data::Image &data)
{
	class copyToChunk:public isis::data::Chunk::VoxelOp<GLushort>{
	public:
		isis::data::MemChunk<GLushort> &target;
		copyToChunk(isis::data::MemChunk<GLushort> &ch):target(ch){};
		bool operator()( GLushort &vox, const isis::util::FixedVector<size_t, 4> &pos ){
			target.voxel<GLushort>(pos[0]+1,pos[1]+1,pos[2]+1) = vox;
		}
	};
	if(!sglChkExt("GL_ARB_texture_non_power_of_two","NPOT-textures are not supportet. Aborting...",0))
		exit(-1);

	//Größe der Textur ("+2" ist für den Rand)
	const isis::util::FixedVector<size_t,4> size=data.getSizeAsVector()+2;
	isis::data::MemChunk<GLushort> chunk(size[isis::data::rowDim]*2,size[isis::data::columnDim],size[isis::data::sliceDim]);

	copyToChunk copyOp(chunk);
	const_cast<isis::data::Image&>(data).foreachVoxel(copyOp); //yea ... go, tell your mom

	glTexImage3D(GL_TEXTURE_3D,0,GL_LUMINANCE12_ALPHA4,
			   size[isis::data::rowDim],size[isis::data::columnDim],size[isis::data::sliceDim],0,
			   GL_LUMINANCE_ALPHA,GL_UNSIGNED_SHORT,&chunk.voxel<GLushort>(0,0));

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
		if(MBSize>1){ SGLprintState("%G MB Bilddaten gelesen, %G MB Texturspeicher für eine %s-Textur belegt",data.getVolume(),MBSize,chunk.getSizeAsString().c_str());}
		loaded=false;
	}
//	Info.calcGaps(1,xsize-Info.X.getCnt('X')-1,1,ysize-Info.Y.getCnt('Y')-1,1,zsize-Info.Z.getCnt('Z')-1);
	return true;
}

SGLVektor GLvlVolumeTex::masteroffset(0,0,0);
