#-------------------------------------------------
#
# Project created by QtCreator 2013-12-28T16:51:13
#
#-------------------------------------------------


QT       += core gui sql opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Piri
TEMPLATE = app


SOURCES += main.cpp \
    nodegraph.cpp \
    node.cpp \
    edge.cpp \
    backdrop.cpp \
    cornergrabber.cpp \
    tableop.cpp \
    top_createtable.cpp \
    top_select.cpp \
    top_append.cpp \
    mainwindow.cpp \
    knob_callback.cpp \
    knobs.cpp \
    viewer.cpp \
    top_sort.cpp \
    top_sqlcommand.cpp \
    dataop.cpp \
    geo2dop.cpp \
    geo2d_createpoints.cpp \
    geoobject.cpp \
    viewer2dview.cpp \
    viewer3dview.cpp \
    qtlogo.cpp \
    op.cpp \
    viewerop.cpp \
    testobject.cpp

HEADERS  += \
    nodegraph.h \
    node.h \
    edge.h \
    backdrop.h \
    cornergrabber.h \
    tableop.h \
    top_createtable.h \
    top_select.h \
    ops.h \
    top_append.h \
    mainwindow.h \
    knob_callback.h \
    knobs.h \
    viewer.h \
    top_sort.h \
    top_sqlcommand.h \
    dataop.h \
    geo2dop.h \
    geo2d_createpoints.h \
    geoobject.h \
    viewer2dview.h \
    viewer3dview.h \
    qtlogo.h \
    interfaces.h \
    op.h \
    viewerop.h \
    backdrop.h \
    viewerop.h \
    viewer3dview.h \
    viewer2dview.h \
    viewer.h \
    top_sqlcommand.h \
    top_sort.h \
    top_select.h \
    top_createtable.h \
    top_append.h \
    testobject.h \
    tableop.h \
    qtlogo.h \
    ops.h \
    op.h \
    nodegraph.h \
    node.h \
    knobs.h \
    knob_callback.h \
    interfaces.h \
    geoobject.h \
    geo2dop.h \
    geo2d_createpoints.h \
    edge.h \
    dataop.h \
    cornergrabber.h

OTHER_FILES += \
    grades.txt

RESOURCES +=

LIBS = -Lplugins
