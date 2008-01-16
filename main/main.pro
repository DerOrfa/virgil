# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./main
# Das Target ist eine Anwendung:  virgil

QMAKE_CXXFLAGS_RELEASE += -march=pentium4 \
                          -ftree-vectorize \
                          -ffast-math \
                          -O3 
QMAKE_CXXFLAGS_DEBUG += -g3 -O0 
DEFINES = GL_GLEXT_PROTOTYPES
TARGET = virgil 
CONFIG += debug qt opengl thread x11 
TEMPLATE = app 
FORMS += newPinDlg.ui \
         PlaneView.ui \
         segments.ui \
         pins.ui \
         config.ui \
         odindataselectorbase.ui 
HEADERS += glvlcutplane.h \
           glvlmasterview.h \
           glvlpin.h \
           glvlplanecam.h \
           glvlplanecursor.h \
           glvlplaneview.h \
           glvlvolumetex.h \
           bild.h \
           glvlpinsdlg.h \
           glvlrule.h \
           glvlposlistener.h \
           bild_odin.h \
           odindataselector.h 
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
           glvlposlistener.cpp \
           odindataselector.cpp 

LIBS += -L/SCR/odin_rel \
  -lblitz \
  -lfreki \
  -lqt_glue \
  -leclasses2 \
  -lsgl \
  -lboost_signals

INCLUDEPATH += /usr/local/include \
  /SCR/odin

CONFIG -= release

