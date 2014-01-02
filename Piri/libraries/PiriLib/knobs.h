#ifndef KNOBS_H
#define KNOBS_H

#include <QString>
#include <QtDebug>
#include <QtWidgets>
#include <QObject>

#include "pirilib.h"
#include "knob_callback.h"

class Knob_Callback;
class ComboBoxKnob;

void PIRILIBSHARED_EXPORT ADD_VALUES(Knob_Callback *f, QString str);
void PIRILIBSHARED_EXPORT UPDATE_VALUES(Knob_Callback *f, QWidget *widget, QString str);
void PIRILIBSHARED_EXPORT UPDATE(Knob_Callback *f, QWidget* widget, QString str);

//__declspec(dllexport) int TestFunc(int a, int b);


class PIRILIBSHARED_EXPORT StringKnob : public QLineEdit
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

class PIRILIBSHARED_EXPORT IntegerKnob : public QSpinBox
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

class PIRILIBSHARED_EXPORT CheckBoxKnob : public QCheckBox
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

class PIRILIBSHARED_EXPORT ComboBoxKnob : public QComboBox
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


StringKnob* PIRILIBSHARED_EXPORT String_knob(Knob_Callback* f, QString* value, QString label);
IntegerKnob* PIRILIBSHARED_EXPORT Integer_knob(Knob_Callback* f, int *value, QString label);
//void PIRILIBSHARED_EXPORT Integer_pair_knob(Knob_Callback* f, QString* value, QString label);
CheckBoxKnob* PIRILIBSHARED_EXPORT CheckBox_knob(Knob_Callback* f, bool *value, QString label);
ComboBoxKnob* PIRILIBSHARED_EXPORT ComboBox_knob(Knob_Callback* f, int *value, QString label, QString valueName);

ComboBoxKnob* PIRILIBSHARED_EXPORT ComboBox_knob2();

#endif // KNOBS_H
