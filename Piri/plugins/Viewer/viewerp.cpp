#include "viewerp.h"
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

void Viewerp::setup()
{
    _enableSorting = true;
    _constSize = false;
    _pointSize = 10;
    _constWidth = true;
    _penWidth = 4;
    _overRide = true;
    _showVerts = false;
}

void Viewerp::description(QString name, QString desc)
{
    setup();
}


QString Viewerp::description()
{
    setup();
    return QString("Viewer/Viewer;Viewer of things./1");
}


void Viewerp::knobs(Knob_Callback *f)
{
    CheckBox_knob(f, &_enableSorting, "Enable table sorting: ");
    CheckBox_knob(f, &_constSize, "Constant point size: ");
    Integer_knob(f, &_pointSize, "Point size: ");
    CheckBox_knob(f, &_constWidth, "Constant pen width: ");
    Integer_knob(f, &_penWidth, "Pen width: ");
    CheckBox_knob(f, &_showVerts, "Show line vertices: ");
}


void Viewerp::engine()
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

    if (_enableSorting)
    {
        QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel;
        proxyModel->setSourceModel( model );
        getViewer()->getTableView()->setModel( proxyModel );
    }
    getViewer()->getTableView()->setSortingEnabled(_enableSorting);
    getViewer()->viewerSettings.sortEnabled = _enableSorting;


    QPen pen(Qt::red, 4);
    foreach(QGraphicsItem *i, scene->items())
    {
        GeoObject *gO = dynamic_cast<GeoObject*>(i);
        GeoPoint *gP = dynamic_cast<GeoPoint*>(i);
        GeoLine *gL = dynamic_cast<GeoLine*>(i);
        //qDebug() << "GeoObject cast O: " << gO;
        //qDebug() << "GeoObject cast P: " << gP;
        //qDebug() << "GeoObject cast L: " << gL;
        if (gO || gP || gL)
        {
            pen = gO->getPen();
            if (_constWidth)
            {
                pen.setCosmetic(true);
                pen.setWidth(_penWidth);
                gO->setPen(pen);
            }
            if (_constSize && gP)
            {
                QPointF p0, p1;
                p0 = getViewer()->get2dView()->mapToScene(0, 0);
                p1 = getViewer()->get2dView()->mapToScene(100, 0);
                float d = p1.x() - p0.x();
                Viewer2dView *v = qobject_cast<Viewer2dView*>(getViewer()->get2dView());
                v->getScaleFactor();
                gP->setSize(_pointSize * v->getScaleFactor());
            }
            if (gL && _showVerts)
            {
                QPen vPen(Qt::blue, 2);
                QBrush vBrush(Qt::white);
                foreach (QVector3D v, gL->getVertices())
                {
                    //GeoPoint *newP = new GeoPoint(scene, v.x(), v.y());
                    //newP->setPen(vPen);
                    //newP->setBrush(vBrush);
                    float rectSize = 4;
                    QRectF rect(-rectSize, -rectSize, rectSize*2, rectSize*2);
                    QGraphicsRectItem *r = new QGraphicsRectItem(rect);
                    scene->addItem(r);
                    r->setPos(v.x(), v.y());
                    r->setPen(vPen);
                    r->setBrush(vBrush);
                }
            }
        }
    }
}
