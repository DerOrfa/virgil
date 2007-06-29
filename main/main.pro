# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./main
# Das Target ist eine Anwendung:  virgil

TARGETDEPS += ../wshed/libwshed.a 
LIBS += ../wshed/libwshed.a \
        -ldcmdata \
        -lofstd \
        -ldcmimgle \
        -lqt_glue \
        -leclasses2 \
        -lvista \
        -lsgl \
        -lboost_signals 
INCLUDEPATH += /usr/include/dcmtk/config \
               /usr/include/dcmtk/dcmimgle 
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
           glvlminima.h \
           bild.h \
           glvlsegment.h \
           glvlsegmentdialog.h \
           glvlpinsdlg.h \
           glvlrule.h \
           glvlposlistener.h \
           bild_dcm.h \
           bild_dcm_impl.h 
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
           glvlpinsdlg.cpp \
           glvlrule.cpp \
           glvlposlistener.cpp 
