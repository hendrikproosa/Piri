#include "generator.h"
#include "node.h"
#include "edge.h"
#include "nodegraph.h"
#include "knob_callback.h"
#include "knobs.h"

#include <QtDebug>
#include <QStandardItemModel>
#include <QTableView>

#include "fstream"
#include "boost/archive/text_oarchive.hpp"

static float frand(){
    float f;
    UINT32 *fi = (UINT32*)&f;
    *fi = 0;
    const int minBitsRandGives  = (1<<15);          //  RAND_MAX is at least (1<<15)
    UINT32 randExp              = 127; //(rand()%254)+1;   //  Esponents are in range of [1..254]
    UINT32 randMantissa         = ((rand() % minBitsRandGives) << 8) | (rand()%256);
    *fi                         = randMantissa | (randExp<<23);                 // Build a float with random exponent and random mantissa
    return f - 1;
}

void Generator::setup()
{
    _points = 100;
    _mult = 1;
    _div = 1;
    _update = false;
    _cacheFile = "C:/tmp/_cache.bin";
}

void Generator::description(QString name, QString desc)
{
    setup();
}


QString Generator::description()
{
    setup();
    return QString("Input/Generate;Generate some data./0");
}


void Generator::knobs(Knob_Callback *f)
{
    String_knob(f, &_cacheFile, "Cache file");
    Integer_knob(f, &_points, "Num points");
    Integer_knob(f, &_mult, "Multiplier");
    Integer_knob(f, &_div, "Divider");
    CheckBox_knob(f, &_update, "Live update");
}


void Generator::engine()
{
    QElapsedTimer timer;
    timer.start();

    if (!_update) return;
    QStandardItemModel *model = getTableModel();
    QGraphicsScene *scene = getScene2D();
    model->clear();
    scene->clear();
    /*
    QFile file("E:/projektid/progemine/piri/lidar_xyz.txt");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        int row = 0;
        while ( !in.atEnd() )
        {
            QString line = in.readLine();
            QStringList list = line.simplified().split(";");
            Element e;
            e.id = row;
            //e.x = list.at(0).toDouble();
            e.x = list.at(0).toDouble();
            e.y = list.at(1).toDouble();
            e.z = list.at(2).toDouble();
            elementsVector << e;
            //if (row == 0) qDebug() QString("%1").arg(e.x, 0, 'g', 30);
            row++;
        }
    }
    else
    {
        model->clear();
        scene->clear();
    }
    file.close();
    */

    ulong points = _points;
    QVector<Element> *elementsVector = new QVector<Element>;
    QList<Element> *elementsList = new QList<Element>;
    QList<Element_f> *elementsFList = new QList<Element_f>;
    QHash<QString, int> *hashList = new QHash<QString, int>;
    qDebug() << "Elementsvector pointer: " << &elementsVector;

    std::ofstream os (_cacheFile.toLatin1(), std::ios::binary);

    for (int i = 0; i < _points; i++)
    {
        Element_f e;
        e.id = i;
        e.x = frand();
        e.y = frand();
        e.z = frand();
        //*elementsVector << e;
        os.write((const char*)&e, sizeof(Element_f));
        //if (i == 0) qDebug() << QString("%1").arg(e.x, 0, 'g', 30);
        //QString hashString = QString(QCryptographicHash::hash((const char*)&e,QCryptographicHash::Md5).toHex());
        //hashList->insert(hashString, i);
        //if (i == 0) qDebug() << hashString;
    }

    os.close();

    std::ifstream is (_cacheFile.toLatin1(), std::ios::binary);

    for (int i = 0; i < _points; i++)
    {
        Element_f r;
        is.seekg(i * sizeof(Element_f),std::ifstream::beg);
        is.read((char*)&r, sizeof(Element_f));
        r.x = r.x * r.y;
        r.y = r.y * r.z;
        r.z = r.z * r.x;
        //qDebug() << QString("%1").arg(r.x, 0, 'g', 30);
    }
    is.close();

    //qDebug() << "Hashlist: " << hashList->key(0);
    //qDebug() << "elementsVector capacity: " << elementsVector->capacity();

    //QString hashString = QString(QCryptographicHash::hash(("myPassword"),QCryptographicHash::Md5).toHex());

    /*
    Element* pData;
    pData = (Element*) calloc (_points, sizeof(Element));
    if (pData==NULL) {
        qDebug() << "pData is null!";
        return;
    }
    free (pData);
    */

    //qDebug() << "Allocated " << _points * _mult * sizeof(Element) << "bytes";
    //QThread::sleep(1);
    //qDebug() << QString("%1").arg(elementsVector->value(0).x, 0, 'g', 30);
    //qDebug() << QString("%1").arg(elementsFList->value(0).x, 0, 'g', 30);
    qDebug() << "Generator Node took: " << timer.elapsed() << "milliseconds";
    delete elementsVector;
}
