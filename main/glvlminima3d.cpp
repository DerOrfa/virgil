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
#include "glvlminima3d.h"
#include "glvlvolumetex.h"

GLvlMinima3D::GLvlMinima3D(unsigned int pos):
GLvlMinimaBase(pos){}

/*!
    \fn GLvlMinima3D::generate()
 */
void GLvlMinima3D::generate()
{
	vincent::lab_value ID=(*plist)[start].wert;
//	cout << tex->texIndex2texKoord((*plist)[start].pos) << endl;
/// @todo wiso is X hier und .x() unten um 20 verschieden ?
	
	SGLprintInfo("Minima 0x%x wird generiert\n",ID);//@todo wiso is X hier und .x() unten um 20 verschieden ?
	
	glColor3f(1,1,1);
	
	glTranslatef(GLvlVolumeTex::masteroffset.SGLV_X,GLvlVolumeTex::masteroffset.SGLV_Y,GLvlVolumeTex::masteroffset.SGLV_Z);
	glScalef(scale.SGLV_X,scale.SGLV_Y,scale.SGLV_Z);
	glDisable(GL_NORMALIZE);
	short old_pos[]={0,0,0};
	glPushMatrix();
	glTranslatef(-.5,-.5,-.5);
	for(unsigned int i=start;i<end;i++)
	{
		const vincent::iPunkt<vincent::lab_value> p=(*plist)[i];
		if(bottomCap > (*org)[p])continue;
		if(topCap < (*org)[p])break;
		vincent::iPunkt<vincent::lab_value> nachb[6];
		unsigned short mask=0x0;
		p.getNachbStruct(nachb,*img);
		const GLfloat x=p.x(img->xsize),y=p.y(img->xsize,img->ysize),z=p.z(img->xsize,img->ysize);
		for(unsigned short i=0;i<6;i++)
		{
			if(	nachb[i].invalid() || //Kein Nachbar
				!(p.wert==nachb[i].wert || (nachb[i].wert==vincent::WSHED_WSHED && incl_wshed)) || //Nachbar gehˆrt nich zum Segment
				((*org)[nachb[i]]< bottomCap || (*org)[nachb[i]]> topCap) //Nachbar liegt auﬂerhalb [bottomCap - topCap]
			)
			mask|=1<<i;
		}
		if(mask)
		{
			glTranslatef(x-old_pos[0],y-old_pos[1],z-old_pos[2]);
			old_pos[0]=x;old_pos[1]=y;old_pos[2]=z;
			glCallList(caps+mask);
		}
	}
	glPopMatrix();
	glEnable(GL_NORMALIZE);
}

GLuint GLvlMinima3D::caps=0;

SGLVektor GLvlMinima3D::scale;


void GLvlMinima3D::setup(
	SGLVektor scale,
	const vincent::transform &transform,
	VImage &src
)
{	
	GLvlMinimaBase::setup(transform,transform.last_erg,src);

	const GLshort vertexes[8][3] = {{0,1,1}, {1,1,1}, {1,1,0}, {0,1,0}, {0,0,1}, {1,0,1}, {1,0,0}, {0,0,0}};
	const GLfloat normales[8][3] = {{-0.57735,0.57735,0.57735}, {0.57735,0.57735,0.57735}, {0.57735,0.57735,-0.57735}, {-0.57735,0.57735,-0.57735}, {-0.57735,-0.57735,0.57735}, {0.57735,-0.57735,0.57735}, {0.57735,-0.57735,-0.57735}, {-0.57735,-0.57735,-0.57735}};
	enum bit_dir{bnord=1<<vincent::nord,bsued=1<<vincent::sued,bost=1<<vincent::ost,bwest=1<<vincent::west,bueber=1<<vincent::ueber,bunter=1<<vincent::unter};
	
	glEnableClientState(GL_VERTEX_ARRAY);/// @todo kˆnnen die nachher wieder aus ?  warscheinlich - sind eh clientseitig
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
		/*000000*/	{}, //nix
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
		/*010000*/	{quad,4,0,4,5,1}, //¸ber 
		/*010001*/	{strip,6,2,3,1,0,5,4}, //¸ber & nord
		/*010010*/	{strip,6,7,6,4,5,0,1}, //¸ber & sued
		/*010011*/	{strip,8,2,3,1,0,5,4,6,7}, //¸ber & sued & nord
		/*010100*/	{strip,6,6,2,5,1,4,0}, //¸ber & ost
		/*010101*/	{strip,6,6,2,5,1,4,0,quad,4,0,1,2,3}, //¸ber & ost & nord
		/*010110*/	{strip,6,6,2,5,1,4,0,quad,4,7,6,5,4}, //¸ber & ost & sued
		/*010111*/	{strip,8,3,0,2,1,6,5,7,4,quad,4,0,4,5,1}, //¸ber & ost & sued & nord
		/*011000*/	{strip,6,3,7,0,4,1,5}, //¸ber & west
		/*011001*/	{strip,6,3,7,0,4,1,5,quad,4,0,1,2,3}, //¸ber & west & nord
		/*011010*/	{strip,6,3,7,0,4,1,5,quad,4,7,6,5,4}, //¸ber & west & sued
		/*011011*/	{strip,8,1,2,0,3,4,7,5,6,quad,4,0,4,5,1}, //¸ber & west & sued & nord
		/*011100*/	{strip,8,6,2,5,1,4,0,7,3}, //¸ber & west & ost
		/*011101*/	{strip,8,6,2,5,1,4,0,7,3,quad,4,0,1,2,3}, //¸ber & west & ost & nord
		/*011110*/	{strip,8,6,2,5,1,4,0,7,3,quad,4,7,6,5,4}, //¸ber & west & ost & sued
		/*011111*/	{strip,8,6,2,5,1,4,0,7,3,quad,8,0,1,2,3,7,6,5,4}, //¸ber & west & ost & sued & nord
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

	GLvlMinima3D::scale=scale;
}

SGLVektor GLvlMinima3D::getCenter(){
    /// @todo implement me
}

shared_ptr<Bild_mem<VBit> > GLvlMinima3D::genTex()
{
	Bild_mem<VBit> *ret = new Bild_mem<VBit>(maxEdge.x-minEdge.x+1,maxEdge.y-minEdge.y+1,maxEdge.z-minEdge.z+1,0);
	for(unsigned int i=GLvlMinimaBase::start;i<GLvlMinimaBase::end;i++)
	{
		const vincent::iPunkt<vincent::lab_value> p=(*GLvlMinimaBase::plist)[i];
		if(bottomCap > (*org)[p])continue;
		if(topCap < (*org)[p])break;
		const unsigned short x=p.x(img->xsize)-minEdge.x;
		const unsigned short y=p.y(img->xsize,img->ysize)-minEdge.y;
		const unsigned short z=p.z(img->xsize,img->ysize)-minEdge.z;
		((Bild<VBit> *)ret)->at(x,y,z)=numeric_limits<VBit>::max();
		vincent::iPunkt<vincent::lab_value>  nachb[6];
		unsigned short nachb_cnt=p.getNachb(nachb,*img);
		for(unsigned short i=0;i<nachb_cnt;i++)
			if(nachb[i].wert==vincent::WSHED_WSHED && incl_wshed)
				((Bild<VBit> *)ret)->at(x,y,z)=numeric_limits<VBit>::max();
	}
	ret->xsize.setElsize(img->xsize.getElsize('X'));
	ret->ysize.setElsize(img->ysize.getElsize('Y'));
	ret->zsize.setElsize(img->zsize.getElsize('Z'));
	return shared_ptr<Bild_mem<VBit> >(ret);
}

void GLvlMinima3D::writeTex(const unsigned short offset[3],Bild<GLubyte> &textur)const
{
	for(unsigned int i=GLvlMinimaBase::start;i<GLvlMinimaBase::end;i++)
	{
		const vincent::iPunkt<vincent::lab_value> p=(*GLvlMinimaBase::plist)[i];
		if(bottomCap > (*org)[p])continue;
		if(topCap < (*org)[p])break;
		const unsigned short x=p.x(img->xsize)-minEdge.x + offset[0];
		const unsigned short y=p.y(img->xsize,img->ysize)-minEdge.y + offset[1];
		const unsigned short z=p.z(img->xsize,img->ysize)-minEdge.z + offset[2];
		textur.at(x,y,z)=numeric_limits<GLubyte>::max();
		vincent::iPunkt<vincent::lab_value>  nachb[6];
		unsigned short nachb_cnt=p.getNachb(nachb,*img);
		for(unsigned short i=0;i<nachb_cnt;i++)
			if(nachb[i].wert==vincent::WSHED_WSHED && incl_wshed)
				textur.at(x,y,z)=numeric_limits<GLubyte>::max();
	}
	textur.xsize.setElsize(img->xsize.getElsize('X'));
	textur.ysize.setElsize(img->ysize.getElsize('Y'));
	textur.zsize.setElsize(img->zsize.getElsize('Z'));
}

GLvlMinima3DList::GLvlMinima3DList(GLvlMinima3D &img)
{
	push_back(img);
}

void GLvlMinima3DList::getOffset(unsigned short offset[3],GLvlMinima3DList::iterator i)
{
	assert(i->minEdge.x-minEdge.x >= 0);
	assert(i->minEdge.y-minEdge.y >= 0);
	assert(i->minEdge.z-minEdge.z >= 0);
	
	offset[0]+= (i->minEdge.x-minEdge.x);
	offset[1]+= (i->minEdge.y-minEdge.y);
	offset[2]+= (i->minEdge.z-minEdge.z);
}

void GLvlMinima3DList::getDim(dim &X,dim &Y, dim &Z)
{
	X.setElsize(GLvlMinimaBase::img->xsize.getElsize('X'));
	Y.setElsize(GLvlMinimaBase::img->ysize.getElsize('Y'));
	Z.setElsize(GLvlMinimaBase::img->zsize.getElsize('Z'));
	
	minEdge.x=minEdge.y=minEdge.z=numeric_limits<unsigned short>::max();
	maxEdge.x=maxEdge.y=maxEdge.z=numeric_limits<unsigned short>::min();

	for(GLvlMinima3DList::iterator i=begin();i!=end();i++)
	{
		minEdge.x = minEdge.x <? i->minEdge.x;
		minEdge.y = minEdge.y <? i->minEdge.y;
		minEdge.z = minEdge.z <? i->minEdge.z;
	
		maxEdge.x = maxEdge.x >? i->maxEdge.x;
		maxEdge.y = maxEdge.y >? i->maxEdge.y;
		maxEdge.z = maxEdge.z >? i->maxEdge.z;
	}
	X.setCnt(maxEdge.x-minEdge.x+1);
	Y.setCnt(maxEdge.y-minEdge.y+1);
	Z.setCnt(maxEdge.z-minEdge.z+1);
}
