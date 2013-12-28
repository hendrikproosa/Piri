#ifndef TOP_SORT_H
#define TOP_SORT_H

#include "tableop.h"
#include <QString>

class Node;

class tOp_sort : public TableOp
{
public:
    tOp_sort(Node* parent);
    virtual void engine();
    virtual void knobs(Knob_Callback* f);
    int sortColumn;
    int sortOrder;


private:
    //Node* myParent;


};

#endif // TOP_SORT_H
