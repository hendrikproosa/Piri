#include "cornergrabber.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QtDebug>

Cornergrabber::Cornergrabber(QGraphicsItem *parent, int corner) :
    _myParent(parent),
    _corner(corner),
    _width(20),
    _height(20),
    _borderColor(Qt::yellow),
    _borderPen(QColor::fromRgbF(0.8, 0.6, 0.2, 1), 1)
{

    setZValue(5);
    setParentItem(parent);
    setupPos();
}


void Cornergrabber::setupPos()
{
    QPointF tp;
    switch (_corner) {
    case 0:
        tp = _myParent->boundingRect().topLeft();
        break;
    case 1:
        tp = _myParent->boundingRect().topRight();
        break;
    case 2:
        tp = _myParent->boundingRect().bottomRight();
        break;
    case 3:
        tp = _myParent->boundingRect().bottomLeft();
        break;
    default:
        tp = _myParent->boundingRect().bottomRight();
        break;
    }
    setPos(tp);
}

QRectF Cornergrabber::boundingRect() const
{
    QRectF bRect;
    qreal e, w, h;
    w = _width/2 + e;
    h = _height/2 + e;
    bRect.setCoords(-w, -h, w, h);
    return bRect;
}


QPainterPath Cornergrabber::shape() const
{
    QPainterPath path;
    qreal e, w, h;
    w = _width/2 + e;
    h = _height/2 + e;
    path.addRect(-w, -h, w, h);
    return path;
}


void Cornergrabber::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QBrush brush(_borderColor);
    painter->setBrush(brush);
    painter->setPen(_borderPen);

    painter->drawPath(shape());
}
