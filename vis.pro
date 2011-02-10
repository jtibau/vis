TEMPLATE = app
QT += opengl
CONFIG += console debug

INCLUDEPATH += $(VJ_BASE_DIR)/include/vrjuggler-3.1.1 \
	$(VJ_BASE_DIR)/include/cppdom-1.0.3 \
	$(VJ_BASE_DIR)/include/gadgeteer-2.1.13 \
	$(VJ_BASE_DIR)/include/gmtl-0.6.1 \
	$(VJ_BASE_DIR)/include/jccl-1.5.0 \
	$(VJ_BASE_DIR)/include/vpr-2.3.2

LIBS += -L$(VJ_BASE_DIR)/lib \
	-lgadget-2_1_13 \
	-ljccl-1_5_0 \
	-lvpr-2_3_2 \
	-lvrj-3_1_1 -lvrj_ogl-3_1_1 \
	-lsonix-1_5_0 

DEFINES += BOOST_ALL_DYN_LINK JUGGLER_DEBUG

LIBS += -lGL -lGLU -lGLEW -lglut \
	-losg -losgDB -losgGA -losgUtil \
	-lOpenThreads

HEADERS += src/App.h src/InputHandler.h src/InteractionMethod.h src/Scene.h
SOURCES += src/App.cpp src/InputHandler.cpp src/main.cpp src/Scene.cpp

DEPENDPATH = src
OBJECTS_DIR = build
MOC_DIR = build
DESTDIR = bin
