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

GLvlMinima::GLvlMinima(unsigned int pos):start(pos),end(pos)
{
}

/*!
    \fn GLvlMinima::generate()
 */
void GLvlMinima::generate()
{
	vincent::lab_value ID=plist->operator[](start).wert;
	cout << tex->texIndex2texKoord(plist->operator[](start).pos) << endl;
	vincent::iPunkt<vincent::lab_value> p=plist->operator[](start);
	glColor3f(1,1,1);
	for(end=start;p.wert==ID;p=plist->operator[](++end))
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
	}
}


/*!
    \fn GLvlMinima::makePoly()
 */
void GLvlMinima::makePoly(unsigned short dir)
{
		
		glCallList(caps+dir);
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
	const GLbyte vertexes[8][3] = {{0,1,1}, {1,1,1}, {1,1,0}, {0,1,0}, {0,0,1}, {1,0,1}, {1,0,0}, {0,0,0}};
	const GLbyte normales[8][3] = {{-1,1,1}, {1,1,1}, {1,1,-1}, {-1,1,-1}, {-1,-1,1}, {1,-1,1}, {1,-1,-1}, {-1,-1,-1}};
	enum bit_dir{bnord=1<<vincent::nord,bsued=1<<vincent::sued,bost=1<<vincent::ost,bwest=1<<vincent::west,bueber=1<<vincent::ueber,bunter=1<<vincent::unter};
	
	glEnableClientState(GL_VERTEX_ARRAY);//@todo können die nachher wieder aus ?  warscheinlich - sind eh clientseitig
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_SHORT, 0, vertexes);
	glNormalPointer(GL_BYTE, 0,normales);


	if(caps)glDeleteLists(caps,1<<6);
	caps=glGenLists(1<<6);
	
	//Einzelne Fl.
	const GLubyte single_indexes[6][4]=
	{
		{0,1,2,3}, //nord (000001)
		{7,6,4,5}, //sued (000010)
		{6,2,1,5}, //ost (000100)
		{0,3,7,4}, //west (001000)
		{0,4,5,1}, //über (010000)
		{3,7,6,2}, //unter (100000)
	};
	
	//streifen 
	const GLubyte double_indexes[12][10]=
	{
		{3,0,2,1,6,5,	7,4,	3,0}, //nord & ost   (000101)
		{1,2,0,3,4,7,	5,6,	1,2}, //nord & west  (001001)
		{2,3,1,0,5,4,	6,7,	2,3}, //nord & über  (010001)
		{0,1,3,2,7,6,	4,5,	0,1}, //nord & unter (100001)

		{4,7,5,6,1,2,	0,3,	4,7}, //sued & ost (000110)
		{6,5,7,4,3,0,	2,1,	6,5}, //sued & west (001010)
		{7,6,4,5,0,1,	3,2,	7,6}, //sued & über (010010)
		{5,4,6,7,2,3,	1,0,	5,4}, //sued & unter (100010)
		
		{6,2,5,1,4,0,	7,3,	6,2}, //ost & über  (010100)
		{1,5,2,6,3,7,	0,4,	1,5}, //ost & unter (100100)
		
		{3,7,0,4,1,5,	2,6,	3,7}, //west & über  (011000)
		{4,0,7,3,6,2,	5,2,	4,0} //west & unter (101000)
	};
	
	const GLuint ref[12][3]=
	{
		//[index in double_indexes|Bitschlüssel für Streifen aus 2|3|und 4 el] (0 => redundant -> nich nochmal machen)
		{0, bnord|bost,		bnord|bost|bsued,	bnord|bost|bsued|bwest},
		{1, bnord|bwest,	bnord|bwest|bsued,	0},
		{2, bnord|bueber,	bnord|bueber|bsued,	bnord|bueber|bsued|bunter},
		{3, bnord|bunter,	bnord|bunter|bsued,	0},
		
		{4, bsued|bost,		0,					0}
		{5, bsued|bwest,	0					0},
		{6, bsued|bueber,	0					0},
		{7, bsued|bunter,	0					0},
		
		{8, bost|bueber,	bost|bueber|bwest,	bost|bueber|bwest|bunter},
		{9, bost|bunter,	bost|bunter|bwest,	0},
		
		{10,bwest|bueber,	0,					0},
		{10,bwest|bunter,	0,					0}
	};
	
	//einzelne Flächen
	for(int d=0;d<6;d++){
		QuadBegin(caps+(1<<d));
		glDrawElements(GL_QUADS, 4,GL_UNSIGNED_BYTE,single_indexes[d]);
		QuadEnd();
	}//noch 60

	//doppelte einzeln
	for(int d=0;d<6;d+=2){
		QuadBegin(caps+(1<<d | 1<<(d+1)));
		glDrawElements(GL_QUADS, 8,GL_UNSIGNED_BYTE,single_indexes[d]);
		QuadEnd();
	}//noch 57

	//Streifen
	for(GLubyte =id=0;id<12;id++){
		GLuint *key=ref[id];
		if(key[1]){
		QuadBegin(key[1]);
			glDrawElements(GL_QUAD_STRIP, 6,GL_UNSIGNED_BYTE,double_indexes[key[0]]);
		QuadEnd();}
		if(key[2]){
		QuadBegin(key[2]);
			glDrawElements(GL_QUAD_STRIP, 8,GL_UNSIGNED_BYTE,double_indexes[key[0]]);
		QuadEnd();}
		if(key[3]){
		QuadBegin(key[3]);
			glDrawElements(GL_QUAD_STRIP, 10,GL_UNSIGNED_BYTE,double_indexes[key[0]]);
		QuadEnd();}
	}//noch 33


	GLvlMinima::tex=tex;
	GLvlMinima::img=img;
	GLvlMinima::plist=vincent::transform::getVoxels(*img);
}

