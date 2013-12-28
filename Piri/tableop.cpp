#include "tableop.h"
#include "node.h"
#include "edge.h"
#include "nodegraph.h"
#include "knob_callback.h"

#include <QtDebug>
#include <QStandardItemModel>

TableOp::TableOp() : DataOp()
{
    description("Other/TableOp", "TableOp baseclass");
}

TableOp::~TableOp()
{
    qDebug() << "TableOp Destructor";
    delete myCallback;
}



TableOp::TableOp(Node *parent) : DataOp(parent)
{
    myParent = parent;
    description("Other/Viewer", "TableOp baseclass");
}



QStandardItemModel* TableOp::getTableModel()
{
    return myParent->getParent()->getTableData();
}


void TableOp::knobs(Knob_Callback *f)
{

}


void TableOp::engine()
{
    qDebug() << "Engine TableOp" << myParent->name();

    if (myParent->edgesIn().count())
    {
        foreach (Edge* e, myParent->edgesIn())
        {
            e->sourceNode()->execute();
        }
    }
}
