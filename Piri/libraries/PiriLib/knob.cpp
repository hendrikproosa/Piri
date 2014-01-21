#include "knob.h"
#include "edge.h"
#include "node.h"
#include "knob_callback.h"
#include "nodegraph.h"
#include "mainwindow.h"
#include "dataop.h"

Knob::Knob()
{

}


void Knob::evaluate()
{
    qDebug() << "Vastus on 42";
}

QString Knob::hash()
{
    return QString(QCryptographicHash::hash((const char*)&_myValue,QCryptographicHash::Md5).toHex());
}
