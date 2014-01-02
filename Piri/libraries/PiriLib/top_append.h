#ifndef TOP_APPEND_H
#define TOP_APPEND_H

#include <QStandardItem>

#include "tableop.h"

#include "pirilib.h"

class Node;

class PIRILIBSHARED_EXPORT tOp_append : public TableOp
{
public:
    tOp_append(Node* parent);
    virtual void engine();
    virtual void knobs(Knob_Callback* f);

private:
    //Node* myParent;
    bool _removeDoubles;
    int _topLayer;
    QString _strtest;

    bool contains(QStandardItemModel *mod, QList<QStandardItem*> list);

};

#endif // TOP_APPEND_H
