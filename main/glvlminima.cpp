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
#include <libsgl/sglvektor.h>
#include "glvlvolumetex.h"
#include "glvlsegment.h"
#include <libsgl/sglmisc.h>

GLvlMinima::GLvlMinima(const GLvlMinima&)
{
	SGLprintError("Minima dürfen nicht kopiert werden");
}

GLvlMinima::GLvlMinima(unsigned int pos):start(pos),incl_wshed(true)
{

	minEdge.x=minEdge.y=minEdge.z=numeric_limits<unsigned short>::max();
	maxEdge.x=maxEdge.y=maxEdge.z=numeric_limits<unsigned short>::min();
	if(img)
	{
		end=start;
		vincent::iPunkt<vincent::lab_value> p=(*plist)[start];
		vincent::lab_value ID=p.wert;
		bottomCap = (*org)[p];
		do 
		{
			writeEdgeData(p,minEdge,maxEdge);
			vincent::iPunkt<vincent::lab_value>  nachb[6];
			const unsigned short nachb_cnt=p.getNachb(nachb,*img);
			for(unsigned short i=0;i<nachb_cnt;i++)
				if(nachb[i].wert==vincent::WSHED_WSHED && incl_wshed)
					writeEdgeData(nachb[i],minEdge,maxEdge);

			p=(*plist)[++end]; 
		}
		while(end<plist->size && p.wert==ID);
			
		topCap = (*org)[(*plist)[end-1]];
		//getPixel verwendet stable_sort -> die Punkte eines Objektes sind noch der Größe nach geordnet
	}
	else{SGLprintError("GLvlMinima::setup wurde nicht ausgeführt, das Objekt kann nicht angelegt werden");abort();}
	assert(topCap == topBorder());
	assert(bottomBorder()== bottomCap);
}

bool GLvlMinima::chCapAbsTop(VUByte Top)
{
	if(Top!=topCap)
	{
		if(Top<=bottomBorder())Top=bottomBorder()+1;
		topCap= topBorder() <? Top;
		if(topCap<=bottomCap)
			bottomCap=topCap-1;
		update_users();
		return true;
	}
	else return false;
}
bool GLvlMinima::chCapAbsBottom(VUByte Bottom)
{
	if(Bottom!=bottomCap)
	{
		if(Bottom>=topBorder())Bottom=topBorder()-1;
		bottomCap= bottomBorder() >? Bottom;
		if(bottomCap>=topCap)
			topCap=bottomCap+1;
		update_users();
		return true;
	}
	else return false;
}
bool GLvlMinima::chCapAbs(VUByte Top,VUByte Bottom)
{
	if(Bottom>=Top)Top=Bottom+1;
	bool update=chCapAbsTop(Top);
	if(chCapAbsBottom(Bottom))update=true;
	if(update)update_users();
	return  update;
}

bool GLvlMinima::chCapRel(signed char Top_delta,signed char Bottom_delta)
{
	bool update;
	if(Top_delta==numeric_limits<signed char>::max())//Maximize TopCap
	{
		if(this->topCap!=topBorder())
		{
			this->topCap=topBorder();
			incl_wshed=true;
			update=true;
		}
	}
	else if(Top_delta>0)//incr TopCap
	{
		if(this->topCap >=topBorder())
		{
			if(!incl_wshed){
				incl_wshed=true;
				update =true;
			}
		}
		else {this->topCap++;update =true;}
	}
	else if(Top_delta<0)//decr TopCap
	{
		if(incl_wshed){
			incl_wshed=false;
			update =true;
		}
		else if(this->topCap>this->bottomCap){this->topCap--;update =true;}
	}
	
	if(Bottom_delta==numeric_limits<short>::min())//Minimize BottomCap
	{
		if(this->bottomCap!=bottomBorder())
		{this->bottomCap=bottomBorder();update=true;}
	}
	if(Bottom_delta>0)
	{
		if(this->bottomCap <this->topCap)
		{this->bottomCap++;update =true;}
	}
	else if(Bottom_delta<0)
	{
		if(this->bottomCap > bottomBorder())
		{this->bottomCap--;update =true;}
	}
	if(update)update_users();
	return update;
}

dim GLvlMinima::getXDim()const
{
	dim ret;
	assert(maxEdge.x-minEdge.x+1);
	ret.setCnt(maxEdge.x-minEdge.x+1);
	ret.setElsize(GLvlMinima::img->xsize.getElsize('X'));
	return ret;
}
dim GLvlMinima::getYDim()const
{
	dim ret;
	assert(maxEdge.y-minEdge.y+1);
	ret.setCnt(maxEdge.y-minEdge.y+1);
	ret.setElsize(GLvlMinima::img->ysize.getElsize('Y'));
	return ret;
}
dim GLvlMinima::getZDim()const
{
	dim ret;
	assert(maxEdge.z-minEdge.z+1);
	ret.setCnt(maxEdge.z-minEdge.z+1);
	ret.setElsize(GLvlMinima::img->zsize.getElsize('Z'));
	return ret;
}

void GLvlMinima::generate()
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
			if(nachb[i].wert==vincent::WSHED_WSHED && incl_wshed) //Nachbar is Wasserscheide, und die soll gez werden
			{
				const GLfloat x=nachb[i].x(img->xsize),
					y=nachb[i].y(img->xsize,img->ysize),
					z=nachb[i].z(img->xsize,img->ysize);
				glTranslatef(x-old_pos[0],y-old_pos[1],z-old_pos[2]);
				old_pos[0]=x;old_pos[1]=y;old_pos[2]=z;
				glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glCallList(caps+0x3F);
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			}//Watersheds werden als Drahtgitter gez - daher müssen  die voxel darunter trotzdem gez werden -> kein "else if"
			if(nachb[i].invalid() || //Kein Nachbar
				!(p.wert==nachb[i].wert) || //Nachbar gehört nich zum Segment [nicht(wert_gleich or wert_wshed)]
				((*org)[nachb[i]]< bottomCap || (*org)[nachb[i]]> topCap) //Nachbar liegt außerhalb [bottomCap - topCap]
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

void GLvlMinima::setup(
	SGLVektor scale,
	const vincent::transform &transform,
	VImage &src
)
{	
	GLvlMinima::img=transform.last_erg;
	GLvlMinima::plist=transform.getVoxels(*img);
	org= boost::shared_ptr< vincent::Bild_vimage<VUByte> >(new vincent::Bild_vimage<VUByte>(src));

	const GLshort vertexes[8][3] = {{0,1,1}, {1,1,1}, {1,1,0}, {0,1,0}, {0,0,1}, {1,0,1}, {1,0,0}, {0,0,0}};
	const GLfloat normales[8][3] = {{-0.57735,0.57735,0.57735}, {0.57735,0.57735,0.57735}, {0.57735,0.57735,-0.57735}, {-0.57735,0.57735,-0.57735}, {-0.57735,-0.57735,0.57735}, {0.57735,-0.57735,0.57735}, {0.57735,-0.57735,-0.57735}, {-0.57735,-0.57735,-0.57735}};
	enum bit_dir{bnord=1<<vincent::nord,bsued=1<<vincent::sued,bost=1<<vincent::ost,bwest=1<<vincent::west,bueber=1<<vincent::ueber,bunter=1<<vincent::unter};
	
	glEnableClientState(GL_VERTEX_ARRAY);/// @todo können die nachher wieder aus ?  warscheinlich - sind eh clientseitig
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
		/*011111*/	{strip,8,6,2,5,1,4,0,7,3,quad,8,0,1,2,3,7,6,5,4}, //über & west & ost & sued & nord
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

	GLvlMinima::scale=scale;
}

vincent::lab_value GLvlMinima::getID()const
{
	vincent::lab_value ret=GLvlMinima::plist->operator[](start).wert;
	return ret;
}
void GLvlMinima::getPktKoord(const unsigned int indexRel,unsigned short &x,unsigned short &y,unsigned short &z)const
{
	vincent::iPunkt<vincent::lab_value> p=(*GLvlMinima::plist)[start+indexRel];
	x=p.x(img->xsize);
	y=p.y(img->xsize,img->ysize);
	z=p.z(img->xsize,img->ysize);
}

SGLVektor GLvlMinima::getCenter(){
    /// @todo implement me
}

void GLvlMinima::writeTex(const unsigned short offset[3],Bild<GLubyte> &textur)const
{
	for(unsigned int i=start;i<end;i++)
	{
		const vincent::iPunkt<vincent::lab_value> p=(*GLvlMinima::plist)[i];
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
			{
				const unsigned short x=nachb[i].x(img->xsize)-minEdge.x + offset[0];
				const unsigned short y=nachb[i].y(img->xsize,img->ysize)-minEdge.y + offset[1];
				const unsigned short z=nachb[i].z(img->xsize,img->ysize)-minEdge.z + offset[2];
				textur.at(x,y,z)=numeric_limits<GLubyte>::max()/2;
			}
	}
	textur.xsize.setElsize(img->xsize.getElsize('X'));
	textur.ysize.setElsize(img->ysize.getElsize('Y'));
	textur.zsize.setElsize(img->zsize.getElsize('Z'));
}


GLuint GLvlMinima::caps=0;
SGLVektor GLvlMinima::scale;
boost::shared_ptr<vincent::Bild_vimage<vincent::lab_value> > GLvlMinima::img;
boost::shared_ptr< vincent::Bild_vimage<VUByte> > GLvlMinima::org;
boost::shared_ptr<vincent::PunktList<vincent::lab_value> > GLvlMinima::plist;


void GLvlMinima::reshow(SGLqtSpace &space,GLvlSegment &seg,const shared_obj &self)
{
	if(volume() <= MAX_MINIMA_SIZE)
	{
		if(users.find(&seg)==users.end())show(space,seg,self);//Wenn das aufrufende seg neu hier is, is kein compileNextTime nötig, denn das Min hat sich nich geändert
		else if(myList)compileNextTime();
		else 
		{
			SGLprintError("Hier stimmt was nich, reshow ohne show");
			space.showObj(self);
		}
	}
}


void GLvlMinima::show(SGLqtSpace &space,GLvlSegment &seg,const shared_obj &self)
{
	assert(self.get()==this);
	if(volume() <= MAX_MINIMA_SIZE)
	{
		if(!users.size() )space.showObj(self);
//@todo mylist ließe weitere kontrollen zu
		users.insert(&seg);
	}
}


void GLvlMinima::unshow(SGLqtSpace &space,GLvlSegment &seg,const shared_obj &self)
{	
	users.erase(&seg);
	if(!users.size())space.unshowObj(self);
}

void GLvlMinima::update_users()
{
	for(set<GLvlSegment*>::iterator it=users.begin();it!=users.end();it++)
		(*it)->redisplay();
}
