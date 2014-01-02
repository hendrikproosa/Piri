#include "geo2dop.h"
#include "node.h"
#include "edge.h"
#include "nodegraph.h"
#include "knob_callback.h"
#include "mainwindow.h"

#include <QtDebug>
#include <QGraphicsScene>

Geo2dOp::Geo2dOp() : DataOp()
{
    description("Other/TableOp", "TableOp baseclass");
}

Geo2dOp::~Geo2dOp()
{
    qDebug() << "Geo2dOp Destructor";
    delete myCallback;
}


Geo2dOp::Geo2dOp(Node *parent) : DataOp(parent)
{
    description("Other/Geo2dOp", "Geo2dOp baseclass");
}


QGraphicsScene* Geo2dOp::getScene2D()
{
    return myParent->getParent()->getParent()->getScene2D();
}


void Geo2dOp::knobs(Knob_Callback *f)
{

}


void Geo2dOp::engine()
{
    qDebug() << "Geo2dOp" << myParent->name();

    if (myParent->edgesIn().count())
    {
        foreach (Edge* e, myParent->edgesIn())
        {
            e->sourceNode()->execute();
        }
    }
}
