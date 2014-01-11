#ifndef VIEWERP_H
#define VIEWERP_H

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
#include "knob.h"
#include "tableop.h"
#include "viewer.h"

class Viewerp : public QObject, public OpInterface, public Op
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
    bool _enableSorting;
    bool _constSize;
    int _pointSize;
    bool _constWidth;
    int _penWidth;
    bool _overRide;
    bool _showVerts;
};

#endif // VIEWERP_H
