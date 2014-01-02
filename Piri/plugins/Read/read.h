#ifndef READ_H
#define READ_H

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

class Read : public QObject, public OpInterface, public Op
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
    QString _file;
    QString _delimiter;
    int _colorSpace;
};

#endif // CREATEPOLYGONS_H