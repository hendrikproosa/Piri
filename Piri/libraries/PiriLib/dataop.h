#ifndef DATAOP_H
#define DATAOP_H

#include "node.h"
#include "edge.h"
#include "viewer.h"
#include "geoobject.h"
#include "interfaces.h"

#include <QString>
#include <QStandardItemModel>
#include <QGraphicsScene>

#include "pirilib.h"

class Node;
class Knob_Callback;
class Viewer;

class PIRILIBSHARED_EXPORT DataOp// : public QObject, public OpInterface
{
public:
    DataOp();
    DataOp(Node* parent);
    ~DataOp();


    virtual void engine();
    virtual void knobs(Knob_Callback* f);
    virtual void description(QString name, QString desc);

    QString description();


    void setCallback(Knob_Callback *cb);
    Knob_Callback* getCallback() {return myCallback;}
    void showError(QString msg);
    void disabled();

    QStandardItemModel* getTableModel();
    QGraphicsScene* getScene2D();
    Viewer* getViewer();

    bool isMakeKnobs();

protected:
    int validateToCol(QStandardItemModel *model, int value);
    int validateToRow(QStandardItemModel *model, int value);
    void copyHeaders(QStandardItemModel *from, QStandardItemModel *to);
    Node* myParent;
    Knob_Callback* myCallback;
    QString _myName;
    QString _myDesc;
    bool makeKnobs = true;

public slots:
    void make_knobs(Knob_Callback* f);

};

#endif // DATAOP_H
