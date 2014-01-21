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

/*!
 * \brief Main nodegraph class that performs operations on nodes.
 *
 * All operations that add, move, connect or delete nodes are performed here,
 * also holds the view into scne that holds the DAG objects.
 * Subclasses QGraphicsView.
 * \param scene Scene that holds graphical representation of DAG
 * \param parent Main UI window
 */
nodeGraph::nodeGraph(QGraphicsScene *scene, MainWindow *parent)
    : QGraphicsView(parent),
      myParent(parent),
      myScene(scene),
      onNode(0),
      selectRect(0),
      myMode(Pan),
      _contextMenuPos(QPointF(0.0, 0.0))
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

/*!
 * \brief Add scene to nodegraph.
 *
 * Sets main DAG scene as nodegraph scene. Nodegraph acts as a window into that scene
 * and thus there can be more than one nodegraph connected to the same scene.
 * \param scene Scene that holds the DAG objects.
 * @see MainWindow::createDAG()
 */
void nodeGraph::addScene(QGraphicsScene *scene)
{
    setScene(scene);
}

/*!
 * \brief Node graph evaluation loop.
 *
 * Starts from active viewer and creates stack of nodes that are ordered
 * in proper evaluation sequence. Used for loop detection?
 */
void nodeGraph::evaluate()
{

    Node *curNode; /*!< Current last node in evaluation stack */

    evalStack.clear();
    visitStack.clear();

    if (nodeList.isEmpty()) { return; }

    if (activeViewer == 0 || activeViewer->edgesIn().isEmpty()) { return; }

    nodeStack << activeViewer;
    curNode = activeViewer;
    evalStack << activeViewer;

    while (nodeStack.length()) {
        if (curNode->getType() != 0) {
            evalStack << curNode;
        }
        nodeStack.removeLast();
        pushNodeEdgesToStack(curNode);
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

    emit evaluated(debugStack(evalStack));
}

/*!
 * \brief Node graph execution method.
 *
 * Calls the execute() function on last node in evaluation stack 'evalStack'.
 * Usually it is the active viewer node.
 * @see evaluate()
 * @see Node::execute()
 */
void nodeGraph::execute()
{
    getTableData()->clear();
    getParent()->getScene2D()->clear();

    if (evalStack.count())
    {
        evalStack.last()->execute();
    }
}

/*!
 * \brief Evaluates node graph starting from node.
 *
 * Almost the same as evaluate() but evaluates node graph starting from
 * specified node.
 * \param node Node to be evaluated.
 * \return Stack of nodes ordered in execution order.
 * @see evaluate()
 * @see pushNodeEdgesToStack()
 * @see nodeVisited()
 */
QList<Node *> nodeGraph::evaluateNode(Node *node)
{
    Node *vNode, *curNode;

    evalStack.clear();
    visitStack.clear();

    vNode = node;
    nodeStack << vNode;
    curNode = node;

    while (nodeStack.length()) {
        evalStack << curNode;
        if (!nodeStack.isEmpty()) {nodeStack.removeLast();}
        pushNodeEdgesToStack(curNode);
        if (nodeStack.isEmpty()) {break;}
        curNode = nodeStack.last();
    }
    foreach (Node *node, reverseStack(evalStack)) {
        if (!nodeVisited(node))
        {
            visitStack << node;
        }
    }
    evalStack = visitStack;
    return evalStack;
}

/*!
 * \brief Add all nodes that are inputs for current node to 'nodeStack'
 *
 * Add all nodes that are connected to current node as inputs to stack of nodes
 * named 'nodeStack'.
 * \param node Node whose inputs are added.
 * @see evaluate()
 * @see evaluateNode()
 * @see Node::edgesIn()
 * @see Edge::sourceNode()
 */
void nodeGraph::pushNodeEdgesToStack(Node *node)
{
    if (!node->edgesIn().isEmpty())
    {
        foreach (Edge* edge, node->edgesIn()) {
            nodeStack << edge->sourceNode();
        }
    }

}

/*!
 * \brief Creates list of nodes in stack.
 *
 * Not used?
 * \param stack Stack which is debugged.
 * \return QString with node names in stack.
 */
QString nodeGraph::debugStack(QList<Node *> stack)
{
    QString evalList;
    if (stack.length() > 0) {
        foreach (Node *node, stack) {
            evalList += QString(node->name()) + QString("\n");
        }
    } else {
    }
    return evalList;
}

/*!
 * \brief Reverses the order of nodes in stack.
 *
 * Reverses the order of nodes in stack.
 * \param stack Stack to be reversed.
 * \return Reversed node stack.
 */
QList<Node *> nodeGraph::reverseStack(QList<Node *> stack)
{
    QList<Node *> tempStack;
    while (stack.length())
    {
        tempStack << stack.takeLast();
    }
    return tempStack;
}

/*!
 * \brief Checks if node is already visited.
 *
 * Checks node against 'visitStack' to see if it is already visited.
 * \param node Node to be checked.
 * \return 1 if visited, 0 if not.
 */
int nodeGraph::nodeVisited(Node *node)
{
    foreach (Node *vnode, visitStack) {
        if (vnode == node) {return 1;}
    }
    return 0;
}

/*!
 * \brief Checks if node is in list 'list'
 *
 * Almost the same as nodeVisited() but checks against arbitrary list.
 * \param list List to be checked against.
 * \param node Node to be checked.
 * \return 1 if visited, 0 if not.
 */
int nodeGraph::inListNode(QList<Node *> list, Node *node)
{
    foreach (Node *n, list) {
        if (n == node) {return 1;}
    }
    return 0;
}

/*!
 * \brief Does nothing.
 */
void nodeGraph::itemMoved()
{

}

/*!
 * \brief Removes edge from scene and deletes it.
 *
 * Detaches edge from source and destination nodes, removes edge object
 * from DAG scene and deletes the edge object.
 * \param edge Edge to be removed.
 * @see Node::removeEdge()
 */
void nodeGraph::removeEdge(Edge *edge)
{
    edge->sourceNode()->removeEdge(edge);
    edge->destNode()->removeEdge(edge);
    this->scene()->removeItem(edge);
    delete edge;
}

/*!
 * \brief Removes node from nodegraph.
 *
 * Removes node from nodegraph and deletes it. Reconnects all edges to node
 * that is higher in graph hierarchy. Deletes node Ops and callbacks. Op deletion is buggy!!!
 * \param node Node to be removed.
 */
void nodeGraph::removeNode(Node *node)
{
    //delete node->getOp();
    delete node->getCallback();

    Edge *mE = node->getMainEdge();
    Node *mD = 0;
    if (mE) {
        mD = mE->sourceNode();
        if (mD) {
        }
    }

    foreach (Edge *edge, node->edgesIn())
    {
        edge->disconnect();
        this->scene()->removeItem(edge);
    }

    foreach (Edge *edge, node->edges())
    {
        if (edge->destNode() == node) {
            this->scene()->removeItem(edge);
        }
    }

    foreach (Edge *edge, node->edges())
    {
        if (edge->sourceNode() != 0) {
            qDebug() << "Edge - source: " << edge->sourceNode()->name();
            edge->disconnect();
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

/*!
 * \brief Returns active edge.
 * \return Edge in member 'activeEdge'
 */
Edge* nodeGraph::getActiveEdge()
{
    return activeEdge;
}

/*!
 * \brief Get current nodegraph mode (Pan, Select etc)
 * \return Current nodegraph mode.
 */
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

/*!
 * \brief Get pointer to main table data model.
 * \return Main table data model held in MainWindow 'tableData'
 */
QStandardItemModel* nodeGraph::getTableData()
{
    return this->myParent->getTableData();
}


/*!
 * \brief Get parent object of nodegraph.
 *
 * Returns the parent of nodegraph, usually applications MainWindow.
 * \return Node graph parent object.
 */
MainWindow* nodeGraph::getParent()
{
    return myParent;
}

/*!
 * \brief Connects viewer to selected node.
 *
 * Viewer socket 'socket' gets connected to selected node 'node'. Socket part
 * does not work at the moment.
 * \param node Node to be connected
 * \param socket Viewer socket that gets connected (not working)
 */
void nodeGraph::connectViewer(Node *node, int socket)
{
    Edge *e = getActiveViewer()->getMainEdge();
    if (e->sourceNode())
    {
        e->sourceNode()->removeEdge(e);
    }
    e->setSourceNode(node);
    evaluate();
}

/*!
 * \brief Get active viewer node.
 *
 * This function returns active viewer node that is held in 'activeViewer'.
 * \return Active viewer node.
 * @see setActiveViewer()
 */
Node* nodeGraph::getActiveViewer()
{
    return activeViewer;
}


/*!
 * \brief Sets node as active viewer.
 *
 * This function sets selected node as active viewer. Active viewer is held in 'activeViewer'.
 * \param node Node to be set as viewer
 * @see getActiveViewer()
 * @see activeViewer
 */
void nodeGraph::setActiveViewer(Node *node)
{
    activeViewer = node;
}


/*!
 * \brief Disables selected nodes.
 *
 * Disables all selected nodes. Disabled nodes let data pass through unchanged.
 * @see Node::disable()
 */
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

/*!
 * \brief Gets node that was selected at contextmenuevent time.
 *
 * This method is used for contextmenuevent. If one node is selected, this
 * function returns selected node and new node will be added after that
 * in nodegraph.
 * \return Selected node held in '_contextSelectedNode'
 * @see _contextSelectedNode
 */
Node* nodeGraph::getSelectedNode()
{
    if (_contextSelectedNode)
    {
        return _contextSelectedNode;
    } else {
        return 0;
    }
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

    QGraphicsView::mouseReleaseEvent(event);

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


/*!
 * \brief Scales view by scaleFactor.
 *
 * This is a helper function for mouse wheel zoom.
 * \param scaleFactor Factor to scale view by.
 */
void nodeGraph::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 5)
        return;
    scale(scaleFactor, scaleFactor);

}

/*!
 * \brief Centers view on selected nodes. Not working!!!
 */
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

/*!
 * \brief Zoom in.
 *
 * Zooming helper function.
 * @see scaleView()
 */
void nodeGraph::zoomIn()
{
    scaleView(qreal(1.1));
}

/*!
 * \brief Zoom out.
 *
 * Zooming helper function.
 * @see scaleView()
 */
void nodeGraph::zoomOut()
{
    scaleView(1 / qreal(1.1));
}

/*!
 * \brief Pushes item to nodegraph at certain position.
 *
 * Pushes QGraphicsItem 'item' to graph at position 'pos'. Used instead of
 * pushNode() now. If item is node, it will be added to 'nodeList'.
 * \param item QGraphicsItem to be pushed.
 * \param pos Position where item will be.
 * @see nodeList
 */
void nodeGraph::pushItem(QGraphicsItem *item, QPointF pos)
{
    this->scene()->addItem(item);
    item->setPos(pos);
    if (qgraphicsitem_cast<Node*>(item))
        nodeList << qgraphicsitem_cast<Node*>(item);
}

/*!
 * \brief Pushes node to graph.
 * \param node Node to be pushed.
 * @see nodeList
 */
void nodeGraph::pushNode(Node *node)
{
    this->scene()->addItem(node);
    nodeList << node;
}

/*!
 * \brief Pushes node to graph at position.
 * \param node Node to be pushed.
 * \param pos Position where node will be.
 * @see nodeList
 */
void nodeGraph::pushNode(Node *node, QPointF pos)
{
    this->scene()->addItem(node);
    node->setPos(pos);
    nodeList << node;
}

/*!
 * \brief Adds new edge object to graph scene.
 * \param edge Edge to be added.
 * \return Returns the same edge.
 */
Edge *nodeGraph::addEdge(Edge *edge)
{
    this->scene()->addItem(edge);
    return edge;
}

/*!
 * \brief Adds node to nodegraph. OLD!!!
 *
 * Adds new node to nodegraph, used with built-in ops. OLD! Not used with plugin system.
 */
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


/*!
 * \brief Adds new Op to nodegraph.
 *
 * Adds new Op to nodegraph. If one node is previously selected, adds new node to it's bottom.
 * @see MainWindow::addOp()
 * \param Op OpInterface to be added
 */
void nodeGraph::addOp(OpInterface *Op)
{
    qDebug() << "AddOp" << Op->description();
    QString opName;
    opName = Op->description().split(";").first();
    opName = opName.split("/").last();

    QGraphicsItem *item = 0;
    item = new Node(this, opName, Op);
    if (_contextSelectedNode)
        _contextMenuPos += QPoint(0.0, 40.0);
    pushItem(item, _contextMenuPos);
    qDebug() << "nodeGraph::addOp item pushed...";
    this->scene()->clearSelection();
    item->setSelected(true);
    qDebug() << "nodeGraph::addOp finished...";
}

/*!
 * \brief Creates actions for built-in ops. OLD!
 *
 * This function creates actions for built-in ops. Not used anymore!
 */
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

/*!
 * \brief Context menu event.
 *
 * This function shows the nodegraph context menu (right-click menu). It also
 * stores right-click position and node that was selected at event time in
 * variables _contextMenuPos and _contextSelectedNode.
 * \param event Event.
 * @see _contextMenuPos
 * @see _contextSelectedNode
 */
void nodeGraph::contextMenuEvent(QContextMenuEvent *event)
{
    /*
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
    */
    _contextMenuPos = mapToScene(mapFromGlobal(event->globalPos()));
    qDebug() << "Contextmenupos: " << _contextMenuPos;
    qDebug() << "Selected items: " << this->scene()->selectedItems();
    if (this->scene()->selectedItems().count() == 1)
    {
        Node* u = qgraphicsitem_cast<Node*>(this->scene()->selectedItems().first());
        qDebug() << "Selecteditem: " << u->name();
        _contextSelectedNode = u;
    } else {
        _contextSelectedNode = 0;
    }
    myParent->nodeMenu->exec(event->globalPos());
}
