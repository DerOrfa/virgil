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
#include <qthread.h>
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
typedef vincent::Bild_vimage<VBild_value> VBild;

extern const lab_value WSHED_INIT;
extern const lab_value WSHED_MASK;
extern const lab_value WSHED_WSHED;

class transform : public QObject, public QThread
{
Q_OBJECT
public:
	Bild_vimage<VUByte> im;
	PunktList<VUByte> D;
	SGLshPtr< Bild_vimage<lab_value> > last_erg;
	
	transform(VImage src);
	void run();
	SGLshPtr< Bild_vimage<lab_value> > operator()();
	SGLshPtr< PunktList<lab_value> > getVoxels(const Bild_vimage<lab_value> &im)const;
    void init();

signals:
    void reached(vincent::VBild_value h,unsigned short objs);
    void msg(QString msg,bool canskip);
	void end();
};

}
#endif
