# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterverzeichnis relativ zum Projektverzeichnis: ./main
# Das Target ist eine Anwendung:  virgil

TARGETDEPS += ../wshed/libwshed.a 
LIBS += ../wshed/libwshed.a \
        -lqt_glue \
        -leclasses2 \
        -lvista \
        -lsgl \
        -lboost_signals 
QMAKE_LIBDIR = ../wshed 
DEFINES = GL_GLEXT_PROTOTYPES 
TARGET = virgil 
CONFIG += release \
          warn_off \
          qt \
          opengl \
          thread \
          x11 
TEMPLATE = app 
FORMS += newPinDlg.ui \
         PlaneView.ui \
         segments.ui \
         pins.ui 
HEADERS += glvlcutplane.h \
           glvlmasterview.h \
           glvlpin.h \
           glvlplanecam.h \
           glvlplanecursor.h \
           glvlplaneview.h \
           glvlvolumetex.h \
           glvlminima.h \
           bild.h \
           glvlsegment.h \
           glvlsegmentdialog.h \
           glvlpinsdlg.h 
SOURCES += main.cpp \
           glvlcutplane.cpp \
           glvlmasterview.cpp \
           glvlpin.cpp \
           glvlplanecam.cpp \
           glvlplanecursor.cpp \
           glvlplaneview.cpp \
           glvlvolumetex.cpp \
           glvlminima.cpp \
           glvlsegment.cpp \
           glvlsegmentdialog.cpp \
           glvlpinsdlg.cpp 
