# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./main
# Target is an application:  virgil

TARGETDEPS += ../wshed/libwshed.a 
LIBS += ../wshed/libwshed.a \
        -lqt_glue \
        -leclasses2 \
        -lvista \
        -lsgl \
        -lboost_signals 
INCLUDEPATH = ../../qt_glue \
              /opt/prog/eclasses/eclasses2/ \
              $(HOME)/emath/ \
              ../../libsgl/ 
QMAKE_LIBDIR = ../wshed \
               ../../qt_glue \
               /opt/prog/eclasses/eclasses2 \
               ../../libsgl 
DEFINES = GL_GLEXT_PROTOTYPES 
TARGET = virgil 
CONFIG += debug \
          warn_off \
          qt \
          opengl \
          thread \
          x11 
TEMPLATE = app 
FORMS += newPinDlg.ui \
         PlaneView.ui 
HEADERS += glvlcutplane.h \
           glvlmasterview.h \
           glvlpin.h \
           glvlplanecam.h \
           glvlplanecursor.h \
           glvlplaneview.h \
           glvlvolumetex.h \
           glvlminima.h 
SOURCES += main.cpp \
           glvlcutplane.cpp \
           glvlmasterview.cpp \
           glvlpin.cpp \
           glvlplanecam.cpp \
           glvlplanecursor.cpp \
           glvlplaneview.cpp \
           glvlvolumetex.cpp \
           glvlminima.cpp 
