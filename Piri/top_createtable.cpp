#include "tableop.h"
#include "top_createtable.h"
#include "node.h"
#include "edge.h"
#include "nodegraph.h"
#include "knobs.h"

#include <QtDebug>
#include <QStandardItemModel>
#include <QFile>
#include <QTableView>

tOp_createTable::tOp_createTable(Node *parent) :
    _file("E:/hendrik/progemine/varia/points.txt"),
    _delimiter(","),
    _colorSpace(0)
{
    myParent = parent;
    description("Input/Read", "Read table");
}


void tOp_createTable::knobs(Knob_Callback *f)
{
    qDebug() << "Got KNOBS: createTable";
    StringKnob *knob = String_knob(f, &_file, "File: ");
    knob->setToolTip("Siia pane faili nimi");
    //knob->setEnabled(false);
    String_knob(f, &_delimiter, "Delimiter: ");
    //ComboBox_knob(f, &_colorSpace, "Colorspace: ");
    //ADD_VALUES(f, "Linear,Log,Gamma corrected");
}


void tOp_createTable::engine()
{
    qDebug() << "Engine CreateTable" << myParent->name();

    //qDebug() << "Myparent->getParent() - " << myParent->getParent();
    QStandardItemModel *model = getTableModel();// myParent->getParent()->getTableData();
    QGraphicsScene *scene = getScene2D();
    model->clear();
    scene->clear();


    qDebug() << "Got data: " << model;

    QFile file(_file);
    qDebug() << "File exists: " << file.exists() << &_file << _file;

    if (file.open(QFile::ReadOnly)) {
        QString line = file.readLine(200);
        QStringList list = line.simplified().split(_delimiter);
        model->setHorizontalHeaderLabels(list);

        int row = 0;
        QStandardItem *newItem = 0;
        while (file.canReadLine()) {
            line = file.readLine(200);
            //qDebug() << line;
            if (!line.startsWith("#") && line.contains(_delimiter)) {
                list = line.simplified().split(_delimiter);
                for (int col = 0; col < list.length(); ++col){
                    if (list.at(col).length() > 0) {
                        newItem = new QStandardItem(list.at(col));
                    } else {
                        newItem = new QStandardItem("");
                    }
                    model->setItem(row, col, newItem);
                }
                row++;
            }
        }
    }
    else
    {
        model->clear();
        scene->clear();
    }
    file.close();


}
