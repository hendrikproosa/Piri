#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>
#include "knob_callback.h"

#include "pirilib.h"

QT_BEGIN_NAMESPACE
class QString;
class QStringList;

QT_END_NAMESPACE

// Kõik funktsioonid peavad olema "pure virtual", ehk siis lõpus = 0 !!! Muidu tuleb jama.
class PIRILIBSHARED_EXPORT OpInterface
{
public:
    virtual ~OpInterface() {}
    virtual void engine() = 0;
    virtual void knobs(Knob_Callback* f) = 0;
    virtual void description(QString name, QString desc) = 0;
    virtual QString description() = 0;

};


QT_BEGIN_NAMESPACE

#define OpInterface_iid "Kaldera.Piri.v01.OpInterface"
Q_DECLARE_INTERFACE(OpInterface, OpInterface_iid)

QT_END_NAMESPACE
#endif // INTERFACES_H
