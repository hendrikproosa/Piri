#include "knobs.h"
#include "knob_callback.h"
#include "dataop.h"

#include <QString>
#include <QtDebug>
#include <QtWidgets>
#include <QObject>

/*
__declspec(dllexport) int TestFunc(int a, int b)
{
    return a + b;
}
*/

void ADD_VALUES(Knob_Callback *f, QString str)
{
    QWidget *last = f->getLayout()->itemAt(f->getLayout()->rowCount()*2-1)->widget();
    for (int i = 1; i <= f->getLayout()->rowCount(); i++)
    {
        qDebug() << "Item in callback: " << f->getLayout()->itemAt(i*2-1)->widget();
    }
    qDebug() << "Last widget in callback: " << last;
    if (qobject_cast<ComboBoxKnob*>(last))
    {
        ComboBoxKnob *combo = qobject_cast<ComboBoxKnob*>(last);
        combo->addItems(str.split(","));
        combo->setMaximumWidth(str.length() * 5);
    }
}

void UPDATE_VALUES(Knob_Callback *f, QWidget* widget, QString str)
{
    if (qobject_cast<ComboBoxKnob*>(widget))
    {
        ComboBoxKnob *combo = qobject_cast<ComboBoxKnob*>(widget);
        combo->clear();
        combo->addItems(str.split(","));
        combo->setMaximumWidth(str.length() * 5);
    }
}

void UPDATE(Knob_Callback *f, QWidget* widget, QString str)
{
    if (qobject_cast<ComboBoxKnob*>(widget))
    {
        ComboBoxKnob *combo = qobject_cast<ComboBoxKnob*>(widget);
        combo->setCurrentIndex(str.toInt());
    }
}



/*
----------------------------------------------------------------------
STRING
----------------------------------------------------------------------
*/

StringKnob* String_knob(Knob_Callback *f, QString *value, QString label)
{
    qDebug() << "String knob: " << value << *value << label;
    StringKnob *knob = new StringKnob(f, value, label);
    f->getLayout()->addRow(label, knob);
    knob->setToolTip(label.replace(":", "").toLower());
    return knob;
}

StringKnob::StringKnob(QWidget *parent) : QLineEdit(parent)
{
    //return &this;
    this->setText(QString("StringKnob set text"));
}

StringKnob::StringKnob(Knob_Callback *f, QString* value, QString label) :
    _myValue(value)
{

    this->setText(*value);
    //this->setMaximumWidth(100);
    connect(this, SIGNAL(editingFinished()), this, SLOT(updateValue()));
    connect(this, SIGNAL(editingFinished()), f, SLOT(valueChanged()));

}

void StringKnob::updateValue()
{
    qDebug() << "In UpdateValue";
    *_myValue = this->text();
}

/*
----------------------------------------------------------------------
INTEGER_KNOB
----------------------------------------------------------------------
*/

IntegerKnob *Integer_knob(Knob_Callback *f, int *value, QString label)
{
    qDebug() << "Integer knob: " << value << *value << label;
    IntegerKnob *knob = new IntegerKnob(f, value, label);
    f->getLayout()->addRow(label, knob);
    knob->setToolTip(label.replace(":", "").toLower());
    return knob;
}

IntegerKnob::IntegerKnob(QWidget *parent) : QSpinBox(parent)
{

}

IntegerKnob::IntegerKnob(Knob_Callback *f, int* value, QString label) :
    _myValue(value)
{

    this->setValue(*value);
    this->setMaximumWidth(70);
    this->setMinimumHeight(20);
    this->setRange(-100000000, 100000000);
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(updateValue(int)));
    connect(this, SIGNAL(valueChanged(int)), f, SLOT(valueChanged()));

}

IntegerKnob::IntegerKnob(Knob_Callback *f, int *value, QString label, int min, int max) :
    _myValue(value)
{
    this->setValue(*value);
    this->setMaximumWidth(70);
    this->setMinimumHeight(20);
    this->setRange(min, max);
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(updateValue(int)));
    connect(this, SIGNAL(valueChanged(int)), f, SLOT(valueChanged()));
}

void IntegerKnob::updateValue(int v)
{
    *_myValue = this->value();
}


/*
----------------------------------------------------------------------
CHECKBOX
----------------------------------------------------------------------
*/

CheckBoxKnob *CheckBox_knob(Knob_Callback *f, bool *value, QString label)
{
    qDebug() << "Checkbox knob: " << value << *value << label;
    CheckBoxKnob *knob = new CheckBoxKnob(f, value, label);
    f->getLayout()->addRow(label, knob);
    knob->setToolTip(label.replace(":", "").toLower());
    return knob;
}

CheckBoxKnob::CheckBoxKnob(QWidget *parent) : QCheckBox(parent)
{
}

CheckBoxKnob::CheckBoxKnob(Knob_Callback *f, bool* value, QString label) :
    _myValue(value)
{
    this->setTristate(false);
    this->setChecked(*value);
    //this->setCheckState(static_cast<Qt::CheckState>(*value));
    this->setMaximumSize(15, 15);
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(updateValue(int)));
    connect(this, SIGNAL(pressed()), f, SLOT(valueChanged()));
    connect(this, SIGNAL(clicked()), f, SLOT(valueChanged()));
}

int CheckBoxKnob::value()
{
    return *_myValue;
}

void CheckBoxKnob::updateValue(int v)
{
    *_myValue = this->isChecked();
}

void CheckBoxKnob::valueChanged(bool v)
{
    int val = 0;
    if (v) {val = 1;}
    emit updateValue(val);
}

/*
----------------------------------------------------------------------
COMBOBOX
----------------------------------------------------------------------
*/

ComboBoxKnob* ComboBox_knob2()
{
    qDebug() << "Ei häda miskit!";
}

ComboBoxKnob* ComboBox_knob(Knob_Callback *f, int *value, QString label, QString valueName)
{
    qDebug() << "Combobox knob: " << value << *value << label;
    ComboBoxKnob *knob = new ComboBoxKnob(f, value, label);
    f->getLayout()->addRow(label, knob);
    f->addKnob(knob, label, valueName);
    knob->setToolTip(valueName);
    return knob;
}

ComboBoxKnob::ComboBoxKnob(QWidget *parent) : QComboBox (parent)
{
}

ComboBoxKnob::ComboBoxKnob(Knob_Callback *f, int* value, QString label) :
    _myValue(value)
{
    this->setMinimumWidth(70);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(updateValue(int)));
    connect(this, SIGNAL(currentIndexChanged(int)), f, SLOT(valueChanged()));
}

void ComboBoxKnob::updateValue(int v)
{
    *_myValue = this->currentIndex();
}
