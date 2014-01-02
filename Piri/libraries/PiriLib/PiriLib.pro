#-------------------------------------------------
#
# Project created by QtCreator 2013-12-29T17:58:47
#
#-------------------------------------------------

QT       += gui core sql opengl

TARGET = PiriLib
TEMPLATE = lib

DEFINES += PIRILIB_LIBRARY

SOURCES += pirilib.cpp \
    nodegraph.cpp \
    knob_callback.cpp \
    knobs.cpp \
    edge.cpp \
    node.cpp \
    backdrop.cpp \
    mainwindow.cpp \
    tableop.cpp \
    dataop.cpp \
    viewer.cpp \
    viewer2dview.cpp \
    viewer3dview.cpp \
    qtlogo.cpp \
    geoobject.cpp \
    op.cpp \
    viewerop.cpp \
    top_createtable.cpp \
    top_select.cpp \
    top_append.cpp \
    top_sort.cpp \
    top_sqlcommand.cpp \
    geo2d_createpoints.cpp \
    geo2dop.cpp \
    cornergrabber.cpp \
    knob.cpp \

HEADERS += pirilib.h\
        pirilib_global.h \
    nodegraph.h \
    interfaces.h \
    knob_callback.h \
    knobs.h \
    edge.h \
    node.h \
    backdrop.h \
    mainwindow.h \
    tableop.h \
    dataop.h \
    viewer.h \
    viewer2dview.h \
    viewer3dview.h \
    qtlogo.h \
    geoobject.h \
    op.h \
    ops.h \
    viewerop.h \
    top_createtable.h \
    top_select.h \
    top_append.h \
    top_sort.h \
    top_sqlcommand.h \
    geo2d_createpoints.h \
    geo2dop.h \
    cornergrabber.h \
    knob.h \

