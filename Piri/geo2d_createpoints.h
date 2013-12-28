#ifndef GEO2D_CREATEPOINTS_H
#define GEO2D_CREATEPOINTS_H

#include "geo2dop.h"

class geo2d_createPoints : public Geo2dOp
{
public:
    geo2d_createPoints(Node* parent);

    void engine();
    void knobs(Knob_Callback *f);

    int _columnX;
    int _columnY;
    int _columnLabels;
    int _columnCount;
    int _count;
    int _size;
    bool _showLabels;
    bool _keepScene;

};

#endif // GEO2D_CREATEPOINTS_H
