#include "createpolygons.h"

void CreatePolygons::setup()
{
    _columnX = 0;
    _columnY = 0;
    _columnID = 0;
    inputCount = 1;
    _keepScene = true;
    _myName = "CreatePolygons";
    _myDesc = "Create polygons from table";
}

void CreatePolygons::description(QString name, QString desc)
{
    setup();
}


QString CreatePolygons::description()
{
    setup();
    qDebug() << "Description return" << inputCount;
    return QString("Create/CreatePolygons;Create polygons from points/1");
}

void CreatePolygons::knobs(Knob_Callback *f)
{

    //ComboBox_knob(f, &_columnX, "X Coords: ", "col_x");
    //ComboBox_knob(f, &_columnY, "Y Coords: ", "col_y");
    //ComboBox_knob(f, &_columnID, "Polygon ID: ", "col_id");
    //CheckBox_knob(f, &_useID, "Use ID values: ");
    //CheckBox_knob(f, &_keepScene, "Keep prev. objects: ");

}


void CreatePolygons::engine()
{
    //qDebug() << "CreateLines engine!!!";

    //qDebug() << "CreateLines getscene2d: ";// << Op::getScene2D();
    //qDebug() << "CreateLines gettablemodel: " << getTableModel();

    /*
    QGraphicsScene *scene = getScene2D();
    QStandardItemModel *model = getTableModel();

    qDebug() << "CreateLines engine: " << scene << model;
    if (myParent->edgesIn().count())
    {
        foreach (Edge* e, myParent->edgesIn())
        {
            e->sourceNode()->execute();
        }
    }

    qDebug() << "CreateLines engine!!!";
    if (!_keepScene)
    {
        scene->clear();
    }

    if (model->columnCount() != _columnCount)
    {
        QString cols("-,");
        for (int c = 0; c < model->columnCount(); c++)
        {
            cols += model->horizontalHeaderItem(c)->text() + ",";
        }
        cols.chop(1);
        _columnCount = model->columnCount();

        QWidget *knob = myCallback->getKnob("col_x")->widget;
        UPDATE_VALUES(myCallback, knob, cols);
        knob = myCallback->getKnob("col_y")->widget;
        UPDATE_VALUES(myCallback, knob, cols);
        knob = myCallback->getKnob("col_id")->widget;
        UPDATE_VALUES(myCallback, knob, cols);
    }


    _columnX = validateToCol(model, _columnX);
    _columnY = validateToCol(model, _columnY);
    _columnID = validateToCol(model, _columnID);

    qreal x, y;
    GeoLine *gL = new GeoLine(scene);
    for (int row = 0; row < model->rowCount(); row++)
    {
        QList<QStandardItem*> list = model->takeRow(row);
        x = list.at(_columnX-1)->text().toFloat();
        y = list.at(_columnY-1)->text().toFloat();
        QPen pen(Qt::black, 2);
        QBrush brush(Qt::red);
        gL->addPoint(x, y, 0);
        gL->setupPolygon(0);
        model->insertRow(row, list);
    }

    */
    qDebug() << "CreatePolygons Engine finished!!!";
    //qDebug() << "CreatePoints access viewercontext: " << getViewer()->getContext();

}
