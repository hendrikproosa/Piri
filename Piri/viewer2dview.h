#ifndef VIEWER2DVIEW_H
#define VIEWER2DVIEW_H

#include <QGraphicsView>
#include <QGraphicsView>
#include <QtWidgets>

class Viewer;

class Viewer2dView : public QGraphicsView
{
    Q_OBJECT
public:
    Viewer2dView(QGraphicsScene *scene, Viewer *parent = 0);
    float getScaleFactor() { return myScale; }

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event);
#endif
    void scaleView(qreal scaleFactor);
    QPoint _lastPos;

private:
    Viewer *myParent;
    QGraphicsScene *myScene;
    float myScale;

};

#endif // VIEWER2DVIEW_H
