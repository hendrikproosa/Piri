#ifndef CREATETABLEOP_H
#define CREATETABLEOP_H

#include "tableop.h"

#include <QString>

class Node;

class tOp_createTable : public TableOp
{
public:
    tOp_createTable(Node* parent);
    virtual void engine();
    virtual void knobs(Knob_Callback* f);
    QString _file;
    QString _delimiter;
    int _colorSpace;

};

#endif // CREATETABLEOP_H
