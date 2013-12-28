#include "geo2dop.h"
#include "geo2d_createpoints.h"
#include "knobs.h"
#include "knob_callback.h"

#include "QGraphicsView"

geo2d_createPoints::geo2d_createPoints(Node *parent) : Geo2dOp(parent),
    _columnX(1),
    _columnY(2),
    _columnLabels(3),
    _columnCount(0),
    _count(10),
    _size(20),
    _showLabels(true),
    _keepScene(true)
{
    myParent = parent;
    description("Geo2D/CreatePoints", "Create points based on coordinates.<br>Choose to keep or delete<br>previous objects.");
}


void geo2d_createPoints::knobs(Knob_Callback *f)
{
    ComboBox_knob(f, &_columnX, "Column X: ", "col_x");
    ComboBox_knob(f, &_columnY, "Column Y:", "col_y");
    ComboBox_knob(f, &_columnLabels, "Labels column: ", "col_labels");
    Integer_knob(f, &_size, "Size: ");
    CheckBox_knob(f, &_showLabels, "Show labels: ");
    CheckBox_knob(f, &_keepScene, "Keep prev. objects: ");

}


void geo2d_createPoints::engine()
{
    QGraphicsScene *scene = getScene2D();
    QStandardItemModel *model = getTableModel();
    //scene->clear();

    if (myParent->edgesIn().count())
    {
        foreach (Edge* e, myParent->edgesIn())
        {
            e->sourceNode()->execute();
        }
    }

    if (!_keepScene)
    {
        scene->clear();
    }

    int cx = -1, cy = -1;
    if (model->columnCount() != _columnCount)
    {

        QString cols("-,");
        for (int c = 0; c < model->columnCount(); c++)
        {
            cols += model->horizontalHeaderItem(c)->text() + ",";
            // Otsime võimalusel X ja Y veerud automaatselt
            if (cols.contains("X", Qt::CaseInsensitive) && cx < 0)
            {
                cx = c + 1;
                qDebug() << "X veerg: " << cx;
            }
            if (cols.contains("Y", Qt::CaseInsensitive) && cy < 0)
            {
                cy = c + 1;
                qDebug() << "Y veerg: " << cy;
            }
        }
        cols.chop(1);

        if (cx > -1)
            _columnX = cx;
        if (cy > -1)
            _columnY = cy;
        _columnCount = model->columnCount();
        QWidget *knob = myCallback->getKnob("col_labels")->widget;
        UPDATE_VALUES(myCallback, knob, cols);
        knob = myCallback->getKnob("col_x")->widget;
        UPDATE_VALUES(myCallback, knob, cols);
        UPDATE(myCallback, knob, QString(_columnX));
        knob = myCallback->getKnob("col_y")->widget;
        UPDATE_VALUES(myCallback, knob, cols);
        UPDATE(myCallback, knob, QString(_columnY));
    }

    _columnX = validateToCol(model, _columnX);
    _columnY = validateToCol(model, _columnY);
    _columnLabels = validateToCol(model, _columnLabels);

    qreal x, y;
    QString label;
    for (int row = 0; row < model->rowCount(); row++)
    {
        QList<QStandardItem*> list = model->takeRow(row);
        x = list.at(_columnX-1)->text().toFloat();
        y = list.at(_columnY-1)->text().toFloat();
        label = list.at(_columnLabels-1)->text();
        QPen pen(Qt::black, 2);
        QBrush brush(Qt::red);
        QGraphicsEllipseItem *point = new QGraphicsEllipseItem(-_size, -_size, _size*2, _size*2);
        point->setPos(x, y);
        point->setZValue(-1);
        point->setPen(pen);
        point->setBrush(brush);
        //point.setFlag(QGraphicsItem::ItemIsMovable);
        point->setFlag(QGraphicsItem::ItemIsSelectable);
        point->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
        //point->setFlag(QGraphicsItem::ItemIsMovable);
        //scene->addItem(point);
        GeoPoint *gP = new GeoPoint(scene, x, y);
        gP->setSize(_size);
        //GeoLine *gL = new GeoLine(scene, x, y);
        //gL->setFlag(QGraphicsItem::ItemIsMovable);
        if (_showLabels)
        {
            scene->addText(label, QFont("Arial", 15))->setPos(x + _size - 5, y - _size - 15);
        }
        model->insertRow(row, list);
    }

    qDebug() << "CreatePoints access viewercontext: " << getViewer()->getContext();

    /*
    Test kuidas saada nuppe viewerisse
    QLineEdit *but = new QLineEdit("UHUU!");
    but->move(10, 10);
    QPushButton *b2 = new QPushButton("Edit");
    b2->move(0, 40);
    b2->setMinimumWidth(50);
    getViewer()->addWidget2d(but);
    getViewer()->addWidget2d(b2);
    */

}
