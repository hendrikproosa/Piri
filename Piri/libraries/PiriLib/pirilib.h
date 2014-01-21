#ifndef PIRILIB_H
#define PIRILIB_H

#include "pirilib_global.h"
#include "knobs.h"
#include "node.h"
#include "edge.h"
#include "nodegraph.h"
#include "geoobject.h"

class PIRILIBSHARED_EXPORT PiriLib
{
    
public:
    PiriLib();
};


QString generateHash(QString hashBase);


#endif // PIRILIB_H
