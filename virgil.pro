# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterverzeichnis relativ zum Projektverzeichnis: .
# Das Target ist Projekt im Unterverzeichnis 

DEFINES = GL_GLEXT_PROTOTYPES \
          BOOST_HAS_PTHREADS 
CONFIG += release \
          warn_on \
          qt \
          opengl \
          thread \
          x11 
TEMPLATE = subdirs 
SUBDIRS += wshed \
           main 
