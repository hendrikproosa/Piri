
TEMPLATE      = lib
CONFIG       += plugin
QT           += widgets core gui opengl sql
INCLUDEPATH  += ../../
HEADERS      += createlines.h \
    ../../knobs.h \
    ../../knob_callback.h \
    ../../node.h \
    ../../edge.h \
    ../../nodegraph.h \
    ../../mainwindow.h \
    ../../dataop.h \
    ../../viewer.h \
    ../../backdrop.h \
    ../../viewer3dview.h \
    ../../viewer2dview.h \
    ../../cornergrabber.h \
    ../../qtlogo.h \
    ../../op.h \
    ../../top_sqlcommand.h \
    ../../top_sort.h \
    ../../top_select.h \
    ../../top_createtable.h \
    ../../top_append.h \
    ../../tableop.h \
    ../../geo2d_createpoints.h \
    ../../geoobject.h \
    ../../geo2dop.h \
    ../../viewerop.h

SOURCES      += createlines.cpp \
    ../../knobs.cpp \
    ../../knob_callback.cpp \
    ../../node.cpp \
    ../../edge.cpp \
    ../../nodegraph.cpp \
    ../../mainwindow.cpp \
    ../../dataop.cpp \
    ../../viewer.cpp \
    ../../backdrop.cpp \
    ../../viewer3dview.cpp \
    ../../viewer2dview.cpp \
    ../../cornergrabber.cpp \
    ../../qtlogo.cpp \
    ../../op.cpp \
    ../../top_sqlcommand.cpp \
    ../../top_sort.cpp \
    ../../top_select.cpp \
    ../../top_createtable.cpp \
    ../../top_append.cpp \
    ../../tableop.cpp \
    ../../geo2d_createpoints.cpp \
    ../../geo2dop.cpp \
    ../../geoobject.cpp \
    ../../viewerop.cpp


TARGET        = $$qtLibraryTarget(createlines)
DESTDIR       = ../../plugins

target.path = ../../plugins
INSTALLS += target
