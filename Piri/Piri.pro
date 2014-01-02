#-------------------------------------------------
#
# Project created by QtCreator 2013-12-28T16:51:13
#
#-------------------------------------------------


QT       += core gui sql opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Piri
TEMPLATE = app

INCLUDEPATH  += libraries/PiriLib

SOURCES += main.cpp \
    testobject.cpp

HEADERS  += testobject.h \


OTHER_FILES += \
    grades.txt

RESOURCES +=

LIBS = -Lplugins

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libraries/PiriLib/ -lPiriLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libraries/PiriLib/ -lPiriLibd
else:unix: LIBS += -L$$PWD/libraries/PiriLib/ -lPiriLib

INCLUDEPATH += $$PWD/libraries/PiriLib
DEPENDPATH += $$PWD/libraries/PiriLib
