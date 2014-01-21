#ifndef NODEGRAPH_H
#define NODEGRAPH_H

#include <QGraphicsView>
#include <QtWidgets>
#include <QStandardItemModel>
#include "interfaces.h"

#include "pirilib.h"

class Node;
class Edge;
class MainWindow;

//! [0]
class PIRILIBSHARED_EXPORT nodeGraph : public QGraphicsView
{
    Q_OBJECT

public:
    nodeGraph(QGraphicsScene *scene, MainWindow *parent = 0);
    enum Mode { Pan, Select, InsertEdge, InsertNode, breakEdge };
    void addScene(QGraphicsScene *scene);
    void itemMoved();
    void evaluate();
    void execute();
    QList<Node *> evaluateNode(Node* node);
    void pushNodeEdgesToStack(Node *node);
    QString debugStack(QList<Node *> stack);
    int nodeVisited(Node* node);
    QList<Node *> reverseStack(QList<Node *> stack);
    void removeEdge(Edge *edge);
    void removeNode(Node *node);
    int inListNode(QList<Node *> list, Node *node);
    Edge* getActiveEdge();
    void pushItem(QGraphicsItem *item, QPointF pos);
    void pushNode(Node *node);
    void pushNode(Node *node, QPointF pos);
    Edge* addEdge(Edge *edge);
    int getMode();
    QStandardItemModel* getTableData();
    MainWindow* getParent();

    void connectViewer(Node* node, int socket);
    Node* getActiveViewer();
    void setActiveViewer(Node* node);

    void disableSelected();

    Node* getSelectedNode();

public slots:
    void zoomIn();
    void zoomOut();
    void addNode();
    void addOp(OpInterface *Op);

signals:
    void evaluated(QString evalList);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event);
#endif
    void contextMenuEvent(QContextMenuEvent *event);
    void scaleView(qreal scaleFactor);
    void centerView();

    QPoint _lastPos; /*!< Mouse position at last event. */
    QPointF _contextMenuPos; /*!< Position of contextMenuEvent. New node will be added here. */
    Node* _contextSelectedNode; /*!< Node selected at the time of contextMenuEvent. */

private:

    MainWindow *myParent; /*!< Nodegraph parent object. */
    QGraphicsScene *myScene; /*!< Nodegraph scene that holds graphical representation of graph. */

    QList<Node *> nodeList; /*!< List of all nodes in nodegraph. */
    QList<Node *> nodeStack; /*!< List of nodes. */
    QList<Node *> evalStack; /*!< List of nodes sorted by execution order. */
    QList<Node *> visitStack; /*!< List of nodes visited. */
    QGraphicsLineItem *line; /*!< Graphical representation of line. Used in edge dragging? */
    QGraphicsRectItem *selectRect; /*!< Graphical representation of selection rectangle. */
    QGraphicsItem *selectedItem; /*!< Current selected item. */
    Mode myMode; /*!< Nodegraph current mode. Used in mouse interactions. */
    Edge *activeEdge; /*!< Active edge being dragged. */
    int onNode; /*!< Is mouse on node?. */

    Node *activeViewer; /*!< Node that is set as active viewer. Graph evaluation starts from this node. */

    void createActions();
    QAction *newAct;
    QAction *newActSort;
    QAction *newActSQL;
    QAction *newActM;
    QAction *newActGeo01;
    QAction *newActR;
    QAction *newActV;
    QAction *newBackDrop;

    QStandardItemModel *tableData; /*!< Table data model. */


};

#endif // NODEGRAPH_H
