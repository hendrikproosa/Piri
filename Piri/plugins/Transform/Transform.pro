
TEMPLATE      = lib
CONFIG       += plugin
QT           += widgets core gui opengl sql
INCLUDEPATH  += ../../libraries/PiriLib
HEADERS      += transform.h

SOURCES      += transform.cpp \

TARGET        = transform
DESTDIR       = ../../plugins

target.path = ../../plugins
INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../libraries/PiriLib/ -lPiriLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../libraries/PiriLib/ -lPiriLibd
else:unix: LIBS += -L$$PWD/../../libraries/PiriLib/ -lPiriLib

INCLUDEPATH += $$PWD/../../libraries/PiriLib
DEPENDPATH += $$PWD/../../libraries/PiriLib
