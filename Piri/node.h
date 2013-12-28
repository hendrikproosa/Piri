#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>

class Edge;
class nodeGraph;
class DataOp;
class Knob_Callback;
class OpInterface;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

//! [0]
class Node : public QGraphicsItem
{
public:
    Node(nodeGraph *nodeGraph, QString name, int type);
    Node(nodeGraph *nodeGraph, QString name, OpInterface *Op);
    ~Node();
    enum { Type = UserType + 1 };
    int type() const { return Type; }
    QString myName;
    QString myDesc;
    QString myClass;

    void addEdge(Edge *edge, int isMain);
    void removeEdge(Edge *edge);
    QList<Edge *> edges() const;
    QList<Edge *> edgesIn() const;
    QList<Edge *> edgesOut() const;

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setName(QString name);
    QString name() { return myName; }
    int getType() { return nodeType; }
    QString hash() { return nodeHash; }
    void setMaxInputs(int inputs) { maxInputs = inputs; }
    void setNumInputs(int inputs) { numInputs = inputs; }
    int getMaxInputs() { return maxInputs; }
    int getNumInputs() { return numInputs; }
    Edge* getMainEdge() { return mainEdge; }
    nodeGraph* getParent() { return graph; }
    Knob_Callback* getCallback() { return myCallback; }
    DataOp* getOp() { return myOp; }
    OpInterface* getOp2() { return myOp2; }
    void makeCallback();
    void disable(bool val);
    bool isDisabled();

    void execute();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void setupInputs();
    void setupInputs2();
    void setupOps();
    void setupOps2();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    QList<Edge *> edgeList;
    QPointF newPos;
    nodeGraph *graph;
    QString nodeName;
    int nodeType;
    QString nodeHash;
    int maxInputs;
    int numInputs;
    Edge* mainEdge;
    bool disabled;

    DataOp *myOp;
    Knob_Callback* myCallback;

    OpInterface *myOp2;
    int version;

};
//! [0]

#endif // NODE_H
