# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: .
# Das Target ist Projekt im Unterordner 

CONFIG += release \
          warn_on \
          qt \
          opengl \
          thread \
          x11 
TEMPLATE = subdirs 
SUBDIRS += wshed \
           main 
