#ifndef OP_H
#define OP_H

#include <QStandardItemModel>
#include <QGraphicsScene>

#include "pirilib.h"

class Knob_Callback;
class Node;
class Edge;

class PIRILIBSHARED_EXPORT Op
{
public:
    void evaluate();
    void setCallback(Knob_Callback *cb);
    Knob_Callback* getCallback() {return myCallback;}
    void showError(QString msg);
    void disabled();

    QStandardItemModel* getTableModel();
    QGraphicsScene* getScene2D();
    int numInputs();
    QList<Edge*> inputs();
    int validateToCol(QStandardItemModel *model, int value);
    int validateToRow(QStandardItemModel *model, int value);
    void copyHeaders(QStandardItemModel *from, QStandardItemModel *to);
    void setParent(Node* node);

protected:
    Node* myParent;
    Knob_Callback* myCallback;
    QString _myName;
    QString _myDesc;
    int inputCount;
    QList<Edge*> myInputs; // see tuleks tegelt asendada Node-dega, kuna nendelt küsime ju sisendit
    QWidget* getKnob(QString knobName);
};

#endif // OP_H
