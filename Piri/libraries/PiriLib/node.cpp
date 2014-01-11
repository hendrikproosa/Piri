#include "node.h"
#include "edge.h"
#include "nodegraph.h"
#include "dataop.h"
#include "tableop.h"
#include "op.h"
#include "ops.h"
#include "mainwindow.h"
#include "knob_callback.h"
#include "interfaces.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QtDebug>

int TestFuncNode(int a, int b)
{
    return a + b;
}

Node::Node(nodeGraph *nodeGraph, QString name, int type) :
    graph(nodeGraph),
    myCallback(0),
    mainEdge(0),
    nodeName(name),
    nodeType(type),
    numInputs(0),
    myName(""),
    disabled(false),
    version(0)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(1);

    setupInputs();
    setupOps();
    makeCallback();
}

Node::Node(nodeGraph *nodeGraph, QString name, OpInterface *Op) :
    graph(nodeGraph),
    myCallback(0),
    mainEdge(0),
    nodeName(name),
    nodeType(4),
    numInputs(0),
    myName(name),
    disabled(false),
    version(1)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(1);

    //myDesc = Op->description().split(";").first().split("/").first();

    myOp2 = Op;

    qDebug() << "Node::Node op set...";
    setupInputs2();
    qDebug() << "Node::Node inputs set...";
    //setupOps();
    makeCallback();
    qDebug() << "Node::Node callback set...";
    qDebug() << "Node: päring graph->getParent: " << graph->getParent();
}


Node::~Node()
{
    qDebug() << "Destructor: " << this->name();
}


void Node::setupInputs2()
{
    QStringList arg = myOp2->description().split(";");
    QStringList arg_name = arg.first().split("/");
    myName = arg_name.last();
    myDesc = arg.last();
    myClass = arg_name.first();
    int i = QString(arg.last().split("/").last()).toInt();
    Op *o = dynamic_cast<Op*>(myOp2);
    qDebug() << "Node::setupInputs2 Op cast: " << i;
    o->setParent(this);
    qDebug() << "SetParent OK!";

    if (getParent()->getSelectedNode())
        qDebug() << "Nodegraph has selected nodes: " << getParent()->getSelectedNode()->name();

    maxInputs = i;
    for (int c = 0; c < i; c++)
    {
        if (c == 0)
        {
            qDebug() << "C is 0";
            if (i > 1)
            {
                qDebug() << "I is greater than 1";
                if (getParent()->getSelectedNode())
                {
                    foreach(Edge* eo, getParent()->getSelectedNode()->edgesOut())
                    {
                        eo->setSourceNode(this);
                    }
                    addEdge(graph->addEdge(new Edge(getParent()->getSelectedNode(), this, 1)), 1);
                } else {
                    addEdge(graph->addEdge(new Edge(0, this, 1)), 1);
                }
            } else {
                qDebug() << "I is not greater than 1";
                if (getParent()->getSelectedNode())
                {
                    //addEdge(graph->addEdge(new Edge(getParent()->getSelectedNode(), this, 3)), 1);
                    foreach(Edge* eo, getParent()->getSelectedNode()->edgesOut())
                    {
                        eo->setSourceNode(this);
                    }
                    addEdge(graph->addEdge(new Edge(getParent()->getSelectedNode(), this, 3)), 1);
                } else {
                    addEdge(graph->addEdge(new Edge(0, this, 3)), 1);
                }
            }
        } else {
            qDebug() << "C is not null";
            addEdge(graph->addEdge(new Edge(0, this, 2)), 0);
        }
    }
    qDebug() << "SetupInputs2 OK!";
}


void Node::setupInputs()
{
    switch (nodeType) {
    case 0: //default
        addEdge(graph->addEdge(new Edge(0, this, 0)), 1);
        maxInputs = 10;
        break;
    case 1: //read
        maxInputs = 1;
        break;
    case 2: //write, praegu viewer
        maxInputs = 1;
        addEdge(graph->addEdge(new Edge(0, this, 2)), 1);
        break;
    case 3: //merge, kaks või rohkem sisendit
        maxInputs = 10;
        addEdge(graph->addEdge(new Edge(0, this, 1)), 1);
        addEdge(graph->addEdge(new Edge(0, this, 2)), 0);
        break;
    case 4: //tavaop ühe sisendiga
        addEdge(graph->addEdge(new Edge(0, this, 3)), 1);
        maxInputs = 1;
        break;
    case 5: //sort, üks sisend
        addEdge(graph->addEdge(new Edge(0, this, 3)), 1);
        maxInputs = 1;
        break;
    case 6: //sql command, üks sisend
        addEdge(graph->addEdge(new Edge(0, this, 3)), 1);
        maxInputs = 1;
        break;
    case 21: //createPoints, sisendid puuduvad
        addEdge(graph->addEdge(new Edge(0, this, 3)), 1);
        maxInputs = 1;
        break;
    case 99: //dot
        maxInputs = 1;
        addEdge(graph->addEdge(new Edge(0, this, 2)), 1);
        break;
    default:
        maxInputs = 1;
        addEdge(graph->addEdge(new Edge(0, this, 2)), 1);
        break;
    }
    qDebug() << "Node::setupInputs - return";
}


void Node::setupOps()
{

    switch (nodeType) {
    case 0:
        qDebug() << "New viewer...";
        myOp = new ViewerOp(this);
        break;
    case 1:
        myOp = new tOp_createTable(this);
        break;
    case 2:
        myOp = new tOp_select(this);
        break;
    case 3:
        myOp = new tOp_append(this);
        break;
    case 4:
        myOp = new tOp_select(this);
        break;
    case 5:
        myOp = new tOp_sort(this);
        break;
    case 6:
        myOp = new tOp_sqlCommand(this);
        break;
    case 21:
        myOp = new geo2d_createPoints(this);
        break;
    case 99:
        myOp = new TableOp(this);
        break;
    default:
        myOp = new TableOp(this);
        break;
    }
    qDebug() << "Node::setupOps - return";
    QStringList arg = myOp->description().simplified().split(";");
    QStringList arg_name = arg.first().split("/");
    myName = arg_name.last();
    myDesc = arg.last();

}


void Node::disable(bool val)
{
    disabled = val;
    emit this->update(boundingRect());
}


bool Node::isDisabled()
{
    return disabled;
}

void Node::makeCallback()
{
    if (version)
    {
        Op *o = dynamic_cast<Op*>(myOp2);
        if (!myCallback) {
            myCallback = new Knob_Callback(this);
            myOp2->knobs(myCallback);
            o->setCallback(myCallback);
        }
    } else {
        if (!myCallback) {
            myCallback = new Knob_Callback(this);
            myOp->knobs(myCallback);
            myOp->setCallback(myCallback);
        }
    }
    getParent()->getParent()->getPropViewLayout()->addWidget(myCallback);
    qDebug() << "Node::makeCallback callback name: " << myCallback->getParent()->name();
}

void Node::execute()
{
    qDebug() << "Execute: " << this->name();

    if ((edges().count() == edgesOut().count()) || edgesIn().count() > 0)
    {
        if (!isDisabled())
        {
            qDebug() << "Engine...";
            if (version)
            {
                myOp2->engine();
            } else {
                myOp->engine();
            }
        } else {
            qDebug() << "Disabled...";
            if (version)
            {
                Op *o = dynamic_cast<Op*>(myOp2);
                //myOp2->disabled();
                o->disabled();
                //myOp2->engine();
            } else {
                myOp->disabled();
            }
        }
    } else {
        getParent()->getTableData()->clear();
        getParent()->getParent()->getScene2D()->clear();
    }

}


void Node::setName(QString name)
{
    nodeName = name;
    myName = name;
    this->update();
}

void Node::setNameS(QString name)
{
    setName(name);
}

void Node::addEdge(Edge *edge, int isMain)
{
    numInputs += 1;
    edgeList << edge;
    if (isMain) {
        mainEdge = edge;
    }
    edge->adjust();
}

void Node::removeEdge(Edge *edge)
{
    QList<Edge *> returnList;
    foreach (Edge* e, edgeList)
    {
        if (e != edge && !returnList.contains(e)) {returnList.append(e);}
    }
    edgeList.clear();
    edgeList = returnList;
    qDebug() << "Node::removeEdge - removed:" << edge;
    qDebug() << "Node::removeEdge - edgelist: " << edgeList;
}


QList<Edge *> Node::edges() const
{
    QList<Edge *> returnList;
    foreach (Edge* e, edgeList) {
        if (!returnList.contains(e)) {
            //qDebug() << "tore on"; //qgraphicsitem_cast<Edge *>(e);
            returnList << e;
        }
    }
    return returnList;
}

QList<Edge *> Node::edgesIn() const
{
    QList<Edge *> returnList;
    if (mainEdge && mainEdge->sourceNode() != 0)
    {
        returnList << mainEdge;
    }
    foreach (Edge* edge, edgeList) {
        if (edge->destNode() == this && edge->sourceNode() != 0) {
            if (!returnList.contains(edge)) {
                returnList << edge;
            }
        }
    }
    return returnList;
}

QList<Edge *> Node::edgesOut() const
{
    QList<Edge *> returnList;
    foreach (Edge* edge, edgeList) {
        if (edge->sourceNode() == this && edge->destNode() != 0) {
            returnList << edge;
        }
    }
    return returnList;
}

QRectF Node::boundingRect() const
{
    QRectF bRect;
    qreal adjust = 2;
    if (nodeType != 99) {
        qreal bx = (myName.length() - 10) * 4;
        if (bx > 0) { bx += 40; } else { bx = 40; }
        if (!edgesOut().isEmpty()) {
            //bRect.setCoords(-40, -20, 40, 20);
            bRect.setCoords(-bx, -20, bx, 20);
        }
        else {
            //bRect.setCoords(-40, -20, 40, 28);
            bRect.setCoords(-bx, -20, bx, 28);
        }
    } else {
        bRect.setCoords(-7, -7, 7, 7);
    }
    return bRect;
}


QPainterPath Node::shape() const
{
    QPainterPath path;
    if (nodeType < 21) {
        path.addRect(-40, -20, 80, 40);
    }
    if (nodeType > 20 && nodeType < 31) {
        path.addRoundedRect(QRectF(-40, -20, 80, 40), 10.0, 10.0);
    }
    if (nodeType == 99) {
        path.addEllipse(QPointF(0, 0), 6, 6);
    }
    return path;
}


void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QColor bottomColor(QColor::fromRgbF(0.6, 0.4, 0.2, 1));
    QColor color2d(QColor::fromRgbF(0.5, 0.6, 1.0, 1));
    QColor disabledC(QColor::fromRgbF(0.4, 0.4, 0.4, 1.0));
    QPen readPen(Qt::darkRed, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen writePen(Qt::darkYellow, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen viewerPen(Qt::darkYellow, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen geo2dPen(color2d.darker(150), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen pen(QColor::fromRgbF(0.4, 0.4, 0.4, 1), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen selectedPen(QColor::fromRgbF(0.8, 0.6, 0.2, 1), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen bottomPen(bottomColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    QPen dotPen(QColor::fromRgbF(0.7, 0.7, 0.7, 1), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen fontPen(QColor::fromRgbF(0.0, 0.0, 0.0, 1), 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen disabledPen(disabledC.darker(150), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    QBrush readBrush(QColor::fromRgbF(0.8, 0.3, 0.3, 1), Qt::SolidPattern);
    QBrush writeBrush(Qt::yellow);
    QBrush viewerBrush(Qt::yellow);
    QBrush geo2dBrush(color2d);
    QBrush brush(Qt::darkGray);
    QBrush bottomBrush(bottomColor);
    QBrush disabledBrush(disabledC);

    // Joonistame kolmnurgakese alla kui väljundeid pole
    if (edgesOut().isEmpty() && nodeType != 0) {
        painter->setPen(bottomPen);
        painter->setBrush(bottomBrush);
        painter->drawPolygon(QPolygonF() << QPointF(-8, 15) << QPointF(8, 15) << QPointF(0, 25));
    }

    if (myClass == "Create") nodeType = 21;
    if (myClass == "Input") nodeType = 1;
    //if (myClass == "Output") nodeType = 2;
    //if (myName == "Viewer") nodeType = 0;
    //if (myName == "Dot") nodeType = 99;

    switch (nodeType) {
    case 0:
        painter->setPen(viewerPen);
        painter->setBrush(viewerBrush);
        break;
    case 1:
        painter->setPen(readPen);
        painter->setBrush(readBrush);
        break;
    case 2:
        painter->setPen(writePen);
        painter->setBrush(writeBrush);
        break;
    case 99:
        painter->setPen(dotPen);
        break;
    default:
        painter->setPen(pen);
        painter->setBrush(brush);
    }

    if (nodeType > 20 && nodeType <= 40) {
        painter->setPen(geo2dPen);
        painter->setBrush(geo2dBrush);
    }

    if (isSelected())
    {
        painter->setPen(selectedPen);
    }

    if (isDisabled())
    {
        //painter->setPen(disabledPen);
        painter->setBrush(disabledBrush);
    }

    // Node shape
    if (nodeType <= 20) {
        painter->drawRect(QRectF(-36, -16, 72, 32));
    }

    if (nodeType > 20 && nodeType <= 30) {
        painter->drawRoundedRect(QRectF(-36, -16, 72, 32), 10.0, 10.0);
    }


    if (nodeType < 99) {
        painter->setPen(fontPen);
        QFont serifFont("Verdana", 10, QFont::Normal);
        painter->setFont(serifFont);
        painter->drawText(QRectF(-60, -18, 120, 35), Qt::AlignCenter, myName);
    } else {
        painter->setBrush(brush);
        painter->drawEllipse(QPointF(0, 0), 5, 5);
    }

    // If disabled
    if (isDisabled())
    {
        QPen redCross(QColor::fromRgbF(1.0, 0.0, 0.0, 0.7), 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(redCross);
        painter->drawLine(QPointF(-36, -16), QPointF(36, 16));
        painter->drawLine(QPointF(-36, 16), QPointF(36, -16));
    }


}


QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        graph->itemMoved();
        break;
    case ItemSelectedHasChanged:
        //qDebug() << "Node::itemChange...";
        //qDebug() << this->getOp() << this->getOp2();
        if (version)
        {
            Op *o = dynamic_cast<Op*>(myOp2);
            if (o->getCallback() > 0)
            {
                if (this->isSelected()) {
                    //qDebug() << "Node selected";
                    o->getCallback()->setStyleSheet("Knob_Callback QPushButton { background: rgb(220, 130, 60); color: black}");

                } else {
                    //qDebug() << "Node NOT selected";
                    o->getCallback()->setStyleSheet("Knob_Callback QPushButton { background: rgb(160, 100, 60); color: white}");
                }
            }

        } else {
            if (this->getOp()->getCallback() > 0)
            {
                if (this->isSelected()) {
                    //qDebug() << "Node selected";
                    this->getOp()->getCallback()->setStyleSheet("Knob_Callback QPushButton { background: rgb(220, 130, 60); color: black}");

                } else {
                    //qDebug() << "Node NOT selected";
                    this->getOp()->getCallback()->setStyleSheet("Knob_Callback QPushButton { background: rgb(160, 100, 60); color: white}");
                }
            }
        }
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}


void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //update();
    //qDebug() << "Node!" << name();
    QGraphicsItem::mousePressEvent(event);
}


void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Node!" << name();
    if (!myCallback) {
        myCallback = new Knob_Callback(this);
        myOp->knobs(myCallback);
        myOp->setCallback(myCallback);
        //myCallback->makeKnobs();
    }
    getParent()->getParent()->getPropViewLayout()->addWidget(myCallback);
    if (myCallback->isHidden())
        myCallback->show();

}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //update();
    //qDebug() << "Node!" << name();
    QGraphicsItem::mouseReleaseEvent(event);
}
