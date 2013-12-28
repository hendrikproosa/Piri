#include "top_sort.h"
#include "tableop.h"
#include "node.h"
#include "edge.h"
#include "nodegraph.h"
#include "knobs.h"


#include <QtDebug>
#include <QStandardItemModel>
#include <QTableView>

tOp_sort::tOp_sort(Node* parent) :
    sortColumn(1),
    sortOrder(0)
{
    myParent = parent;
    description("Select/Sort", "Simple column based sorting");
}



void tOp_sort::knobs(Knob_Callback *f)
{
    qDebug() << "Got KNOBS: sort" << f;
    Integer_knob(f, &sortColumn, "Column to sort: ");
    ComboBox_knob(f, &sortOrder, "Sort: ", "sort");
    ADD_VALUES(f, "Ascending,Descending");

}


void tOp_sort::engine()
{
    QStandardItemModel *model = myParent->getParent()->getTableData();

    foreach (Edge* e, myParent->edgesIn())
    {
        e->sourceNode()->execute();
    }

    if (sortOrder)
    {
        model->sort(sortColumn - 1, Qt::DescendingOrder);
    }
    else
    {
        model->sort(sortColumn - 1, Qt::AscendingOrder);
    }
}
