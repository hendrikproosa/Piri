#include "transform.h"
#include "node.h"
#include "edge.h"
#include "nodegraph.h"
#include "knob_callback.h"
#include "knobs.h"

#include <QtDebug>
#include <QStandardItemModel>
#include <QTableView>
#include <QList>

void Transform::setup()
{
    _transformX = 0;
    _transformY = 0;
    _myName = "Transform";
    _myDesc = "Transform objects.";
}

void Transform::description(QString name, QString desc)
{
    setup();
}


QString Transform::description()
{
    setup();
    return QString("Transform/Transform;Transform objects./1");
}


void Transform::knobs(Knob_Callback *f)
{
    Integer_knob(f, &_transformX, "Transform X: ");
    Integer_knob(f, &_transformY, "Transform Y: ");
}


void Transform::engine()
{
    QStandardItemModel* model = getTableModel();
    QGraphicsScene *scene = getScene2D();

    if (myParent->edgesIn().count())
    {
        foreach (Edge* e, myParent->edgesIn())
        {
            e->sourceNode()->execute();
        }
    }

    foreach(QGraphicsItem *i, scene->items())
    {
    GeoObject *gO = dynamic_cast<GeoObject*>(i);
    GeoPoint *gP = dynamic_cast<GeoPoint*>(i);
    GeoLine *gL = dynamic_cast<GeoLine*>(i);
    if (gP)
        {

        }
    if (gL)
    {
        int p = 0;
        foreach (QVector3D v, gL->getVertices())
        {
            gL->updatePoint(_transformX, _transformY, 0, p);
            p++;
        }
    }
    }
}
