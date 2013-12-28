#ifndef GEO2DOP_H
#define GEO2DOP_H

#include "dataop.h"

#include <QString>

class Node;
class Knob_Callback;
class QGraphicsScene;


class Geo2dOp : public DataOp
{
public:
    Geo2dOp();
    Geo2dOp(Node* parent);
    ~Geo2dOp();

    virtual void engine();
    virtual void knobs(Knob_Callback* f);
    QGraphicsScene* getScene2D();
};

#endif // GEO2DOP_H
