#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>

#include "pirilib.h"

class Edge;
class nodeGraph;
class DataOp;
class Knob_Callback;
class OpInterface;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE


class PIRILIBSHARED_EXPORT Node : public QGraphicsItem
{
public:
    Node(nodeGraph *nodeGraph, QString name, int type);
    Node(nodeGraph *nodeGraph, QString name, OpInterface *op);
    ~Node();
    enum { Type = UserType + 1 };
    int type() const { return Type; }
    /*
    QString myName;
    QString myDesc;
    QString myClass;
    */
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
    QString getDesc() { return myDesc; }
    QString hash() { return nodeHash; }
    QString getHash();
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

public Q_SLOTS:
    void setNameS(QString name);

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
    QString myName; /*!< Node name. First set in Op description. */
    QString myDesc; /*!< Node description. Set in Op description. */
    QString myClass; /*!< Node class. Set in Op description. */
    QList<Edge *> edgeList; /*!< List of all node edges. */
    QPointF newPos; /*!< Some position holder. */
    nodeGraph *graph; /*!< Nodegraph this node is in. */
    QString nodeName; /*!< Node name. OLD!!! */
    int nodeType; /*!< Node type. OLD!!! */
    QString nodeHash; /*!< Node hash. Not used. */
    int maxInputs; /*!< Maximum number of inputs. */
    int numInputs; /*!< Number of node inputs. */
    Edge* mainEdge; /*!< Main edge object. */
    bool disabled; /*!< Is node disabled? */

    DataOp *myOp; /*!< Node dataop. OLD!!! */
    Knob_Callback* myCallback; /*!< Knob callback of node. */

    OpInterface *myOp2; /*!< Node OpInterface. Used instead of myOp in plugins. */
    int version; /*!< Node version. Separates built-in and plugin ops. To be removed. */

};

#endif // NODE_H
