//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <qobject.h>
#include "vincent_bild.h"
#include "vincent_punkt.h"
#include <vista/Vlib.h>

/**
@author Enrico Reimer,,,
*/
namespace vincent
{
class transform : public QObject
{
	Bild_vimage<VUByte> im;
public:
	PunktList<VUByte> D;
	transform(VImage src);
	~transform();
	void test();
};

}
#endif
