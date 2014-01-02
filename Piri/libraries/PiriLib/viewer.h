#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QTableView>
#include <QGraphicsView>

#include "mainwindow.h"
#include "viewer2dview.h"
#include "viewer3dview.h"

#include "pirilib.h"

enum context { table, view2d, view3d };

struct settings {
    bool sortEnabled;
};

class PIRILIBSHARED_EXPORT Viewer : public QWidget
{
    Q_OBJECT
public:
    Viewer(QWidget *parent = 0);
    Viewer(MainWindow* mainW);


    MainWindow* getMainWindow();
    QTableView* getTableView();
    QGraphicsView* get2dView();

    context getContext();
    void addWidget2d(QWidget* widget);

    settings viewerSettings;

protected:
    void keyPressEvent(QKeyEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

signals:
    void contextChanged(int c);

private:
    void createTableView();
    void createSceneView();
    void createTabs();

    MainWindow* myMainWindow;
    QTableView* myTableView;
    QGraphicsView* my2dView;
    QGLWidget* my3dView;
    context viewerContext;

};

#endif // VIEWER_H
