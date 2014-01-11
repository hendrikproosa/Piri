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


void Op::showError(QString msg)
{
    //qDebug() << "Vastus on 42";
    //getCallback()->showError(msg);
}


void Op::setParent(Node *node)
{
    myParent = node;
}


void Op::disabled()
{
    if (myParent->edgesIn().count())
    {
        foreach (Edge* e, myParent->edgesIn())
        {
            e->sourceNode()->execute();
        }
    } else {
        QStandardItemModel *model = getTableModel();
        QGraphicsScene *scene = getScene2D();
        model->clear();
        scene->clear();
    }
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

Viewer* Op::getViewer()
{
    return myParent->getParent()->getParent()->getViewer();
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

int Op::validateToRow(QStandardItemModel *model, int value)
{
    if (value < 1) return 1;
    if (value > model->rowCount()) return model->rowCount();
    return value;
}

QWidget* Op::getKnob(QString knobName)
{
    return myCallback->getKnob(knobName)->widget;
}

void Op::copyHeaders(QStandardItemModel *from, QStandardItemModel *to)
{
    qDebug() << "Copyheaders..." << from << from->columnCount();
    QStringList labelList;
    int cc = from->columnCount();
    if (to->columnCount() > cc) { cc = to->columnCount(); }
    for (int c = 0; c < cc; c++)
    {
        if (from->horizontalHeaderItem(c))
        {
            labelList << from->horizontalHeaderItem(c)->text();
        }
        else if (to->horizontalHeaderItem(c))
        {
            labelList << to->horizontalHeaderItem(c)->text();
        }

        else {
            labelList << QString("%1").arg(c+1);
        }
    }
    qDebug() << labelList;
    to->setHorizontalHeaderLabels(labelList);
    qDebug() << "Label copy ok!";
}
