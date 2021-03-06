#ifndef KNOB_CALLBACK_H
#define KNOB_CALLBACK_H

#include <QtWidgets>

#include "pirilib.h"

struct KnobStruct {
    QWidget* widget;
    QString label;
    QVariant* value;
    QString valueName;
    QString* hash;
};

class DataOp;
class TableOp;
class Node;

class PIRILIBSHARED_EXPORT Knob_Callback : public QWidget
{
    Q_OBJECT
public:
    Knob_Callback(QWidget *parent = 0);
    Knob_Callback(Node* parent);
    ~Knob_Callback();

    void makeKnobs();
    QFormLayout *getLayout();
    Node* getParent();
    DataOp* getOp();
    void addKnob(QWidget* knob, QString label, QVariant *value);
    KnobStruct* getKnob(QString valName);
    QString getHash();

    //void addValue(QVariant* v);
    void showError(QString msg);

public slots:
    void valueChanged();
    void updateKnobs();
    void nodeNameChanged(QString name);

private:
    QFormLayout* myLayout;
    Node* myParent;
    QList<KnobStruct*> knobs;
    QString myHash;
    //QList<QVariant*> myValues;

};

#endif // KNOB_CALLBACK_H
