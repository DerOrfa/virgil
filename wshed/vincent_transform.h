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
#include <boost/shared_ptr.hpp>

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
Q_OBJECT
	Bild_vimage<VUByte> im;
public:
	PunktList<VUByte> D;
	transform(VImage src);
	void test();
	boost::shared_ptr< Bild_vimage<lab_value> > operator()();
	static boost::shared_ptr< PunktList<lab_value> > getVoxels(const Bild_vimage<lab_value> &im);

signals:
    void reached(vincent::lab_value h,unsigned short objs);
};

}
#endif
