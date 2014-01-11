#include "select.h"

#include <QtDebug>
#include <QStandardItemModel>
#include <QTableView>
#include <QList>

void Select::setup()
{
    rowFrom = 1;
    rowTo = 5;
    colFrom = 1;
    colTo = 5;
}

void Select::description(QString name, QString desc)
{
    setup();
}


QString Select::description()
{
    setup();
    return QString("Query/Select;Simple select./1");
}


void Select::knobs(Knob_Callback *f)
{
    Integer_knob(f, &rowFrom, "Row from: ");
    Integer_knob(f, &rowTo, "Row to: ");
    Integer_knob(f, &colFrom, "Column from: ");
    Integer_knob(f, &colTo, "Column to: ");
}


void Select::engine()
{
    QElapsedTimer timer;
    timer.start();


    qDebug() << "Engine Select" << myParent->name();
    QStandardItemModel *model = myParent->getParent()->getTableData();

    qDebug() << "Select Node init took: " << timer.elapsed() << "milliseconds";
    foreach (Edge* e, myParent->edgesIn())
    {
        e->sourceNode()->execute();
    }
    qDebug() << "Select previous nodes took: " << timer.elapsed() << "milliseconds";
    timer.restart();
    if (model->columnCount() && model->rowCount())
    {

        rowFrom = validateToRow(model, rowFrom);
        rowTo = validateToRow(model, rowTo);
        colFrom = validateToCol(model, colFrom);
        colTo = validateToCol(model, colTo);

        model->removeRows(rowTo, model->rowCount()- rowTo);
        model->removeColumns(colTo, model->columnCount() - colTo);
        model->removeRows(0,rowFrom-1);
        model->removeColumns(0, colFrom-1);
    }
    qDebug() << "Select Node took: " << timer.elapsed() << "milliseconds";
}
