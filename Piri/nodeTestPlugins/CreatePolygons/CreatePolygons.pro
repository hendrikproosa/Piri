
TEMPLATE      = lib
CONFIG       += plugin
QT           += widgets core gui opengl sql
INCLUDEPATH  += ../../
HEADERS      += createpolygons.h

SOURCES      += createpolygons.cpp \

TARGET        = $$qtLibraryTarget(createpolygons)
DESTDIR       = ../../plugins

target.path = ../../plugins
INSTALLS += target
