#include "viewer2dview.h"
#include "viewer.h"

Viewer2dView::Viewer2dView(QGraphicsScene *scene, Viewer *parent) : QGraphicsView(parent),
    myScene(scene),
    myParent(parent)
{
    setScene(scene);
    //setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setRenderHint(QPainter::Antialiasing, false);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
}


void Viewer2dView::mousePressEvent(QMouseEvent *event)
{
    _lastPos = event->pos();
    if (event->buttons().testFlag(Qt::MiddleButton))
    {
        return;
    }
    QGraphicsView::mousePressEvent(event);
}


void Viewer2dView::mouseMoveEvent(QMouseEvent *event)
{
    QPoint curPos;
    int deltax, deltay;
    curPos = event->pos();
    deltax = _lastPos.x() - curPos.x();
    deltay = _lastPos.y() - curPos.y();

    if (event->buttons().testFlag(Qt::MiddleButton))
    {
        QScrollBar* myHScrollbar;
        myHScrollbar = this->horizontalScrollBar();
        myHScrollbar->setValue(myHScrollbar->value() + deltax);

        QScrollBar* myVScrollbar;
        myVScrollbar = this->verticalScrollBar();
        myVScrollbar->setValue(myVScrollbar->value() + deltay);
        _lastPos = curPos;
        return;
    }
    _lastPos = curPos;
    QGraphicsView::mouseMoveEvent(event);
}


void Viewer2dView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
}


void Viewer2dView::wheelEvent(QWheelEvent *event)
{
    QPointF s = mapToScene(_lastPos);
    QPointF e, d;

    scaleView(pow((double)2, event->delta() / 500.0));

    e = mapToScene(_lastPos);
    s = mapFromScene(s);
    e = mapFromScene(e);
    d = s - e;

    int deltax, deltay;
    deltax = d.x();
    deltay = d.y();

    QScrollBar* myHScrollbar;
    myHScrollbar = this->horizontalScrollBar();
    myHScrollbar->setValue(myHScrollbar->value() + deltax);

    QScrollBar* myVScrollbar;
    myVScrollbar = this->verticalScrollBar();
    myVScrollbar->setValue(myVScrollbar->value() + deltay);

}


void Viewer2dView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    //if (factor < 0.1)
    //    return;
    scale(scaleFactor, scaleFactor);
    myScale = factor;
    //qDebug() << "View factor: " << myScale;
}
