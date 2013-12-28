#include "backdrop.h"
#include "node.h"
#include "nodegraph.h"
#include "cornergrabber.h"

#include <math.h>

#include <QPainter>
#include <QtDebug>
#include <QKeyEvent>
#include <QtWidgets>

Backdrop::Backdrop(nodeGraph *nodeGraph, QString name)
    : graph(nodeGraph)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsFocusable);
    setFlag(ItemIsSelectable);
    setZValue(-1);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setAcceptHoverEvents(true);
    qDebug() << acceptHoverEvents();
    //setFocus(Qt::OtherFocusReason);
    //setAcceptedMouseButtons(0);
    bRect.setCoords(-100, -50, 100, 50);
    setName(name);
    lastPos = pos();
    itemList.clear();

    qDebug() << "bd seleced? " << isSelected();
    /* Siin on näide sellest, kuidas lisada widgeteid stseeni proxyobjektid abil

    QPushButton *button = new QPushButton(QString("Evaluate"));
    QGraphicsProxyWidget *proxy = graph->scene()->addWidget(button);
    */
}


Backdrop::~Backdrop()
{
    qDebug() << "Backdrop destructor";
}


nodeGraph* Backdrop::getParent()
{
    return graph;
}


void Backdrop::setName(QString name)
{
    myName = name;
}


QString Backdrop::getName()
{
    return myName;
}


void Backdrop::setItems()
{
    itemList.clear();
    QList<QGraphicsItem*> list;
    list = graph->scene()->items(getBounds(), Qt::ContainsItemShape);
    foreach (QGraphicsItem* g, list) {
        if (qgraphicsitem_cast<Node*>(g))
            itemList.append(g);
    }
}


QList<QGraphicsItem*> Backdrop::getItems()
{
    setItems();
    return itemList;
}


QRectF Backdrop::getBounds()
{
    QRectF r;
    r.setTopLeft(mapToScene(boundingRect().topLeft()));
    r.setBottomRight(mapToScene(boundingRect().bottomRight()));
    return r;
}

QRectF Backdrop::boundingRect() const
{
    return bRect;
}


QVariant Backdrop::itemChange(GraphicsItemChange change, const QVariant &value)
{
    //qDebug() << "Itemchange: " << change;
    QPointF delta = pos() - lastPos;
    switch (change) {
    case ItemPositionHasChanged:
        //qDebug() << "I moved!";
        //getItems();
        if (itemList.count() > 0)
            {
            foreach (QGraphicsItem *g, itemList)
            {
                g->moveBy(delta.x(), delta.y());
                graph->itemMoved();

            }
        }
        lastPos = pos();
        break;
    default:
        break;
    };
    return QGraphicsItem::itemChange(change, value);
}


void Backdrop::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //qDebug() << "Hover enter";
    //_corners[0] = new Cornergrabber(this, 0);
    //_corners[1] = new Cornergrabber(this, 1);
    _corners[2] = new Cornergrabber(this, 2);
    //_corners[3] = new Cornergrabber(this, 3);
}


void Backdrop::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    //qDebug() << "Hover leave";
    //_corners[0]->setParentItem(NULL);
    //_corners[1]->setParentItem(NULL);
    _corners[2]->setParentItem(NULL);
    //_corners[3]->setParentItem(NULL);

    //delete _corners[0];
    //delete _corners[1];
    delete _corners[2];
    //delete _corners[3];
}


void Backdrop::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor bgColor(QColor::fromRgbF(0.15, 0.8, 1.0, 0.5));
    QPen pen(bgColor, 2, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
    QPen selectedPen(QColor::fromRgbF(0.8, 0.6, 0.2, 1), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen linePen(bgColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
    QPen fontPen(QColor::fromRgbF(0.2, 0.2, 0.2), 1, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
    QBrush brush(bgColor, Qt::SolidPattern);


    // Box
    painter->setPen(pen);
    if (isSelected())
        painter->setPen(selectedPen);
    painter->setBrush(brush);
    painter->drawRect(boundingRect());
    painter->setPen(linePen);
    painter->drawLine(QPointF(boundingRect().topLeft() + QPointF(1, 16)), QPointF(boundingRect().topRight() + QPointF(-1, 16)));

    // Resizecorner
    /*
    painter->setBrush(QBrush(bgColor, Qt::Dense4Pattern));
    painter->drawLine(boundingRect().bottomRight() + QPointF(-15, 2), boundingRect().bottomRight() + QPointF(2, -15));
    painter->drawLine(boundingRect().bottomRight() + QPointF(-11, 2), boundingRect().bottomRight() + QPointF(2, -11));
    painter->drawLine(boundingRect().bottomRight() + QPointF(-7, 2), boundingRect().bottomRight() + QPointF(2, -7));
    */

    // Name
    QFont nameFont("Verdana", 8, QFont::Normal);
    painter->setPen(fontPen);
    painter->setFont(nameFont);
    painter->drawText(QRectF(boundingRect().topLeft() + QPointF(3, 1), QSizeF(120, 30)), Qt::AlignLeft << Qt::AlignTop, myName);
}
