#ifndef SELECT_H
#define SELECT_H

#include <QObject>
#include <QtPlugin>
#include <QStringList>
#include <QImage>
#include "QGraphicsView"
#include <QtCore/qglobal.h>
#include <QStandardItem>

#include "pirilib.h"
#include "interfaces.h"
#include "op.h"

class Select : public QObject, public OpInterface, public Op
{

    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Kaldera.Piri.v01.OpInterface")
    Q_INTERFACES(OpInterface)


public:
    void engine();
    void knobs(Knob_Callback* f);
    void description(QString name, QString desc);
    QString description();

    void setup();

protected:
    int rowFrom;
    int rowTo;
    int colFrom;
    int colTo;

};

#endif // SELECT_H
