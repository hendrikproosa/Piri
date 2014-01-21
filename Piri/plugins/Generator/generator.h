#ifndef GENERATOR_H
#define GENERATOR_H

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

struct Element {
    ulong id;
    qreal x;
    qreal y;
    qreal z;
};

struct Element_f {
    int id;
    float x;
    float y;
    float z;
};

struct Object {
    ulong id;
    uint type;
};

struct Mesh {
    ulong objectId;
    ulong elementId;
};

class Generator : public QObject, public OpInterface, public Op
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
    int _points;
    int _mult;
    int _div;
    bool _update;
    QString _cacheFile;
    QVector<QVector4D> vertices;
    QVector<QVector2D> objects;
    QHash<QString, ulong> hash;
    QVector<Element> elementsVector;
    QList<Element> elementsList;
    QVector<Object> objectsVector;
    QList<Object> objectsList;
    //QHash<QString, Mesh> meshHash;
};

#endif // GENERATOR_H
