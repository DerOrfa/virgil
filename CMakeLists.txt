cmake_minimum_required(VERSION 2.6)

project(virgil)

###########################################################
# find and configure the isis core library
###########################################################
SET(ISIS_DIR /usr/share/isis/cmake CACHE PATH "The directory where the isis cmake configuration script is located.")

find_package(ISIS REQUIRED)
find_package(Qt4 REQUIRED )
FIND_PACKAGE(OpenGL REQUIRED)

set(QT_USE_QTOPENGL TRUE)
set(QT_USE_OPENGL TRUE)

INCLUDE(${QT_USE_FILE})

FIND_PACKAGE(Boost REQUIRED)
find_library(LIBFTGL ftgl)

set(DEFINES -DGL_GLEXT_PROTOTYPES -DBOOST_HAS_PTHREADS)
add_definitions(-DGL_GLEXT_PROTOTYPES -DBOOST_HAS_PTHREADS -DQT_NO_KEYWORDS)

OPTION(${CMAKE_PROJECT_NAME}_RUNTIME_LOG "Enable runtime logging" ON)
OPTION(${CMAKE_PROJECT_NAME}_DEBUG_LOG "Enable debug logging" ON)

# enable RUNTIME_LOG
IF(${CMAKE_PROJECT_NAME}_RUNTIME_LOG)
	ADD_DEFINITIONS(-D_ENABLE_LOG=1)
ELSE(${CMAKE_PROJECT_NAME}_RUNTIME_LOG)
	ADD_DEFINITIONS(-D_ENABLE_LOG=0)
ENDIF(${CMAKE_PROJECT_NAME}_RUNTIME_LOG)

# enable DEBUG_LOG
IF(${CMAKE_PROJECT_NAME}_DEBUG_LOG)
	ADD_DEFINITIONS(-D_ENABLE_DEBUG=1)
	MESSAGE(STATUS "Debug code enabled, this will will have a big impact on the performance")
ELSE(${CMAKE_PROJECT_NAME}_DEBUG_LOG)
	ADD_DEFINITIONS(-D_ENABLE_DEBUG=0)
ENDIF(${CMAKE_PROJECT_NAME}_DEBUG_LOG)

find_path(SGL_INCLUDE_DIR "libsgl/qt4_glue/sglqtspace.h")
INCLUDE_DIRECTORIES(${ISIS_INCLUDE_DIRS} ${SGL_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIR})

find_library(SGL_LIBRARY "sgl")
find_library(SGL_QT_LIBRARY "qt4_glue")


qt4_wrap_ui(UI_H main/GLvlView.ui)
qt4_wrap_cpp(MOCS main/glvlplanecam.h main/glvlplanecursor.h main/glvlplaneview.h main/glvlview.h main/glvlmultiviewmanager.h OPTIONS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)

add_executable(virgil ${MOCS} ${UI_H}
	main/main.cpp
	main/glvlcutplane.cpp
	main/glvlmasterview.cpp
	main/glvlplanecam.cpp
	main/glvlplanecursor.cpp
	main/glvlplaneview.cpp
	main/glvlvolumetex.cpp
	main/glvlposlistener.cpp
	main/glvlview.cpp
	main/glvlmultiviewmanager.cpp
	main/imgframe.cpp
	main/glvlplanecursor.cpp
)
target_link_libraries(virgil ${SGL_LIBRARY} ${SGL_QT_LIBRARY} ${ISIS_LIB} ${ISIS_LIB_DEPENDS} ${OPENGL_LIBRARIES} ${SGL_LIBRARY} ${SGL_QT_LIBRARY} ${QT_LIBRARIES})
