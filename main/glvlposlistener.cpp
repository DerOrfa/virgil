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
#include "glvlposlistener.h"

GLvlPosListener::GLvlPosListener(QString name):SGLSlot()
{
	listenerList.insert(listenerList.begin(),this);
}


GLvlPosListener::~GLvlPosListener()
{
	listenerList.erase(this);
}


std::set<GLvlPosListener*> GLvlPosListener::listenerList;
