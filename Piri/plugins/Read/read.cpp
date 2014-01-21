#include "read.h"
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

void Read::setup()
{
    _file = "";
    _delimiter = ",";
    _colorSpace = 0;
    _myName = "Read";
    _myDesc = "Read data table.";
    _headers = true;
}

void Read::description(QString name, QString desc)
{
    setup();
}


QString Read::description()
{
    setup();
    return QString("Input/Read;Read data table./0");
}


void Read::knobs(Knob_Callback *f)
{
    FileDialogKnob* knob = FileDialog_knob(f, &_file, "Ava fail: ");
    knob->setToolTip("Siia pane faili nimed");
    String_knob(f, &_delimiter, "Delimiter: ");
    CheckBox_knob(f, &_headers, "First line is headers: ");
    //ComboBox_knob(f, &_colorSpace, "Colorspace: ");
    //ADD_VALUES(f, "Linear,Log,Gamma corrected");
}


void Read::engine()
{
    QElapsedTimer timer;
    timer.start();

    qDebug() << "Engine CreateTable" << myParent->name();

    //qDebug() << "Myparent->getParent() - " << myParent->getParent();
    QStandardItemModel *model = getTableModel();// myParent->getParent()->getTableData();
    QGraphicsScene *scene = getScene2D();
    model->clear();
    scene->clear();


    qDebug() << "Got data: " << model;

    QFile file(_file);
    //qDebug() << "File exists: " << file.exists() << &_file2 << _file2;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        int row = 0;
        QStandardItem *newItem = 0;
        if (!in.atEnd() && _headers)
        {
            QString line = in.readLine();
            QStringList list = line.simplified().split(_delimiter);
            model->setHorizontalHeaderLabels(list);
        }
        while ( !in.atEnd() )
        {
           QString line = in.readLine();
           QStringList list = line.simplified().split(_delimiter);
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
        qDebug() << "Lugesin ridu: " << row;

    }
    else
    {
        model->clear();
        scene->clear();
    }
    file.close();

    qDebug() << "Read Node took: " << timer.elapsed() << "milliseconds";

}
