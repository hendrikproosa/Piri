#include "edge.h"
#include "node.h"
#include "nodegraph.h"

#include <math.h>

#include <QPainter>
#include <QtDebug>
#include <QKeyEvent>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;


Edge::Edge(Node *sourceNode, Node *destNode, int eType)
    : arrowSize(10)
{
    setFlag(ItemIsFocusable);
    setFlag(ItemIsMovable, false);
    setFocus(Qt::OtherFocusReason);
    setAcceptedMouseButtons(0);
    source = sourceNode;
    dest = destNode;
    edgeType = eType;
    if (source) source->addEdge(this, 0);
    dest->addEdge(this, 0);
    adjust();
}


Node *Edge::sourceNode() const
{
    return source;
}

Node *Edge::destNode() const
{
    return dest;
}

void Edge::setDragged(int drag)
{
    dragged = drag;
}


void Edge::adjust()
{
    if (!source && !dest)
        return;

    if (!source) {
        QPointF ss, of;
        QPointF ds(mapFromItem(dest, 0, 0));
        switch (edgeType) {
        case 0:
            of.setX(+30);
            of.setY(-40);
            break;
        case 1:
            of.setX(-30);
            of.setY(-40);
            break;
        case 2:
            of.setX(+30);
            of.setY(-40);
            break;
        case 3:
            of.setX(-30);
            of.setY(-40);
            break;
        case 4:
            of.setX(-60);
            of.setY(0);
            break;
        default:
            break;
        }
        ss.setX(ds.x() + of.x());
        ss.setY(ds.y() + of.y());
        prepareGeometryChange();
        if (!dragged) sourcePoint = ss;
        destPoint = ds;
    }
    else if (!dest)
    {
        QPointF ss(mapFromItem(source, 0, 0));
        QPointF ds;
        ds.setX(ss.x());
        ds.setY(ss.y() + 30);
        prepareGeometryChange();
        sourcePoint = ss;
        destPoint = ds;
    }
    else
    {
        QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
        qreal length = line.length();

        prepareGeometryChange();

        if (length > qreal(20.)) {
            sourcePoint = line.p1();
            destPoint = line.p2();
        } else {
            sourcePoint = destPoint = line.p1();
        }
    }
    update(boundingRect());
}

void test()
{
    qDebug() << "Edgetest vastus on 42";
}

void Edge::disconnect()
{
    //qDebug() << "Disconnect: start";
    switch (edgeType) {
        case (0):
            qDebug() << "Edge::Disconnect: viewer" << source->name();
            source->removeEdge(this);
            source = 0;
            qDebug() << "Disconnect: viewer ok";
            break;
        case (1):
            qDebug() << "Disconnect: base" << source->name();
            source->removeEdge(this);
            source = 0;
            qDebug() << "Disconnect: base ok";
            break;
        case (2):
            qDebug() << "Disconnect: input" << source->name();
            source->removeEdge(this);
            source = 0;
            qDebug() << "Disconnect: input ok";
            break;
        case (3):
            qDebug() << "Disconnect: output" << source->name();
            source->removeEdge(this);
            source = 0;
            //dest->removeEdge(this);
            //dest = 0;
            qDebug() << "Disconnect: output ok";
            break;
        case (4):
            qDebug() << "Disconnect: mask" << source->name();
            source->removeEdge(this);
            source = 0;
            qDebug() << "Disconnect: mask ok";
            break;
        default:
        //qDebug() << "Disconnect: default"
        ;
    }
    qDebug() << "Disconnect ok!";
    adjust();
    //qDebug() << "Adjust ok!";
}


void Edge::setType(int eT)
{
    edgeType = eT;
}


QPointF Edge::getSourcePoint()
{
    return sourcePoint;
}

void Edge::setSourcePoint(QPointF point)
{
    sourcePoint = point;
}

void Edge::setSourceNode(Node *node)
{
    source = node;
    if (node)
        node->addEdge(this, 0);
    adjust();
}


QRectF Edge::boundingRect() const
{
    if (!source && !dest)
        return QRectF();

    qreal penWidth = 8;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);

}

QPainterPath Edge::shape() const
{
    QPainterPath path;
    QPolygon shapePolygon;

    int points[] = {sourcePoint.x(), sourcePoint.y(), destPoint.x(), destPoint.y()};
    shapePolygon.setPoints(2, points);
    path.addPolygon(shapePolygon);

    QPainterPathStroker stroker;
    stroker.setWidth(10);
    stroker.setJoinStyle(Qt::MiterJoin);
    QPainterPath newpath = (stroker.createStroke(path) + path).simplified();

    return newpath;
}


void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    adjust();
    if (!source && !dest)
        return;

    QPointF p2;
    QPointF intersectPoint, oldDest;
    QLineF polyLine;
    QLineF centerLine;
    QPolygonF endPolygon;
    QPointF p1;
    QLineF line;

    if (destNode()->getType() == 0) {setType(0);}
    //if (destNode()->getType() != 2) {setType(2);}

    if (source && dest) {

        if (sourceNode()->collidesWithItem(destNode()))
            return;

        line.setP1(sourcePoint);
        line.setP2(destPoint);
        if (qFuzzyCompare(line.length(), qreal(0.)))
            return;

        // Joone joonistamine ja lõikepunkti leidmine sõlme kujuga
        centerLine.setPoints(sourceNode()->pos(), destNode()->pos());
        QRectF br = destNode()->boundingRect();
        QPolygonF bp = destNode()->shape().toFillPolygon();
        br.adjust(0, 0, 0, -5);
        endPolygon = bp;
        //endPolygon.moveBottom(-20);
        p1 = endPolygon.first() + destNode()->pos();
        for (int i = 1; i < endPolygon.count(); ++i) {
            p2 = endPolygon.at(i) + destNode()->pos();
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType =
                polyLine.intersect(centerLine, &intersectPoint);
            if (intersectType == QLineF::BoundedIntersection)
                break;
                p1 = p2;
        }
    }
    else
    {
        QPointF start = sourcePoint;
        line.setP1(start);
        line.setP2(destPoint);

        centerLine.setPoints(start, destNode()->pos());
        //endPolygon = destNode()->boundingRect();
        QRectF br = destNode()->boundingRect();
        QPolygonF bp = destNode()->shape().toFillPolygon();
        br.adjust(0, 1, 0, -5);
        endPolygon = bp;
        p1 = endPolygon.first() + destNode()->pos();
        for (int i = 1; i < endPolygon.count(); ++i) {
            p2 = endPolygon.at(i) + destNode()->pos();
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType =
                polyLine.intersect(centerLine, &intersectPoint);
            if (intersectType == QLineF::BoundedIntersection)
                break;
                p1 = p2;
        }
        //sourcePoint = start;
        //qDebug() << sourcePoint << destPoint;
    }

    QPen viewerPen(Qt::yellow, 2, Qt::DotLine, Qt::SquareCap, Qt::MiterJoin);
    QBrush viewerBrush(Qt::yellow);
    QPen inputPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    QBrush inputBrush(Qt::gray);
    QPen selectedPen(QColor::fromRgbF(0.8, 0.6, 0.2, 1), 3, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    QBrush selectedBrush(QColor::fromRgbF(0.8, 0.6, 0.2, 1));

    switch (edgeType) {
    case 0:
        painter->setPen(viewerPen);
        painter->setBrush(viewerBrush);
        break;
    case 1:
        painter->setPen(inputPen);
        painter->setBrush(inputBrush);
        break;
    case 2:
        painter->setPen(inputPen);
        painter->setBrush(inputBrush);
        break;
    case 3:
        painter->setPen(inputPen);
        painter->setBrush(inputBrush);
        break;
    case 4:
        painter->setPen(inputPen);
        painter->setBrush(inputBrush);
        break;
    default:
        painter->setPen(inputPen);
        painter->setBrush(inputBrush);
    }

    line.setP1(sourcePoint);
    if (dest) {
        //line.setP2(intersectPoint);
        destPoint = intersectPoint;
    }
    else {
        //line.setP2(destPoint);
    }
    line.setP2(destPoint);

    if (dest->isSelected() && edgeType != 0) {
        painter->setPen(selectedPen);
        painter->setBrush(selectedBrush);
    }

    if (hovered) {
        painter->setPen(selectedPen);
        painter->setBrush(selectedBrush);
    }

    painter->drawLine(line);

    // Kui Control klahv on all ja serval on algus, siis joonistame poolitusmummu
    if (dest->getParent()->getMode() == 4 && source != 0)
    {
        QPen p = painter->pen();
        painter->setPen(selectedPen);
        painter->drawEllipse((destPoint - sourcePoint)/2 + sourcePoint, 5, 5);
        painter->setPen(p);
    }

    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;
    oldDest = destPoint;
    destPoint = QPointF(line.x2(), line.y2());

    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                              cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                              cos(angle - Pi + Pi / 3) * arrowSize);

    painter->setPen(inputPen);

    if (edgeType == 0) {
        viewerPen.setStyle(Qt::SolidLine);
        painter->setPen(viewerPen);
    }

    if (dest->isSelected() && edgeType != 0) {
        painter->setPen(selectedPen);
        painter->setBrush(selectedBrush);
    }

    if (hovered) {
        painter->setPen(selectedPen);
        painter->setBrush(selectedBrush);
    }

    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);

    painter->setPen(QPen(Qt::white, 0));
    QFont boldFont("Verdana", 8, QFont::Bold);
    QFont normalFont("Verdana", 8, QFont::Normal);
    painter->setFont(normalFont);
    QPointF textPos;
    textPos.setX(intersectPoint.x() + (intersectPoint.x() - oldDest.x())/4 - 10);
    textPos.setY(intersectPoint.y() + (intersectPoint.y() - oldDest.y())/4);

    QString t = "";
    switch (edgeType) {
    case 0:
        t = "1";
        //painter->drawText(textPos, QString("1"));
        break;
    case 1:
        t = "B";
        //painter->drawText(textPos, QString("B"));
        break;
    case 2:
        if (destNode()->getMaxInputs() > 1)
        {
            t = "A";
            //painter->drawText(textPos, QString("A"));
        }
        else
        {
            t = "A";
            //painter->drawText(textPos, QString());
        }
        break;
    case 3:
        t = "";
        //painter->drawText(textPos, QString(""));
        break;
    case 4:
        t = "mask";
        //painter->drawText(textPos, QString("mask"));
        break;
    default:
        break;
    }

    QPen tp = painter->pen();
    painter->setPen(QPen(Qt::black, 0));
    painter->drawText(textPos + QPointF(0.3, 0.3), t);
    painter->setPen(tp);
    painter->drawText(textPos, t);


    //painter->drawRect(boundingRect());
    //painter->setBrush(Qt::blue);
    //painter->drawPath(shape());
}
