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
	weich=repeat=MipMap=false;
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
void GLvlVolumeTex::calcMatr(const isis::util::PropertyMap &prop)
{

//		+SGLVektor(Info.X.getElsize('X')/2,Info.Y.getElsize('Y')/2,Info.Z.getElsize('Z')/2)//@todo warum muss das um nen halben Pixel verschoben werden
		//eig mÃ¼sste GL_NEAREST doch von -.5 bis .5 in der Farbe des Eintrages zeichnen, und nicht 0 bis 1
//		-offset-GLvlVolumeTex::masteroffset;
}

bool GLvlVolumeTex::load(const isis::data::Image &data)
{
	class copyToChunk:public isis::data::Chunk::VoxelOp<GLubyte>{
	public:
		isis::data::MemChunk<GLubyte> &target;
		copyToChunk(isis::data::MemChunk<GLubyte> &ch):target(ch){};
		bool operator()( GLubyte &vox, const isis::util::FixedVector<size_t, 4> &pos ){
			GLubyte *vp=&target.voxel<GLubyte>((pos[0]+1)*2,pos[1]+1,pos[2]+1);
			*(vp++) = vox;
			*vp =  std::numeric_limits<GLubyte>::max();
		}
	};
	if(!sglChkExt("GL_ARB_texture_non_power_of_two","NPOT-textures are not supportet. Aborting...",0))
		exit(-1);

	//Größe der Textur ("+2" ist für den Rand)
	const isis::util::FixedVector<size_t,4> size=data.getSizeAsVector()+2;
	isis::data::MemChunk<GLubyte> chunk(size[isis::data::rowDim]*2,size[isis::data::columnDim],size[isis::data::sliceDim]);

	copyToChunk copyOp(chunk);
	const_cast<isis::data::Image&>(data).foreachVoxel(copyOp); //yea ... go, tell your mom

	glBindTexture(GL_TEXTURE_3D,ID);
	glTexImage3D(GL_TEXTURE_3D,0,GL_LUMINANCE8_ALPHA8,
			   size[isis::data::rowDim],size[isis::data::columnDim],size[isis::data::sliceDim],0,
			   GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE,&chunk.voxel<GLubyte>(0,0));

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
		if(MBSize>1){ SGLprintState("%G MB Texturspeicher für ein %s-Bild belegt",MBSize,data.getSizeAsString().c_str());}
		loaded=false;
	}

	const isis::util::fvector4 offset=data.getPropertyAs<isis::util::fvector4>("indexOrigin");
	const isis::util::fvector4 row=data.getPropertyAs<isis::util::fvector4>("rowVec");
	const isis::util::fvector4 col=data.getPropertyAs<isis::util::fvector4>("columnVec");
	const isis::util::fvector4 slice=data.getPropertyAs<isis::util::fvector4>("sliceVec");

	//fill the orientation matrix
	memset(mm2tex_Matrix,0,sizeof(GLdouble)*4*4);
	((GLdouble*)mm2tex_Matrix)[15]=1;

	for(int i=0;i<3;i++){
		mm2tex_Matrix[i][0]=row[i];
		mm2tex_Matrix[i][1]=col[i];
		mm2tex_Matrix[i][2]=slice[i];
	}

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glScalef(1/rowDim.outer_mm_size(),1/colDim.outer_mm_size(),1/sliceDim.outer_mm_size()); //norm the image to uvw-mapping (0..1)
	glTranslatef(rowDim.startgap_mm,colDim.startgap_mm,sliceDim.startgap_mm); //apply offset from the transparent border
	glMultMatrixd(&mm2tex_Matrix[0][0]); //apply the orientation
	glTranslatef(-offset[0],-offset[1],-offset[2]); // move to the index origin
	saveMatrix(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	return true;
}

SGLVektor GLvlVolumeTex::masteroffset(0,0,0);
