//
// C++ Interface: bild_odin
//
// Description: 
//
//
// Author: Enrico Reimer,,, <reimer@cbs.mpg.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BILD_ODIN_H
#define BILD_ODIN_H

#include <bild.h>
#include <odindata/fileio.h>

/**
	@author Enrico Reimer,,, <reimer@cbs.mpg.de>
*/

template<class T> class Bild_odin : public Bild<T>
{
public:
	Data<T,4> data;
	Protocol prot;
	Bild_odin(std::pair<const Protocol, Data<T, 4> > data_pair):
		Bild<T>(data_pair.second.shape()[3], 
						data_pair.second.shape()[2], 
						data_pair.second.shape()[1]>1 ? data_pair.second.shape()[1]:data_pair.second.shape()[0]),
		prot(data_pair.first),data(data_pair.second)
	{init();}

	Bild_odin(Protocol _prot, Data<T,4> _data):Bild<T>(
			_data.shape()[3], 
			_data.shape()[2], 
			_data.shape()[1]>1 ? _data.shape()[1]:_data.shape()[0]),
			prot(_prot),data(_data)
	{init();}
	void init()
	{
		Geometry &geo=prot.geometry;
		this->xsize.setElsize(geo.get_FOV(readChannel)/this->xsize.getCnt('X'));
		this->ysize.setElsize(geo.get_FOV(phaseChannel)/this->ysize.getCnt('Y'));
		this->zsize.setElsize(geo.get_FOV(sliceChannel)/this->zsize.getCnt('Z'));
	}
	~Bild_odin(){
	}
	
	T& at(const short x, const unsigned short y, const unsigned short z)
	{
		return data(0,z,y,x);
	}

	T* copy_line(const unsigned short y, const unsigned short z, void* dst)
	{
		const unsigned short slice_size=this->xsize*this->ysize;
		
		return (T*)memcpy(dst,data.c_array()+y*this->xsize+z*slice_size,slice_size*sizeof(T));
	}

	T& at(const unsigned int index)
	{
		return *(data.c_array()+index);
	}
};

#endif
