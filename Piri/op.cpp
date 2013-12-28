#include "op.h"
#include "edge.h"
#include "node.h"
#include "knob_callback.h"
#include "nodegraph.h"
#include "mainwindow.h"
#include "dataop.h"

void Op::evaluate()
{
    myParent->getOp()->engine();
}

void Op::setCallback(Knob_Callback* cb)
{
    myCallback = cb;
}


void Op::disabled()
{
    if (myParent->edgesIn().count())
    {
        Edge* e = myParent->edgesIn().first();
        e->sourceNode()->execute();
    } else {
        QStandardItemModel *model = getTableModel();
        QGraphicsScene *scene = getScene2D();
        model->clear();
        scene->clear();
    }
}

void Op::showError(QString msg)
{
    getCallback()->showError(msg);
}


void Op::setParent(Node *node)
{
    myParent = node;
}

QStandardItemModel* Op::getTableModel()
{
    qDebug() << "Op::getTableModel..." << myParent->getParent();
    return myParent->getParent()->getTableData();
}

QGraphicsScene* Op::getScene2D()
{
    qDebug() << "Op::getScene2D..." << myParent;//->getParent();
    return myParent->getParent()->getParent()->getScene2D();
}

int Op::numInputs()
{
    qDebug() << "Inputs: " << inputCount;
    return inputCount;
}

QList<Edge*> Op::inputs()
{
    return myInputs;
}

int Op::validateToCol(QStandardItemModel *model, int value)
{
    if (value < 1) return 1;
    if (value > model->columnCount()) return model->columnCount();
    return value;
}
