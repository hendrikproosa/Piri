#include "top_append.h"
#include "tableop.h"
#include "node.h"
#include "edge.h"
#include "nodegraph.h"
#include "knob_callback.h"
#include "knobs.h"

#include <QtDebug>
#include <QStandardItemModel>
#include <QTableView>
#include <QList>

tOp_append::tOp_append(Node* parent) :
    _removeDoubles(false),
    _topLayer(1),
    _strtest("col1, col2, col4")
{
    myParent = parent;
    description("Merge/Append", "Append two tables. B input is first, A is second.");
}



void tOp_append::knobs(Knob_Callback *f)
{
    CheckBoxKnob *chk = CheckBox_knob(f, &_removeDoubles, "Remove doubles: ");
    Integer_knob(f, &_topLayer, "Layer on top: ");
    // Näide sellest, kuidas ühendada nuppude parameetreid omavahel
    //StringKnob *knob = String_knob(f, &_strtest, "Parameetrid: ");
    //knob->setEnabled(!_removeDoubles);
    //QObject::connect(chk, SIGNAL(toggled(bool)), knob, SLOT(setDisabled(bool)));

}


void tOp_append::engine()
{
    qDebug() << "Engine Append" << myParent->name();
    QStandardItemModel *model = myParent->getParent()->getTableData();
    QGraphicsScene *scene = getScene2D();
    QStandardItemModel *tempModel = new QStandardItemModel();
    QGraphicsScene *tempScene = new QGraphicsScene();




    QList<QStandardItem*> iList;
    int edge = 0;
    foreach (Edge* e, myParent->edgesIn())
    {
        e->sourceNode()->execute();
        qDebug() << "Rows in: " << model->rowCount();
        if (!edge)
        {
            copyHeaders(model, tempModel);
        }
        while (model->rowCount())
        {
            iList = model->takeRow(0);
            // Kas on vaja eemaldada duplikaadid?
            if (_removeDoubles) {
                // Kui jah, siis esimese sisendi laseme täiega sisse
                if (!edge) {

                    tempModel->appendRow(iList);
                }
                else
                {
                    if (contains(tempModel, iList) == false) {
                        tempModel->appendRow(iList);
                    }
                }
                // Kui ei, siis laseme kõik read läbi
            } else {
                tempModel->appendRow(iList);
            }
        }
        foreach (QGraphicsItem *i, scene->items())
        {
            if (edge == _topLayer)
            {
                i->setZValue(2);
            } else {
                i->setZValue(0);
            }
            tempScene->addItem(i);
        }
        edge++;
    }

    copyHeaders(tempModel, model);
    while (tempModel->rowCount())
    {
        model->appendRow(tempModel->takeRow(0));
    }
    scene->clear();
    foreach (QGraphicsItem *i, tempScene->items())
    {
        scene->addItem(i);
    }

}


bool tOp_append::contains(QStandardItemModel* mod, QList<QStandardItem *> list)
{
    //qDebug() << "Inside contains...";
    QList<QStandardItem *> iL;
    for (int r = 0; r < mod->rowCount(); r++)
    {
        iL = mod->takeRow(r);
        //qDebug() << "iL ok" << iL;
        int c = 0, m = 0;
        if (iL.count() != list.count()) {
            //qDebug() << "Not equal rows";
            mod->insertRow(r, iL);
            return false;
        }
        foreach (QStandardItem* i, iL)
        {
            if (i)
            {
                if (list.at(c)->text() == i->text()) {
                    m++;
                }
            }
            c++;
        }
        mod->insertRow(r, iL);
        if (m == c) {
            //qDebug() << "Inside return true...";
            return true;
        }
    }
    //qDebug() << "Inside return false...";
    return false;
}
