#include "top_select.h"
#include "tableop.h"
#include "node.h"
#include "edge.h"
#include "nodegraph.h"
#include "knobs.h"


#include <QtDebug>
#include <QStandardItemModel>
#include <QTableView>

tOp_select::tOp_select(Node* parent) :
    rowFrom(1),
    rowTo(5),
    colFrom(1),
    colTo(5)


{
    myParent = parent;
    description("Select/Select", "Simple row-column select");
}


void tOp_select::knobs(Knob_Callback *f)
{
    qDebug() << "Got KNOBS: select" << f;
    //String_knob(f, &_tekst, "From: ");
    Integer_knob(f, &rowFrom, "Row from: ");
    Integer_knob(f, &rowTo, "Row to: ");
    Integer_knob(f, &colFrom, "Column from: ");
    Integer_knob(f, &colTo, "Column to: ");

}


void tOp_select::engine()
{
    qDebug() << "Engine Select" << myParent->name();
    QStandardItemModel *model = myParent->getParent()->getTableData();


    foreach (Edge* e, myParent->edgesIn())
    {
        e->sourceNode()->execute();
    }

    if (model->columnCount() && model->rowCount())
    {

        rowFrom = validateToRow(model, rowFrom);
        rowTo = validateToRow(model, rowTo);
        colFrom = validateToCol(model, colFrom);
        colTo = validateToCol(model, colTo);
        /*
        if (rowFrom < 1) rowFrom = 1;
        if (rowFrom > model->rowCount()) rowFrom = model->rowCount();
        if (rowTo < rowFrom) rowTo = rowFrom;
        if (rowTo > model->rowCount()) rowTo = model->rowCount();

        if (colFrom < 1) colFrom = 1;
        if (colFrom > model->columnCount()) colFrom = model->rowCount();
        if (colTo < colFrom) colTo = colFrom;
        if (colTo > model->columnCount()) colTo = model->rowCount();
        */

        qDebug() << model->rowCount() << model->columnCount();
        qDebug() << rowFrom << rowTo << colFrom << colTo;

        model->removeRows(rowTo, model->rowCount()- rowTo);
        model->removeColumns(colTo, model->columnCount() - colTo);
        model->removeRows(0,rowFrom-1);
        model->removeColumns(0, colFrom-1);
    }

}
