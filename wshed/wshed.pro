# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterverzeichnis relativ zum Projektverzeichnis: ./wshed
# Das Target ist eine Bibliothek:  

INCLUDEPATH = $(HOME)/emath/ \
              ../../libsgl/ 
CONFIG += debug \
          warn_on \
          thread \
          staticlib 
TEMPLATE = lib 
HEADERS += vincent_bild.h \
           vincent_punkt.h \
           vincent_transform.h 
SOURCES += vincent_transform.cpp 
