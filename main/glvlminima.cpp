//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "glvlminima.h"
#include <sglvektor.h>

GLvlMinima::GLvlMinima(unsigned int pos):start(pos)
{
	//@todo ende bestimmen
	//@todo auf setup prüfen
}

/*!
    \fn GLvlMinima::generate()
 */
void GLvlMinima::generate()
{
	vincent::lab_value ID=(*plist)[start].wert;
	cout << tex->texIndex2texKoord((*plist)[start].pos) << endl;
	glColor3f(1,1,1);
	glDisable(GL_NORMALIZE);
	for(unsigned int i=start;i<end;i++)
	{
		const vincent::iPunkt<vincent::lab_value> p=(*plist)[i];
		vincent::iPunkt<vincent::lab_value> nachb[6];
		unsigned short mask=0x0;
		p.getNachbStruct(nachb,*img);
		for(unsigned short i=0;i<6;i++)
		{
			if(nachb[i].invalid() || p.wert!=nachb[i].wert)
				mask|=1<<i;
		}
		if(!mask)continue;
		SGLVektor Voxel=tex->texIndex2texKoord(plist->operator[](end).pos);
		glPushMatrix();
		glTranslatef(Voxel.SGLV_X-.5,Voxel.SGLV_Y-.5,Voxel.SGLV_Z-.5);
		glCallList(caps+mask);
		glPopMatrix();
	}
/*	for(end=start;p.wert==ID;p=plist->operator[](++end))
	{
		vincent::iPunkt<vincent::lab_value> nachb[6];
		p.getNachbStruct(nachb,*img);
		SGLVektor Voxel=tex->texIndex2texKoord(plist->operator[](end).pos);
		glPushMatrix();
		glTranslatef(Voxel.SGLV_X-.5,Voxel.SGLV_Y-.5,Voxel.SGLV_Z-.5);
		for(unsigned short i=0;i<6;i++)
		{
			if(nachb[i].invalid() || p.wert!=nachb[i].wert)
				makePoly(i);
		}
		glPopMatrix();
	}*/
	glEnable(GL_NORMALIZE);
}


/*!
    \fn GLvlMinima::getCenter()
 */
SGLVektor GLvlMinima::getCenter()
{
    /// @todo implement me
}

GLuint GLvlMinima::caps=0;

shared_ptr<GLvlVolumeTex> GLvlMinima::tex;
shared_ptr<vincent::Bild_vimage<vincent::lab_value> > GLvlMinima::img;
shared_ptr<vincent::PunktList<vincent::lab_value> > GLvlMinima::plist;
/*!
    \fn GLvlMinima::setup_norm(SGLVektor norm)
 */

const GLshort GLvlMinima::diff[6][5][3]={
{{0,1,1},{1,1,1},{1,1,0},{0,1,0}, { 0, 1, 0}},//Nord
{{0,0,0},{1,0,0},{1,0,1},{0,0,1}, { 0,-1, 0}},//Sued
{{1,0,0},{1,1,0},{1,1,1},{1,0,1}, { 1, 0, 0}},//Ost
{{0,0,1},{0,1,1},{0,1,0},{0,0,0}, {-1, 0, 0}},//West
{{0,0,1},{1,0,1},{1,1,1},{0,1,1}, { 0, 0, 1}},//Ueber
{{0,1,0},{1,1,0},{1,0,0},{0,0,0}, { 0, 0,-1}} //Unter
};

void GLvlMinima::setup(
	SGLVektor norm,
	boost::shared_ptr<GLvlVolumeTex> tex,
	boost::shared_ptr< vincent::Bild_vimage<vincent::lab_value>  > img
)
{	
	if(caps)glDeleteLists(caps,6);
	caps=glGenLists(6);
	for(int d=0;d<6;d++)
	{
		assert(glIsList(caps+d));
		glNewList(caps+d,GL_COMPILE);glBegin(GL_QUADS);
		{
			glNormal3sv(diff[d][4]);
			for(int i=0;i<4;i++)
				glVertex3sv(diff[d][i]);
		}
		glEnd();glEndList();

	}
	assert(glGetError()==GL_NO_ERROR);
	
	GLvlMinima::tex=tex;
	GLvlMinima::img=img;
	GLvlMinima::plist=vincent::transform::getVoxels(*img);
	
	for(int pos=0;pos<img->size()-1;pos++)
		assert(plist->operator[](pos).wert<=plist->operator[](pos+1).wert);
}


void GLvlMinima::setup2(
	SGLVektor norm,
	boost::shared_ptr<GLvlVolumeTex> tex,
	boost::shared_ptr< vincent::Bild_vimage<vincent::lab_value>  > img
)
{	
	const GLshort vertexes[8][3] = {{0,1,1}, {1,1,1}, {1,1,0}, {0,1,0}, {0,0,1}, {1,0,1}, {1,0,0}, {0,0,0}};
	const GLfloat normales[8][3] = {{-0.57735,0.57735,0.57735}, {0.57735,0.57735,0.57735}, {0.57735,0.57735,-0.57735}, {-0.57735,0.57735,-0.57735}, {-0.57735,-0.57735,0.57735}, {0.57735,-0.57735,0.57735}, {0.57735,-0.57735,-0.57735}, {-0.57735,-0.57735,-0.57735}};
	enum bit_dir{bnord=1<<vincent::nord,bsued=1<<vincent::sued,bost=1<<vincent::ost,bwest=1<<vincent::west,bueber=1<<vincent::ueber,bunter=1<<vincent::unter};
	
	glEnableClientState(GL_VERTEX_ARRAY);//@todo können die nachher wieder aus ?  warscheinlich - sind eh clientseitig
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_SHORT, 0, vertexes);
	glNormalPointer(GL_FLOAT, 0,normales);


	if(caps)glDeleteLists(caps,1<<6);
	caps=glGenLists(1<<6);
	
	
	enum mode{strip=0xFE,quad};
	
	const GLushort indexes[64][2*(1+1+10)]=
	{
		//Unter
		//|Ueber
		//||West
		//|||Ost
		//||||Sued
		//|||||Nord
		/*000000*/	{}, //nord 
		/*000001*/	{quad,4,0,1,2,3}, //nord 
		/*000010*/	{quad,4,7,6,5,4}, //sued
		/*000011*/	{quad,8,0,1,2,3,7,6,5,4}, //nord & sued
		/*000100*/	{quad,4,6,2,1,5}, //ost 
		/*000101*/	{strip,6,3,0,2,1,6,5}, //ost & nord
		/*000110*/	{strip,6,4,7,5,6,1,2}, //ost & sued
		/*000111*/	{strip,8,3,0,2,1,6,5,7,4}, //ost & sued &nord
		/*001000*/	{quad,4,0,3,7,4}, //west 
		/*001001*/	{strip,6,1,2,0,3,4,7}, //west & nord
		/*001010*/	{strip,6,6,5,7,4,3,0}, //west & sued
		/*001011*/	{strip,8,1,2,0,3,4,7,5,6}, //west & sued & nord
		/*001100*/	{quad,8,0,3,7,4,6,2,1,5}, //west & ost
		/*001101*/	{strip,8,7,4,3,0,2,1,6,5}, //west & ost & nord
		/*001110*/	{strip,8,0,3,4,7,5,6,1,2}, //west & ost & sued
		/*001111*/	{strip,10,0,3,4,7,5,6,1,2,0,3}, //west & ost & sued & nord
		/*010000*/	{quad,4,0,4,5,1}, //über 
		/*010001*/	{strip,6,2,3,1,0,5,4}, //über & nord
		/*010010*/	{strip,6,7,6,4,5,0,1}, //über & sued
		/*010011*/	{strip,8,2,3,1,0,5,4,6,7}, //über & sued & nord
		/*010100*/	{strip,6,6,2,5,1,4,0}, //über & ost
		/*010101*/	{strip,6,6,2,5,1,4,0,quad,4,0,1,2,3}, //über & ost & nord
		/*010110*/	{strip,6,6,2,5,1,4,0,quad,4,7,6,5,4}, //über & ost & sued
		/*010111*/	{strip,8,3,0,2,1,6,5,7,4,quad,4,0,4,5,1}, //über & ost & sued & nord
		/*011000*/	{strip,6,3,7,0,4,1,5}, //über & west
		/*011001*/	{strip,6,3,7,0,4,1,5,quad,4,0,1,2,3}, //über & west & nord
		/*011010*/	{strip,6,3,7,0,4,1,5,quad,4,7,6,5,4}, //über & west & sued
		/*011011*/	{strip,8,1,2,0,3,4,7,5,6,quad,4,0,4,5,1}, //über & west & sued & nord
		/*011100*/	{strip,8,6,2,5,1,4,0,7,3}, //über & west & ost
		/*011101*/	{strip,8,6,2,5,1,4,0,7,3,quad,4,0,1,2,3}, //über & west & ost & nord
		/*011110*/	{strip,8,6,2,5,1,4,0,7,3,quad,4,7,6,5,4}, //über & west & ost & sued
		/*011111*/	{strip,6,6,2,5,1,4,0,quad,8,0,1,2,3,7,6,5,4}, //über & west & ost & sued & nord
		/*100000*/	{quad,4,3,2,6,7}, // unter 
		/*100001*/	{strip,6,0,1,3,2,7,6}, // unter & nord 
		/*100010*/	{strip,6,5,4,6,7,2,3}, // unter & sued
		/*100011*/	{strip,8,0,1,3,2,7,6,4,5}, // unter & sued & nord
		/*100100*/	{strip,6,1,5,2,6,3,7}, // unter & ost
		/*100101*/	{strip,6,1,5,2,6,3,7,quad,4,0,1,2,3}, // unter & ost & nord
		/*100110*/	{strip,6,1,5,2,6,3,7,quad,4,7,6,5,4}, // unter & ost & sued
		/*100111*/	{strip,6,1,5,2,6,3,7,quad,8,0,1,2,3,7,6,5,4}, // unter & ost & sued
		/*101000*/	{strip,6,4,0,7,3,6,2}, // unter & west
		/*101001*/	{strip,6,4,0,7,3,6,2,quad,4,0,1,2,3}, // unter & west & nord
		/*101010*/	{strip,6,4,0,7,3,6,2,quad,4,7,6,5,4}, // unter & west & sued
		/*101011*/	{strip,6,4,0,7,3,6,2,quad,8,0,1,2,3,7,6,5,4}, // unter & west & sued & nord
		/*101100*/	{strip,8,4,0,7,3,6,2,5,1}, // unter & west & ost 
		/*101101*/	{strip,8,4,0,7,3,6,2,5,1,quad,4,0,1,2,3}, // unter & west & ost & nord
		/*101110*/	{strip,8,4,0,7,3,6,2,5,1,quad,4,7,6,5,4}, // unter & west & ost & sued
		/*101111*/	{strip,8,4,0,7,3,6,2,5,1,quad,8,0,1,2,3,7,6,5,4}, // unter & west & ost & nord & sued
		/*110000*/	{quad,8,0,4,5,1,6,7,3,2}, // unter & ueber
		/*110001*/	{strip,8,4,5,0,1,3,2,7,6}, // unter & ueber & nord
		/*110010*/	{strip,8,1,0,5,4,6,7,2,3}, // unter & ueber & sued
		/*110011*/	{strip,10,1,0,5,4,6,7,2,3,1,0}, // unter & ueber & sued & nord
		/*110100*/	{strip,8,0,4,1,5,2,6,3,7}, // unter & ueber & ost
		/*110101*/	{strip,8,0,4,1,5,2,6,3,7,quad,4,0,1,2,3}, // unter & ueber & ost & nord
		/*110110*/	{strip,8,0,4,1,5,2,6,3,7,quad,4,7,6,5,4}, // unter & ueber & ost & sued
		/*110111*/	{strip,8,0,4,1,5,2,6,3,7,quad,8,0,1,2,3,7,6,5,4}, // unter & ueber & ost & sued
		/*111000*/	{strip,8,5,1,4,0,7,3,6,2}, // unter & ueber & west
		/*111001*/	{strip,8,5,1,4,0,7,3,6,2,quad,4,0,1,2,3}, // unter & ueber & west & nord
		/*111010*/	{strip,8,5,1,4,0,7,3,6,2,quad,4,7,6,5,4}, // unter & ueber & west & sued
		/*111011*/	{strip,8,5,1,4,0,7,3,6,2,quad,8,0,1,2,3,7,6,5,4}, // unter & ueber & west & ueber & nord & sued
		/*111100*/	{strip,10,0,4,1,5,2,6,3,7,0,4}, // unter & ueber & west & ost
		/*111101*/	{strip,10,0,4,1,5,2,6,3,7,0,4,quad,4,0,1,2,3}, // unter & ueber & west & ost & nord
		/*111110*/	{strip,10,0,4,1,5,2,6,3,7,0,4,quad,4,7,6,5,4}, // unter & ueber & west & ost & sued
		/*111111*/	{strip,8,0,4,1,5,2,6,3,7,strip,8,6,5,7,4,3,0,2,1}, // unter & ueber & west & ost & sued
		//|||||Nord
		//||||Sued
		//|||Ost
		//||West
		//|Ueber
		//Unter
	};
	
	//Streifen
	for(GLubyte id=1;id<64;id++)
	{
		GLubyte token=0;
		QuadBegin(caps+id);
		do{
			const GLuint mode= indexes[id][token++] == strip ? GL_QUAD_STRIP: GL_QUADS; 
			const GLubyte cnt=indexes[id][token++];
			glDrawElements(mode,cnt,GL_UNSIGNED_SHORT,&indexes[id][token]);
			token+=cnt;
		}while(indexes[id][token]);
		QuadEnd();
	}


	GLvlMinima::tex=tex;
	GLvlMinima::img=img;
	GLvlMinima::plist=vincent::transform::getVoxels(*img);
}

