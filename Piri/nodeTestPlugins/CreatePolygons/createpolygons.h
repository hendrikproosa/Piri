#ifndef CREATEPOLYGONS_H
#define CREATEPOLYGONS_H

#include <QObject>
#include <QtPlugin>
#include <QStringList>
#include <QImage>
#include "QGraphicsView"

#include "interfaces.h"
#include "op.h"

class CreatePolygons : public QObject, public OpInterface, public Op
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

#endif // CREATEPOLYGONS_H
