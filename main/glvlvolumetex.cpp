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

	//Ungültige Koord werden numeric_limits<unsigned short>::max(), was ja auch gößer als Info.X.getCnt('X') ist
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

bool GLvlVolumeTex::load(const isis::data::Image &data)
{
	if(!sglChkExt("GL_ARB_texture_non_power_of_two","NPOT-textures are not supportet. Aborting...",0))
		exit(-1);

	const isis::util::FixedVector<size_t,4> size=data.getSizeAsVector();
	const isis::data::ValueArray<GLubyte> buff=data.copyAsValueArray<GLubyte>();


	glBindTexture(GL_TEXTURE_3D,ID);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage3D(GL_TEXTURE_3D,0,GL_LUMINANCE12_ALPHA4,
			   size[isis::data::rowDim],size[isis::data::columnDim],size[isis::data::sliceDim],0,
			   GL_LUMINANCE,GL_UNSIGNED_BYTE,buff.begin().operator->());

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

	const isis::util::fvector3 offset=data.getPropertyAs<isis::util::fvector3>("indexOrigin");
	const isis::util::fvector3 row=data.getPropertyAs<isis::util::fvector3>("rowVec");
	const isis::util::fvector3 col=data.getPropertyAs<isis::util::fvector3>("columnVec");
	const isis::util::fvector3 slice=data.getPropertyAs<isis::util::fvector3>("sliceVec");
	const isis::util::fvector3 scale=isis::util::fvector3(1,1,1)/data.getFoV();// @todo its 0/0 = NaN but we dont use it, so we dont care ... or do we ?

	//fill the orientation matrix
	GLdouble mat[4][4];
	memset(mat,0,sizeof(GLdouble)*4*4);
	mat[3][3]=1;

	for(int i=0;i<3;i++){
		mat[i][0]=row[i];
		mat[i][1]=col[i];
		mat[i][2]=slice[i];
	}

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glScalef(scale[0],scale[1],scale[2]); //norm the image to uvw-mapping (0..1)
	glMultMatrixd(&mat[0][0]); //apply the orientation
	glTranslatef(-offset[0],-offset[1],-offset[2]); // move to the index origin
	saveMatrix(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	return true;
}

