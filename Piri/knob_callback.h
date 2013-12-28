#ifndef KNOB_CALLBACK_H
#define KNOB_CALLBACK_H

#include <QtWidgets>

struct KnobStruct {
    QWidget* widget;
    QString label;
    QString valueName;
};

class DataOp;
class TableOp;
class Node;

class Knob_Callback : public QWidget
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
    void addKnob(QWidget* knob, QString label, QString valName);
    KnobStruct* getKnob(QString valName);

    //void addValue(QVariant* v);
    void showError(QString msg);

public slots:
    void valueChanged();
    void updateKnobs();

private:
    QFormLayout* myLayout;
    Node* myParent;
    QList<KnobStruct*> knobs;
    //QList<QVariant*> myValues;

};

#endif // KNOB_CALLBACK_H
