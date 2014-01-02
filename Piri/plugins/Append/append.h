#ifndef APPEND_H
#define APPEND_H

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
#include "knobs.h"
#include "knob_callback.h"
#include "viewer.h"
#include "knob.h"
#include "tableop.h"
#include "dataop.h"

class Append : public QObject, public OpInterface, public Op
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
    bool _removeDoubles;
    int _topLayer;
    QString _strtest;
    bool contains(QStandardItemModel *mod, QList<QStandardItem*> list);
};

#endif // CREATEPOLYGONS_H
