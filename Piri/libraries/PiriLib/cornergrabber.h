#ifndef CORNERGRABBER_H
#define CORNERGRABBER_H

#include <QGraphicsItem>
#include <QPainter>
#include <QColor>

#include "pirilib.h"

class PIRILIBSHARED_EXPORT Cornergrabber : public QGraphicsItem
{
public:
    Cornergrabber(QGraphicsItem *parent = 0,  int corner=0);
    enum { Type = UserType + 10 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setupPos();

protected:


private:
    QGraphicsItem *_myParent;
    qreal   _width;
    qreal   _height;
    int _corner;

    QColor _borderColor;
    QPen _borderPen;

};

#endif // CORNERGRABBER_H
