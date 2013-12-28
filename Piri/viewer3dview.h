#ifndef VIEWER3DVIEW_H
#define VIEWER3DVIEW_H

#include <QtWidgets>
#include <QtOpenGL>

#include <math.h>

#include "qtlogo.h"
#include "viewer.h"

class Viewer3dView : public QGLWidget
{
    Q_OBJECT
public:
    Viewer3dView(QGraphicsScene* scene, QWidget *parent = 0);
    ~Viewer3dView();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    
signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    
private:
    QGraphicsScene *myScene;
    QWidget *myParent;
    QtLogo *logo;
    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
    QColor qtGreen;
    QColor qtPurple;
};

#endif // VIEWER3DVIEW_H
