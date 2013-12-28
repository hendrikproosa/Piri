#ifndef KNOBS_H
#define KNOBS_H

#include <QString>
#include <QtDebug>
#include <QtWidgets>
#include <QObject>


class Knob_Callback;
class StringKnob;
class IntegerKnob;
class CheckBoxKnob;
class ComboBoxKnob;

void ADD_VALUES(Knob_Callback *f, QString str);
void UPDATE_VALUES(Knob_Callback *f, QWidget *widget, QString str);
void UPDATE(Knob_Callback *f, QWidget* widget, QString str);

StringKnob *String_knob(Knob_Callback* f, QString* value, QString label);
IntegerKnob* Integer_knob(Knob_Callback* f, int *value, QString label);
//void Integer_pair_knob(Knob_Callback* f, QString* value, QString label);
CheckBoxKnob* CheckBox_knob(Knob_Callback* f, bool *value, QString label);
ComboBoxKnob* ComboBox_knob(Knob_Callback* f, int *value, QString label, QString valueName);


class StringKnob : public QLineEdit
{
    Q_OBJECT
public:
    StringKnob(QWidget *parent = 0);
    StringKnob(Knob_Callback *f, QString *value, QString label);

public slots:
    void updateValue();

private:
    QString* _myValue;
};

class IntegerKnob : public QSpinBox
{
    Q_OBJECT
public:
    IntegerKnob(QWidget *parent = 0);
    IntegerKnob(Knob_Callback *f, int *value, QString label);
    IntegerKnob(Knob_Callback *f, int *value, QString label, int min, int max);

public slots:
    void updateValue(int v);

private:
    int* _myValue;
};

class CheckBoxKnob : public QCheckBox
{
    Q_OBJECT
public:
    CheckBoxKnob(QWidget *parent = 0);
    CheckBoxKnob(Knob_Callback *f, bool *value, QString label);
    int value();

public slots:
    void updateValue(int v);
    void valueChanged(bool v);

private:
    bool* _myValue;
};

class ComboBoxKnob : public QComboBox
{
    Q_OBJECT
public:
    ComboBoxKnob(QWidget *parent = 0);
    ComboBoxKnob(Knob_Callback *f, int *value, QString label);

public slots:
    void updateValue(int v);

private:
    int* _myValue;
};
#endif // KNOBS_H
