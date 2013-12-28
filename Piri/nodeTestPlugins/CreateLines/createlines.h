#ifndef CREATELINES_H
#define CREATELINES_H

#include <QObject>
#include <QtPlugin>
#include <QStringList>
#include <QImage>
#include "QGraphicsView"

#include "interfaces.h"
#include "op.h"

#include "geo2dop.h"
#include "geo2d_createpoints.h"
#include "knobs.h"
#include "knob_callback.h"


class CreateLines : public QObject, public OpInterface, public Op
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
    int _columnX;
    int _columnY;
    int _columnID;
    bool _useID;
    int _columnCount;
    int _count;
    int _size;
    bool _showLabels;
    bool _keepScene;
};

#endif // CREATELINES_H
