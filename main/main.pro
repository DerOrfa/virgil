# File generated by kdevelop's qmake manager.
# -------------------------------------------
# Subdir relative project main directory: ./main
# Target is an application:  virgil

LIBS += -lqt4_glue -lsgl -lboost_signals-mt
QMAKE_CXXFLAGS_DEBUG += -g3 -O0
DEFINES = GL_GLEXT_PROTOTYPES
TARGET = virgil
TEMPLATE = app
QT += opengl
CONFIG += debug no_keywords

FORMS += \
	GlvlView.ui
HEADERS += glvlcutplane.h \
		   glvlmasterview.h \
		   glvlplanecam.h \
		   glvlplanecursor.h \
		   glvlplaneview.h \
		   glvlvolumetex.h \
		   bild.h \
		   glvlposlistener.h \
		   bild_odin.h
SOURCES += main.cpp \
		   glvlcutplane.cpp \
		   glvlmasterview.cpp \
		   glvlplanecam.cpp \
		   glvlplanecursor.cpp \
		   glvlplaneview.cpp \
		   glvlvolumetex.cpp \
		   glvlposlistener.cpp
