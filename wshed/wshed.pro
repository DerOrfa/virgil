# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./wshed
# Das Target ist eine Bibliothek:  

INCLUDEPATH += ../../ 
CONFIG += debug \
          warn_on \
          thread \
          staticlib 
TEMPLATE = lib 
HEADERS += vincent_bild.h \
           vincent_punkt.h \
           vincent_transform.h 
SOURCES += vincent_transform.cpp 
