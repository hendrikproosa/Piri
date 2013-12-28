#ifndef TABLEOP_H
#define TABLEOP_H

#include "dataop.h"

#include <QString>

class Node;
class Knob_Callback;
class QStandardItemModel;


class TableOp : public DataOp
{
public:
    TableOp();
    TableOp(Node* parent);
    ~TableOp();

    void engine();
    void knobs(Knob_Callback* f);
    QStandardItemModel* getTableModel();
};

#endif // TABLEOP_H
