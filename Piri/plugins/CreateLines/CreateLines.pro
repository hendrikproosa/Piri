
TEMPLATE      = lib
CONFIG       += plugin
QT           += widgets core gui opengl sql
INCLUDEPATH  += ../../ \
                ../../libraries \
                ../../libraries/PiriLib \
                ../

HEADERS      += createlines.h \
                interfaces.h

SOURCES      += createlines.cpp

TARGET        = $$qtLibraryTarget(createlines)
DESTDIR       = ../../plugins

target.path = ../../plugins
INSTALLS += target


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/include/ -lPiriLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/include/ -lPiriLibd
else:unix: LIBS += -L$$PWD/include/ -lPiriLib

INCLUDEPATH += $$PWD/include \
               $$PWD/../
DEPENDPATH += $$PWD/include  \
               $$PWD/../
