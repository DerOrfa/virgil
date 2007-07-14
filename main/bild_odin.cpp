//
// C++ Implementation: bild_odin
//
// Description: 
//
//
// Author: Enrico Reimer,,, <reimer@cbs.mpg.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bild_odin.h"

Bild_odin::Bild_odin(Protocol prot, Data<float,4> dat): 
		Bild<float>(x, y, z)
{
}


Bild_odin::~Bild_odin()
{
}


float& Bild_odin::at(const short x, const unsigned short y, const unsigned short z)
{
    return Bild<float>::at(x, y, z);
}

float* Bild_odin::copy_line(const unsigned short y, const unsigned short z, void* dst)
{
    return Bild<float>::copy_line(y, z, dst);
}

float& Bild_odin::at(const unsigned int index)
{
}

