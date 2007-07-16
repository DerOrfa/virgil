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
	{}

	Bild_odin(Protocol prot, Data<T,4> data):Bild<T>(
			data.shape()[3], 
			data.shape()[2], 
			data.shape()[1]>1 ? data.shape()[1]:data.shape()[0])
	{}
	~Bild_odin(){
	}
	
	T& at(const short x, const unsigned short y, const unsigned short z)
	{
		data(0,z,y,x);
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
