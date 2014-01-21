#ifndef KNOB_H
#define KNOB_H

#include <QStandardItemModel>
#include <QGraphicsScene>

#include "pirilib.h"

class PIRILIBSHARED_EXPORT Knob
{
public:
    Knob();
    void evaluate();
    QString hash();

private:
    QString _myValue;
};

#endif // KNOB_H
