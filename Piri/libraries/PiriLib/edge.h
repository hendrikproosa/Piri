#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

#include "pirilib.h"

class Node;

//! [0]
class PIRILIBSHARED_EXPORT Edge : public QGraphicsItem
{
public:
    Edge(Node *sourceNode, Node *destNode, int eType);

    Node *sourceNode() const;
    Node *destNode() const;
    QPainterPath shape() const;

    void adjust();
    void disconnect();
    void test();

    enum { Type = UserType + 2 };
    int type() const { return Type; }

    int edgeType; //0 - viewer, 1 - base, 2 - input, 3 - output, 4 - mask
    void setType(int eT);
    int getType() const { return edgeType; }

    QPointF getSourcePoint();
    void setSourcePoint(QPointF point);
    void setSourceNode(Node *node);
    //void setDestNode(Node *node);
    void setDragged(int drag);
    int hovered = 0;

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    Node *source, *dest;
    int dragged = 0;
    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;

};
//! [0]

#endif // EDGE_H
