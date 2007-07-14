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
		Bild<T>(data_pair.second.shape()[3], data_pair.second.shape()[2], data_pair.second.shape()[1]),
		prot(data_pair.first),data(data_pair.second)
	{}

	Bild_odin(Protocol prot, Data<T,4> data):Bild<T>(data.shape()[3], data.shape()[2], data.shape()[1])
	{
	}
	~Bild_odin(){}
	
	T& at(const short x, const unsigned short y, const unsigned short z)
	{
		data[z][y][x];
	}

	T* copy_line(const unsigned short y, const unsigned short z, void* dst)
	{
		return Bild<float>::copy_line(y, z, dst);
	}

	virtual T& at(const unsigned int index)
	{
	}
};

#endif
