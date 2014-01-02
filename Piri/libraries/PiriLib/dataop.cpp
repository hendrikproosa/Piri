#include "dataop.h"
#include "node.h"
#include "edge.h"
#include "nodegraph.h"
#include "knob_callback.h"
#include "mainwindow.h"
#include "viewer.h"

#include <QtDebug>


DataOp::DataOp()
{
    description("Other/DataOp", "DataOp is the baseclass of all Ops");
}



DataOp::~DataOp()
{
    qDebug() << "DataOp Destructor";
    delete myCallback;
}



DataOp::DataOp(Node *parent)
{
    myParent = parent;
    description("Other/DataOp", "DataOp baseclass");
}


void DataOp::setCallback(Knob_Callback* cb)
{
    myCallback = cb;
}


void DataOp::knobs(Knob_Callback *f)
{

}


void DataOp::make_knobs(Knob_Callback *f)
{
    this->knobs(f);
    makeKnobs = false;
}

void DataOp::engine()
{
    qDebug() << "Engine DataOp" << myParent->name();

    if (myParent->edgesIn().count())
    {
        foreach (Edge* e, myParent->edgesIn())
        {
            e->sourceNode()->execute();
        }
    }
}


void DataOp::disabled()
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

QStandardItemModel* DataOp::getTableModel()
{
    return myParent->getParent()->getTableData();
}

QGraphicsScene* DataOp::getScene2D()
{
    return myParent->getParent()->getParent()->getScene2D();
}

Viewer* DataOp::getViewer()
{
    return myParent->getParent()->getParent()->getViewer();
}


bool DataOp::isMakeKnobs()
{
    return makeKnobs;
}


void DataOp::description(QString name, QString desc)
{
    _myName = name;
    _myDesc = desc;
}

QString DataOp::description()
{
    return _myName + ";" + _myDesc;
}


void DataOp::showError(QString msg)
{
    getCallback()->showError(msg);
}


int DataOp::validateToCol(QStandardItemModel *model, int value)
{
    if (value < 1) return 1;
    if (value > model->columnCount()) return model->columnCount();
    return value;
}

int DataOp::validateToRow(QStandardItemModel *model, int value)
{
    if (value < 1) return 1;
    if (value > model->rowCount()) return model->rowCount();
    return value;
}

void DataOp::copyHeaders(QStandardItemModel *from, QStandardItemModel *to)
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
