#ifndef CREATEPOLYGONS_GLOBAL_H
#define CREATEPOLYGONS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CREATEPOLYGONS_LIBRARY)
#  define CREATEPOLYGONSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CREATEPOLYGONSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CREATEPOLYGONS_GLOBAL_H
