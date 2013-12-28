#ifndef BACKDROP_H
#define BACKDROP_H

#include <QGraphicsItem>
#include <QList>

class nodeGraph;
class Cornergrabber;

class Backdrop : public QGraphicsItem
{
public:
    Backdrop(nodeGraph *nodeGraph, QString name);
    ~Backdrop();
    enum { Type = UserType + 3 };
    int type() const { return Type; }
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setName(QString name);
    QString getName();

    nodeGraph* getParent();

    void setItems();
    QList<QGraphicsItem*> getItems();

    QRectF getBounds();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    QRectF bRect;
    nodeGraph *graph;
    QString myName;

    QList<QGraphicsItem*> itemList;

    QPointF lastPos;
    Cornergrabber* _corners[4];

};

#endif // BACKDROP_H
