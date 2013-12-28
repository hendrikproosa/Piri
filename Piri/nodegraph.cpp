#include "nodegraph.h"
#include "edge.h"
#include "node.h"
#include "backdrop.h"
#include "mainwindow.h"
#include "tableop.h"
#include "viewer.h"
#include "knob_callback.h"

#include <math.h>

#include <QKeyEvent>
#include <QtDebug>
#include <QCryptographicHash>
#include <QAction>
#include <QtWidgets>
#include <QMainWindow>
#include <QGraphicsObject>
#include <QStandardItemModel>
#include <QTableView>

//! [0]
nodeGraph::nodeGraph(QGraphicsScene *scene, MainWindow *parent)
    : QGraphicsView(parent),
      myParent(parent),
      myScene(scene),
      onNode(0),
      selectRect(0),
      myMode(Pan)
{

    addScene(myScene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContextMenuPolicy(Qt::DefaultContextMenu);

    scale(qreal(0.8), qreal(0.8));

    createActions();

    Node *node = new Node(this, "Viewer", 0);
    pushNode(node, QPointF(-50, 150));
    activeViewer = node;
    node->getOp()->getCallback()->hide();

}


void nodeGraph::addScene(QGraphicsScene *scene)
{
    setScene(scene);
}


void nodeGraph::evaluate()
{

    Node *vNode, *curNode;
    //qDebug() << "Nodes: " << nodeList.length();

    evalStack.clear();
    visitStack.clear();

    foreach (Node *node, nodeList) {
        qDebug() << "Name: " << node->name() << "Type: " << node->getType() << "Edges: " << node->edges().length();
    }


    qDebug() << "nodes in scene: " << nodeList.count();
    if (nodeList.isEmpty()) { return; }

    /*
    foreach (Node *node, nodeList) {
        if (node->getType() == 0) {
            vNode = node;
            nodeStack << vNode;
            curNode = node;
        }
    }
    */

    if (activeViewer == 0 || activeViewer->edgesIn().isEmpty()) { return; }

    nodeStack << activeViewer;
    curNode = activeViewer;
    evalStack << activeViewer;
    //qDebug() << "Viewer: " << activeViewer->name();

    while (nodeStack.length()) {
        if (curNode->getType() != 0) {
            evalStack << curNode;
        }
        //qDebug() << "CurNode: " << curNode->name();
        nodeStack.removeLast();
        pushNodeEdgesToStack(curNode);
        //debugStack(nodeStack);
        if (!nodeStack.length()) {break;}
        curNode = nodeStack.last();
    }
    foreach (Node *node, reverseStack(evalStack)) {
        if (!visitStack.contains(node) && node->getType() != 99)
        {
            visitStack << node;
        }

    }
    evalStack = visitStack;
    getParent()->getViewer()->getTableView()->setUpdatesEnabled(false);
    execute();
    getParent()->getViewer()->getTableView()->setUpdatesEnabled(true);
    //debugStack(evalStack);

    QString blah = QString(QCryptographicHash::hash(("myPassword"),QCryptographicHash::Md5).toHex());
    //qDebug() << blah;
    emit evaluated(debugStack(evalStack));
}


void nodeGraph::execute()
{

    getTableData()->clear();
    getParent()->getScene2D()->clear();

    qDebug() << "Execute";
    if (evalStack.count())
    {
        evalStack.last()->execute();
    }
    //if (activeViewer)
    //    activeViewer->execute();
}

QList<Node *> nodeGraph::evaluateNode(Node *node)
{
    Node *vNode, *curNode;

    evalStack.clear();
    visitStack.clear();

    vNode = node;
    nodeStack << vNode;
    curNode = node;

    qDebug() << "Viewer: " << vNode->name();
    while (nodeStack.length()) {
        evalStack << curNode;
        qDebug() << "CurNode: " << curNode->name();
        if (!nodeStack.isEmpty()) {nodeStack.removeLast();}
        pushNodeEdgesToStack(curNode);
        //debugStack(nodeStack);
        if (nodeStack.isEmpty()) {break;}
        curNode = nodeStack.last();
    }
    foreach (Node *node, reverseStack(evalStack)) {
        if (!nodeVisited(node))
        {
            visitStack << node;
            //qDebug() << node->name();
        }

    }
    evalStack = visitStack;
    return evalStack;
}

void nodeGraph::pushNodeEdgesToStack(Node *node)
{
    if (!node->edgesIn().isEmpty())
    {
        foreach (Edge* edge, node->edgesIn()) {
            nodeStack << edge->sourceNode();
            //qDebug() << "Pushed: " << nodeStack.last()->name();
        }
    }

}

QString nodeGraph::debugStack(QList<Node *> stack)
{
    QString evalList;
    if (stack.length() > 0) {
        //qDebug() << "Debugging stack:";
        foreach (Node *node, stack) {
            evalList += QString(node->name()) + QString("\n");
        }
    }
    else {
    qDebug() << "Stack on tühi!";
    }
    //qDebug() << evalList;
    return evalList;

}

QList<Node *> nodeGraph::reverseStack(QList<Node *> stack)
{
    QList<Node *> tempStack;
    while (stack.length())
    {
        tempStack << stack.takeLast();
    }
    return tempStack;
}

int nodeGraph::nodeVisited(Node *node)
{
    foreach (Node *vnode, visitStack) {
        if (vnode == node) {return 1;}
    }
    return 0;
}

int nodeGraph::inListNode(QList<Node *> list, Node *node)
{
    foreach (Node *n, list) {
        if (n == node) {return 1;}
    }
    return 0;
}

void nodeGraph::itemMoved()
{
    //viewport()->update();
    //qDebug() << "moved";
}

void nodeGraph::removeEdge(Edge *edge)
{
    edge->sourceNode()->removeEdge(edge);
    edge->destNode()->removeEdge(edge);
    this->scene()->removeItem(edge);
    delete edge;
}

void nodeGraph::removeNode(Node *node)
{
    qDebug() << "Node - mainEdge: " << node->getMainEdge();
    qDebug() << "Edges in: " << node->edgesIn();
    qDebug() << "Edges out: " << node->edgesOut();
    qDebug() << "Edges: " << node->edges();

    delete node->getOp();

    Edge *mE = node->getMainEdge();
    Node *mD = 0;
    qDebug() << "MainEdge: " << mE;
    if (mE) {
        qDebug() << "meDest: " << node->name();
        mD = mE->sourceNode();
        if (mD) {
            qDebug() << "meSource" << mD->name();
        }
    }

    foreach (Edge *edge, node->edgesIn())
    {
        edge->disconnect();
        this->scene()->removeItem(edge);
        //delete edge;
    }


    foreach (Edge *edge, node->edges())
    {
        if (edge->destNode() == node) {
            this->scene()->removeItem(edge);
        }
    }

    qDebug() << "edgesIn deleted";

    qDebug() << "Edges: " << node->edges();
    foreach (Edge *edge, node->edges())
    {
        if (edge->sourceNode() != 0) {
            qDebug() << "Edge - source: " << edge->sourceNode()->name();
            edge->disconnect();
            //edge->setSourceNode();
            //qDebug() << "Edge - source ds: " << edge->sourceNode()->name();
            edge->setSourceNode(mD);
        }
        //this->scene()->removeItem(edge);
    }
    qDebug() << "edges disconnected";
    qDebug() << "Edges: " << node->edges();

    QList<Node *> tempList;
    foreach (Node* n, nodeList)
    {
        if (!tempList.contains(n) && n != node) tempList.append(n);
    }
    nodeList.clear();
    nodeList = tempList;
    qDebug() << "node removed from list";
    this->scene()->removeItem(node);

    qDebug() << "node deleted";
}

Edge* nodeGraph::getActiveEdge()
{
    return activeEdge;
}

int nodeGraph::getMode()
{
    int m = 0;
    if (myMode == Pan) m = 0;
    if (myMode == Select) m = 1;
    if (myMode == InsertEdge) m = 2;
    if (myMode == InsertNode) m = 3;
    if (myMode == breakEdge) m = 4;
    return m;
}


QStandardItemModel* nodeGraph::getTableData()
{
    qDebug() << "nodeGraph::getTableData() - " <<  getParent();
    if (this->getParent() == NULL) qDebug() << "nodeGraph Parent NULL!";
    qDebug() << "nodeGraph parent: " << this->myParent;
    return this->myParent->getTableData();
}


MainWindow* nodeGraph::getParent()
{
    return myParent;
}


void nodeGraph::connectViewer(Node *node, int socket)
{

    Edge *e = getActiveViewer()->getMainEdge();
    qDebug() << "Node " << node->name() << " to socket " << socket;
    qDebug() << "Active viewer: " << getActiveViewer()->name();
    qDebug() << "Viewer Main Edge: " << e;
    if (e->sourceNode())
    {
        qDebug() << "Viewer Main Edge name: " << e->sourceNode()->name();
        e->sourceNode()->removeEdge(e);
    }
    //node->removeEdge(getActiveViewer()->getMainEdge());
    e->setSourceNode(node);

    evaluate();
}


Node* nodeGraph::getActiveViewer()
{
    return activeViewer;
}


void nodeGraph::setActiveViewer(Node *node)
{
    activeViewer = node;
}


void nodeGraph::disableSelected()
{
    foreach (QGraphicsItem *i, this->scene()->selectedItems()) {
        Node *n = qgraphicsitem_cast<Node *>(i);
        if (n) {
            n->disable(!n->isDisabled());
        }
    }
    evaluate();
}

void nodeGraph::keyPressEvent(QKeyEvent *event)
{
    QList<Node *> delList;
    switch (event->key()) {
    case Qt::Key_1:
        if (this->scene()->selectedItems().count() == 1)
        {
            Node* n = qgraphicsitem_cast<Node*>(this->scene()->selectedItems().first());
            if (n)
                connectViewer(n, 1);
        }
        break;
    case Qt::Key_2:
        if (this->scene()->selectedItems().count() == 1)
        {
            Node* n = qgraphicsitem_cast<Node*>(this->scene()->selectedItems().first());
            if (n)
                connectViewer(n, 2);
        }
        break;
    case Qt::Key_3:
        if (this->scene()->selectedItems().count() == 1)
        {
            Node* n = qgraphicsitem_cast<Node*>(this->scene()->selectedItems().first());
            if (n)
                connectViewer(n, 3);
        }
        break;
    case Qt::Key_4:
        if (this->scene()->selectedItems().count() == 4)
        {
            Node* n = qgraphicsitem_cast<Node*>(this->scene()->selectedItems().first());
            if (n)
                connectViewer(n, 1);
        }
        break;
    case Qt::Key_5:
        if (this->scene()->selectedItems().count() == 1)
        {
            Node* n = qgraphicsitem_cast<Node*>(this->scene()->selectedItems().first());
            if (n)
                connectViewer(n, 5);
        }
        break;
    case Qt::Key_Control:
        qDebug() << "Control";
        myMode = breakEdge;
        break;
    case Qt::Key_P:
        qDebug() << "Mode: Pan";
        myMode = Pan;
        break;
    case Qt::Key_E:
        qDebug() << "Mode: Insert Edge";
        myMode = InsertEdge;
        break;
    case Qt::Key_N:
        qDebug() << "Mode: Insert Node";
        myMode = InsertNode;
        break;
    case Qt::Key_D:
        qDebug() << "Disable nodes";
        disableSelected();
        break;
    case Qt::Key_Space:
        if (!this->scene()->selectedItems().isEmpty()) {
            //qDebug() << this->scene()->selectedItems();
            foreach (QGraphicsItem *g, this->scene()->selectedItems()) {
                if (qgraphicsitem_cast<Node *>(g)) {
                    qDebug() << qgraphicsitem_cast<Node *>(g)->name();
                }
            }

        }
        break;
    case Qt::Key_Enter:
        break;
    case Qt::Key_Delete:
        qDebug() << "Deleting... " << this->scene()->selectedItems();
        foreach (QGraphicsItem *i, this->scene()->selectedItems()) {
            if (qgraphicsitem_cast<Node *>(i)) {
                delList << qgraphicsitem_cast<Node *>(i);
            } else {
                this->scene()->removeItem(i);
                delete i;
            }
        }
        foreach (Node* n, delList) {
            if (n == activeViewer) { activeViewer = 0; }
            qDebug() << debugStack(nodeList);
            qDebug() << "Start deleting " << n->name();
            removeNode(n);
            delete n;
            qDebug() << "Deleted successfully!";
        }
        qDebug() << debugStack(nodeList);
        evaluate();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
    QGraphicsView::keyPressEvent(event);
}

void nodeGraph::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Control:
        myMode = Pan;
        break;
    default:
        ;
    }
    QGraphicsView::keyReleaseEvent(event);
}

void nodeGraph::mousePressEvent(QMouseEvent *event)
{

    _lastPos = event->pos();
    if (event->buttons().testFlag(Qt::MiddleButton))
    {
        return;
    }

    qDebug() << "Start mousepressEvent...";
    activeEdge = 0;
    QList<QGraphicsItem *> startItems = this->scene()->items(mapToScene(event->pos()), Qt::IntersectsItemShape);
    if (startItems.count() > 0)
    {
        Node *nodeHit = qgraphicsitem_cast<Node *>(startItems.first());
        Edge *edgeHit = qgraphicsitem_cast<Edge *>(startItems.first());
        Backdrop *bdHit = qgraphicsitem_cast<Backdrop *>(startItems.first());
        //qDebug() << "backdrop: " << bdHit;
        if (edgeHit != 0) {
            if (myMode != breakEdge) {
                myMode = InsertEdge;
            }
            this->scene()->clearSelection();
            activeEdge = edgeHit;
            activeEdge->adjust();
            activeEdge->update();
        }
        if (nodeHit != 0) {
            myMode = Pan;
            //activeEdge = 0;
        }

        if (bdHit != 0) {
            myMode = Pan;
        }

    }
    else
    {
        //activeEdge = 0;
        if (event->buttons().testFlag(Qt::LeftButton))
        {
            myMode = Select;
        }
        else
        {
            myMode = Pan;
        }
    }

    qDebug() << "Start mousePress switch...";
    Node *node;
    Backdrop *bd;
    switch (myMode) {
        case InsertNode:
            //qDebug() << event->pos();
            node = new Node(this, "Ehee", 0);
            this->scene()->addItem(node);
            node->setPos(mapToScene(event->pos()));
            nodeList << node;
            viewport()->setCursor(Qt::ArrowCursor);
            break;
        case InsertEdge:

            //qDebug() << "InsertEdge";

            //if (activeEdge)

            if (activeEdge->sourceNode()) {
                //qDebug() << "Insertedge - source exists, disconnect" << activeEdge->sourceNode()->name();
                activeEdge->disconnect();
            }
            line = new QGraphicsLineItem(QLineF(mapToScene(event->pos()), mapToScene(event->pos())));
            line->setPen(QPen(QColor::fromRgbF(1.0, 0.0, 0.0, 0.0), 2));
            this->scene()->addItem(line);
            break;
        case Pan:
            qDebug() << "Mode: Pan";
            if (!startItems.isEmpty())
            {
                bd = qgraphicsitem_cast<Backdrop *>(startItems.first());
                //qDebug() << "Pan backdrop" << bd;
                if (bd != 0)
                {
                    //qDebug() << "bd 2: " << bd;
                    bd->setItems();
                }
            }
            qDebug() << "Pan break";
            break;
        case Select:
            qDebug() << "Mode: select";
            selectRect = new QGraphicsRectItem(QRectF(mapToScene(event->pos()), mapToScene(event->pos())));
            selectRect->setPen(QPen(QColor::fromRgbF(1.0, 0.6, 0.0, 0.7), 1, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
            selectRect->setBrush(QBrush(QColor::fromRgbF(1.0, 0.6, 0.0, 0.1)));
            this->scene()->addItem(selectRect);
            break;
        case breakEdge:
            qDebug() <<  "break!";
            node = new Node(this, ".", 99);
            pushNode(node, mapToScene(event->pos()));
            qDebug() << "breakEdge: pushed";
            qDebug() << "breakEdge: getMainEdge " << node->getMainEdge();
            node->getMainEdge()->setSourceNode(activeEdge->sourceNode());
            qDebug() << "breakEdge: set dot Source " << node->getMainEdge()->sourceNode()->name();
            activeEdge->disconnect();
            qDebug() << "breakEdge: disconnect";
            activeEdge->setSourceNode(node);
            qDebug() << "breakEdge: setnewsource";
            activeEdge->adjust();
            node->setSelected(true);
            myMode = Pan;
            evaluate();
            break;
    default:
        ;
    }
    //viewport()->update();
    QGraphicsView::mousePressEvent(event);
}

void nodeGraph::mouseReleaseEvent(QMouseEvent *event)
{

    //if (activeEdge) activeEdge->setVisible(true);

    if (myMode == Pan)
    {
        if (this->scene()->selectedItems().count() == 1)
        {
            Backdrop *bd;
            Node *nd;
            bd = qgraphicsitem_cast<Backdrop*>(this->scene()->selectedItems().first());
            nd = qgraphicsitem_cast<Node*>(this->scene()->selectedItems().first());
            if (bd)
            {
                bd->setItems();
            }

            // Kui parasjagu hõljume mõne serva kohal, siis loome uued ühendused
            foreach (QGraphicsItem *g, this->scene()->items())
            {
                Edge *e = qgraphicsitem_cast<Edge*>(g);
                if (e)
                {
                    if (e->hovered && nd->getMainEdge()) {
                        nd->getMainEdge()->setSourceNode(e->sourceNode());
                        e->disconnect();
                        e->setSourceNode(nd);
                        e->hovered = false;
                        e->adjust();
                        evaluate();
                    }
                }
            }
        }
    }


    if (myMode == Select && selectRect != 0) {
        this->scene()->removeItem(selectRect);
        selectRect = 0;
        myMode = Pan;
        //qDebug() << "nodeGraph::mouseReleaseEvent - remove selectRect";
    }

    if (myMode == InsertEdge && line != 0) {
        //qDebug() << "nodeGraph::mouseReleaseEvent - insertEdge start";
        //QList<QGraphicsItem *> startItems = this->scene()->items(line->line().p1());
        QList<QGraphicsItem *> startItems = this->scene()->items(activeEdge->getSourcePoint());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = this->scene()->items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        this->scene()->removeItem(line);
        //delete line;
        qDebug() << "nodeGraph::mouseReleaseEvent - insertEdge line removed";

        if (activeEdge)
        {
            activeEdge->setDragged(0);
            startItems = endItems;
            endItems.clear();
            endItems.append(activeEdge->destNode());
        }


        if (startItems.count() == 0) {
            line = 0;
            //qDebug() << "lõpp pole sõlmel";
            //activeEdge->disconnect();
            activeEdge = 0;
            evaluate();
            return;
        }

        //qDebug() << "release - startitems: " << startItems;
        //qDebug() << "release - enditems: " << endItems;

        if (startItems.count() > 0 && endItems.count() > 0 && startItems.first() != endItems.first()) {
            Node *startNode = qgraphicsitem_cast<Node *>(startItems.first());
            Node *endNode = qgraphicsitem_cast<Node *>(endItems.first());

            // Kui lõpp ei ole sõlmel, siis väljume ja serva ei loo
            if (!endNode || !startNode || (startNode->getType()) == 2) {
                line = 0;
                qDebug() << "algus või lõpp pole sõlmel";
                viewport()->setCursor(Qt::ArrowCursor);
                if (activeEdge->sourceNode()) {
                    activeEdge->disconnect();
                }
                activeEdge->adjust();
                evaluate();
                QGraphicsView::mouseReleaseEvent(event);
                return;
            }

            // Kontroll, kas serv nende sõlmede vahel on juba olemas
            int edgeExists = 0;
            foreach (Edge *e, endNode->edgesIn())
            {
                if (e->sourceNode() == startNode || startNode == activeEdge->destNode())
                {
                    edgeExists = 1;
                    qDebug() << "Exists!";
                }
            }

            if (endNode->getType() == 1) {
                line = 0;
                QGraphicsView::mouseReleaseEvent(event);
                viewport()->setCursor(Qt::ArrowCursor);
                return;
            }

            // Kui serva ei ole, siis loome uue serva
            if (edgeExists == 0) {
                qDebug() << "Edge! " << startNode;
                QList<Node *> evalList;
                evalList = evaluateNode(startNode);
                qDebug() << "Edge 2!";
                if (evalList.contains(endNode))
                {
                    //qDebug() << "Circular!";
                }
                else
                {
                    if (activeEdge)
                    {
                        qDebug() << "Edge 3!" << myMode << activeEdge->sourceNode() << startNode;
                        if (myMode != 4 && activeEdge->sourceNode() != startNode) {
                            qDebug() << "Edge 4!";
                            activeEdge->setSourceNode(startNode);
                        }
                        //startNode->addEdge(activeEdge);
                        qDebug() << "activeedge endnode: " << endNode->name();
                        //endNode->addEdge(activeEdge);
                        //this->scene()->update();
                    }
                    else
                    {
                        Edge *edge = new Edge(startNode, endNode, 2);
                        this->scene()->addItem(edge);
                    }
                }
            }
        }
        evaluate();
    }
    line = 0;
    selectRect = 0;
    activeEdge = 0;

    //qDebug() << "nodeGraph::mouseReleaseEvent";
    QGraphicsView::mouseReleaseEvent(event);
    //viewport()->setCursor(Qt::ArrowCursor);
    //viewport()->update();
    //qDebug() << "nodeGraph::mouseReleaseEvent ok";

}

void nodeGraph::mouseMoveEvent(QMouseEvent *event)
{

    QPoint curPos;
    int deltax, deltay;
    curPos = event->pos();
    deltax = _lastPos.x() - curPos.x();
    deltay = _lastPos.y() - curPos.y();
    onNode = 0;

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

    if (selectRect != 0)
    {
        if (myMode == Select)
        {
            //qDebug() << "modeGraph::mouseMoveEvent - selectmode in" << selectRect;
            QRectF newRect;
            newRect.setTopLeft(mapToScene(curPos));
            newRect.setBottomRight(selectRect->rect().bottomRight());
            selectRect->setRect(newRect);
            this->scene()->clearSelection();
            //QList<QGraphicsItem *> QGraphicsScene::items ( const QRectF & rect, Qt::ItemSelectionMode mode, Qt::SortOrder order, const QTransform & deviceTransform = QTransform() ) const
            QList<QGraphicsItem *> selectedItems = this->scene()->items(newRect);
            //if (selectedItems.count()) qDebug() << selectedItems;
            foreach (QGraphicsItem *g, selectedItems) {
                Edge *e = qgraphicsitem_cast<Edge*>(g);
                if (e != 0) { }
                else {
                    g->setSelected(true);
                }
            }

            if (!event->buttons().testFlag(Qt::LeftButton))
            {
                this->scene()->removeItem(selectRect);
                selectRect = 0;
                myMode == Pan;
            }
        }
        else
        {
            //qDebug() << "selectrect & not select";
            this->scene()->removeItem(selectRect);
            selectRect = 0;
        }
    }

    if (myMode == InsertEdge && line != 0)
    {

        QLineF newLine;
        if (activeEdge) {
            newLine.setP1(activeEdge->destNode()->pos());
            newLine.setP2(mapToScene(event->pos()));
            activeEdge->setDragged(1);
            activeEdge->setSourcePoint(mapToScene(event->pos()));
            activeEdge->adjust();
            activeEdge->setVisible(true);
        }
        else {
            newLine.setPoints(line->line().p1(), mapToScene(event->pos()));
        }
        line->setLine(newLine);
        QList<QGraphicsItem *> underMouse = this->scene()->items(mapToScene(curPos));
        //qDebug() << "underMouse: " << underMouse;
        if (underMouse.count() > 0)
        {
            //qDebug() << "underMouse";
            Node *um;
            foreach (QGraphicsItem *g, underMouse) {
                um = qgraphicsitem_cast<Node *>(g);
                if (um == activeEdge->destNode())
                {
                    onNode = 1;
                    activeEdge->setVisible(false);
                    //qDebug() << "onNode: " << onNode;
                }
            }
        }
        else
        {
            onNode = 0;
            activeEdge->setVisible(true);
        }

    }
    //qDebug() << "onNode: " << onNode;
    // Sõlmede ringivedamine vasaku klahviga
    if (event->buttons().testFlag(Qt::LeftButton) && myMode == Pan)
    {
        // Käime üle kõigi servade ja muudame nende hovered flagi ära
        foreach (QGraphicsItem *g, this->scene()->items())
        {
            Edge *e = qgraphicsitem_cast<Edge*>(g);
            if (e)
            {
                e->hovered = false;
            }
        }
        if (!this->scene()->selectedItems().isEmpty())
        {
            // Liigutame kõiki valitud objekte
            foreach (QGraphicsItem *g, this->scene()->selectedItems())
            {

                g->setPos(g->pos() + mapToScene(curPos) - mapToScene(_lastPos));
            }
        }
        // Dragging one node
        if (this->scene()->selectedItems().count() == 1)
        {
            Node *dn = qgraphicsitem_cast<Node*>(this->scene()->selectedItems().first());
            if (dn)
                {
                QList<QGraphicsItem *> hoverOverItems = this->scene()->items(dn->pos());
                foreach (QGraphicsItem *g, hoverOverItems)
                {
                    Edge *de = qgraphicsitem_cast<Edge*>(g);
                    if (de && de->destNode() != dn && de->sourceNode() != dn && dn->getMainEdge())
                    {
                        //qDebug() << "Hovering over x objects: " << hoverOverItems.count() << de;
                        de->hovered = true;
                    }
                }
            }
        }
    }
    _lastPos = curPos;
    QGraphicsView::mouseMoveEvent(event);
}

#ifndef QT_NO_WHEELEVENT
void nodeGraph::wheelEvent(QWheelEvent *event)
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
#endif


void nodeGraph::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 5)
        return;
    scale(scaleFactor, scaleFactor);

}

void nodeGraph::centerView()
{
    qreal x1, y1, x2, y2;

    foreach (Node *n, nodeList) {
        if (n->pos().x() < x1) {x1 = n->pos().x() - 50;}
        if (n->pos().y() < y1) {y1 = n->pos().y() - 50;}
        if (n->pos().x() > x2) {x2 = n->pos().x() + 50;}
        if (n->pos().y() > y2) {y2 = n->pos().y() + 50;}
    }
    //QPointF tl(mapFromScene(x1, y1));
    //QPointF br(mapFromScene(x2, y2));
    //QRect r(tl.x(), tl.y(), br.x(), br.y());
    QRect r(x1, y1, x2, y2);
    //qDebug() << x1 << y1 << x2 << y2 << r.center();
    //centerOn(r.center());

    //setSceneRect(this->scene()->itemsBoundingRect());
    //fitInView(r, Qt::KeepAspectRatio);

    //qDebug() << "brect: " << this->scene()->itemsBoundingRect();
}

void nodeGraph::zoomIn()
{
    scaleView(qreal(1.1));
}

void nodeGraph::zoomOut()
{
    scaleView(1 / qreal(1.1));
}

void nodeGraph::pushItem(QGraphicsItem *item, QPointF pos)
{
    this->scene()->addItem(item);
    item->setPos(pos);
    //qDebug() << "item: " << item;
    if (qgraphicsitem_cast<Node*>(item))
        nodeList << qgraphicsitem_cast<Node*>(item);
    //qDebug() << "Pushed item: " << item;
}

void nodeGraph::pushNode(Node *node)
{
    this->scene()->addItem(node);
    nodeList << node;
    qDebug() << "Pushed: " << node->name();
}

void nodeGraph::pushNode(Node *node, QPointF pos)
{
    this->scene()->addItem(node);
    node->setPos(pos);
    nodeList << node;
    //qDebug() << "Pushed_pos: " << node->name();
}

Edge *nodeGraph::addEdge(Edge *edge)
{
    this->scene()->addItem(edge);
    return edge;
}

void nodeGraph::addNode()
{
    QAction *pAction = qobject_cast<QAction*>(sender());
    qDebug() << pAction->text();

    if (!pAction) {return;}
    Node *node = 0;
    QGraphicsItem *item = 0;

    int n;
    if (pAction->text() == "&New select") {n = 0;}
    if (pAction->text() == "&New Reader") {n = 1;}
    if (pAction->text() == "&New Viewer") {n = 2;}
    if (pAction->text() == "&New append") {n = 3;}
    if (pAction->text() == "&New Backdrop") {n = 4;}
    if (pAction->text() == "&New sort") {n = 5;}
    if (pAction->text() == "&New SQL") {n = 6;}
    if (pAction->text() == "&CreatePoints") {n = 21;}

    switch (n){
    case 0:
        item = new Node(this, "Select", 4);
        break;
    case 1:
        item = new Node(this, "Read", 1);
        break;
    case 2:
        item = new Node(this, "Viewer", 0);
        setActiveViewer(qgraphicsitem_cast<Node*>(item));
        break;
    case 3:
        item = new Node(this, "Append", 3);
        break;
    case 4:
        item = new Backdrop(this, "Backdrop");
        break;
    case 5:
        item = new Node(this, "Sort", 5);
        break;
    case 6:
        item = new Node(this, "SQL Command", 6);
        break;
    case 21:
        item = new Node(this, "CreatePoints", 21);
        break;
    default:
        return;
        break;
    }
    QPointF pos(mapToScene(mapFromGlobal(QCursor::pos())));
    pushItem(item, pos);
    this->scene()->clearSelection();
    item->setSelected(true);
}

void nodeGraph::addOp(OpInterface *Op)
{
    qDebug() << "AddOp" << Op->description();
    QString opName;
    opName = Op->description().split(";").first();
    opName = opName.split("/").last();

    QGraphicsItem *item = 0;
    item = new Node(this, opName, Op);
    qDebug() << "nodeGraph::addOp item: " << item;
    QPointF pos(mapToScene(mapFromGlobal(QCursor::pos())));
    pushItem(item, pos);
    qDebug() << "nodeGraph::addOp item pushed...";
    //this->scene()->clearSelection();
    //item->setSelected(true);
    qDebug() << "nodeGraph::addOp finished...";
}

void nodeGraph::createActions()
{
    newActV = new QAction(tr("&New Viewer"), this);
    newActV->setStatusTip(tr("Create a new viewer node"));
    connect(newActV, SIGNAL(triggered()), this, SLOT(addNode()));

    newActR = new QAction(tr("&New Reader"), this);
    newActR->setStatusTip(tr("Create a new reader node"));
    connect(newActR, SIGNAL(triggered()), this, SLOT(addNode()));

    newAct = new QAction(tr("&New select"), this);
    newAct->setStatusTip(tr("Create a new select node"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(addNode()));

    newActSort = new QAction(tr("&New sort"), this);
    newActSort->setStatusTip(tr("Create a new sort node"));
    connect(newActSort, SIGNAL(triggered()), this, SLOT(addNode()));

    newActSQL = new QAction(tr("&New SQL"), this);
    newActSQL->setStatusTip(tr("Create a new SQL node"));
    connect(newActSQL, SIGNAL(triggered()), this, SLOT(addNode()));

    newActM = new QAction(tr("&New append"), this);
    newActM->setStatusTip(tr("Create a new append node"));
    connect(newActM, SIGNAL(triggered()), this, SLOT(addNode()));

    newActGeo01 = new QAction(tr("&CreatePoints"), this);
    newActGeo01->setStatusTip(tr("Create a new CreatePoints node"));
    connect(newActGeo01, SIGNAL(triggered()), this, SLOT(addNode()));

    newBackDrop = new QAction(tr("&New Backdrop"), this);
    newBackDrop->setStatusTip(tr("Create a new backdrop"));
    connect(newBackDrop, SIGNAL(triggered()), this, SLOT(addNode()));
}

void nodeGraph::contextMenuEvent(QContextMenuEvent *event)
{
    /*
    QMenu menu(this);
    menu.addAction(newActR);
    menu.addAction(newActV);
    menu.addSeparator();
    menu.addAction(newAct);
    menu.addAction(newActSort);
    menu.addAction(newActSQL);
    menu.addAction(newActM);
    menu.addSeparator();
    menu.addAction(newActGeo01);
    menu.addSeparator();
    menu.addAction(newBackDrop);
    menu.exec(event->globalPos());
    */

    myParent->nodeMenu->addAction(newActR);
    myParent->nodeMenu->addAction(newActV);
    myParent->nodeMenu->addSeparator();
    myParent->nodeMenu->addAction(newAct);
    myParent->nodeMenu->addAction(newActSort);
    myParent->nodeMenu->addAction(newActSQL);
    myParent->nodeMenu->addAction(newActM);
    myParent->nodeMenu->addSeparator();
    myParent->nodeMenu->addAction(newActGeo01);
    myParent->nodeMenu->addSeparator();
    myParent->nodeMenu->addAction(newBackDrop);
    myParent->nodeMenu->exec(event->globalPos());
}
