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
#ifndef GLVLPOSLISTENER_H
#define GLVLPOSLISTENER_H

#include <set>
#include <sglsignal.h>
#include <qstring.h>
/**
@author Enrico Reimer,,,
*/
class GLvlPosListener : public SGLSlot
{
public:
    GLvlPosListener(QString name);
    ~GLvlPosListener();
    static std::set<GLvlPosListener*> listenerList;
};

#endif
