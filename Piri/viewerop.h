#ifndef VIEWEROP_H
#define VIEWEROP_H

#include "dataop.h"

#include <QString>

class Node;
class Knob_Callback;
class QStandardItemModel;


class ViewerOp : public DataOp
{
public:
    ViewerOp();
    ViewerOp(Node* parent);
    ~ViewerOp();

    void engine();
    void knobs(Knob_Callback* f);

    bool _enableSorting;
    bool _constSize;
    int _pointSize;
    bool _constWidth;
    int _penWidth;
    bool _overRide;
    bool _showVerts;

};

#endif // VIEWEROP_H
