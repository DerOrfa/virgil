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
typedef VUByte VBild_value;
typedef VShort lab_value;
typedef Bild_vimage<VBild_value> VBild;

class transform : public QObject
{
	Bild_vimage<VUByte> im;
public:
	PunktList<VUByte> D;
	transform(VImage src);
	void test();
	Bild_vimage<lab_value> operator()();

};

}
#endif
