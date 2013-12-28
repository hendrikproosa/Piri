#ifndef TOP_SELECT_H
#define TOP_SELECT_H

#include "tableop.h"
#include <QString>

class Node;

class tOp_select : public TableOp
{
public:
    tOp_select(Node* parent);
    virtual void engine();
    virtual void knobs(Knob_Callback* f);
    int rowFrom;
    int rowTo;
    int colFrom;
    int colTo;
    QString _tekst;


private:
    //Node* myParent;


};

#endif // TOP_SELECT_H
