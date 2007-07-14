# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./main
# Das Target ist eine Anwendung:  virgil

LIBS += -lodindata \
        -lqt_glue \
        -leclasses2 \
        -lsgl \
        -lboost_signals 
INCLUDEPATH += /usr/local/include 
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
         PlaneView.ui \
         segments.ui \
         pins.ui \
         config.ui 
HEADERS += glvlcutplane.h \
           glvlmasterview.h \
           glvlpin.h \
           glvlplanecam.h \
           glvlplanecursor.h \
           glvlplaneview.h \
           glvlvolumetex.h \
           bild.h \
           glvlsegment.h \
           glvlsegmentdialog.h \
           glvlpinsdlg.h \
           glvlrule.h \
           glvlposlistener.h \
           bild_odin.h 
SOURCES += main.cpp \
           glvlcutplane.cpp \
           glvlmasterview.cpp \
           glvlpin.cpp \
           glvlplanecam.cpp \
           glvlplanecursor.cpp \
           glvlplaneview.cpp \
           glvlvolumetex.cpp \
           glvlpinsdlg.cpp \
           glvlrule.cpp \
           glvlposlistener.cpp 
