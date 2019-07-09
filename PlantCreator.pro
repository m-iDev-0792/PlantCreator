#-------------------------------------------------
#
# Project created by QtCreator 2018-12-03T21:18:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PlantCreator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    HJGraphics/BasicGLObject.cpp \
    HJGraphics/Camera.cpp \
    HJGraphics/DebugUtility.cpp \
    HJGraphics/ElementObjects.cpp \
    HJGraphics/HJGWidget.cpp \
    HJGraphics/Light.cpp \
    HJGraphics/Material.cpp \
    HJGraphics/Model.cpp \
    HJGraphics/Scene.cpp \
    HJGraphics/Shader.cpp \
	src/main.cpp \
	src/mainwindow.cpp \
	src/painterview.cpp \
	src/sketchview.cpp \
	src/utility.cpp \
	src/leafedge.cpp \
	src/plantcreator.cpp \
	src/branch.cpp \
	src/triangulationview.cpp \
	src/petal.cpp \
	src/plantobject.cpp \
	src/commondatatype.cpp

HEADERS += \
	HJGraphics/BasicGLObject.h \
	HJGraphics/Camera.h \
	HJGraphics/DebugUtility.h \
	HJGraphics/ElementObjects.h \
	HJGraphics/HJGWidget.h \
	HJGraphics/Light.h \
	HJGraphics/Material.h \
	HJGraphics/Model.h \
	HJGraphics/OpenGLHeader.h \
	HJGraphics/Scene.h \
	HJGraphics/Shader.h \
	HJGraphics/stb_image.h \
	include/mainwindow.h \
	include/painterview.h \
	include/sketchview.h \
	include/utility.h \
	include/leafedge.h \
	include/plantcreator.h \
	include/branch.h \
	include/triangulationview.h \
	include/petal.h \
	include/plantobject.h \
	include/commondatatype.h

FORMS += \
        mainwindow.ui
INCLUDEPATH += /usr/local/include\
			   HJGraphics\
			   include

LIBS += -L/usr/local/Cellar/opencv@2/2.4.13.7_2/lib \
	 -lopencv_core \
	 -lopencv_highgui \
	 -lopencv_imgproc \
	 /usr/local/Cellar/assimp/4.0.1/lib/libassimp.4.0.1.dylib\

RESOURCES += \
    src.qrc


