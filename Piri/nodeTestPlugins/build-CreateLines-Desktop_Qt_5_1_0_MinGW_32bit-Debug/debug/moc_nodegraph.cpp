/****************************************************************************
** Meta object code from reading C++ file 'nodegraph.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../nodegraph.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'nodegraph.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_nodeGraph_t {
    QByteArrayData data[10];
    char stringdata[76];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_nodeGraph_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_nodeGraph_t qt_meta_stringdata_nodeGraph = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 9),
QT_MOC_LITERAL(2, 20, 0),
QT_MOC_LITERAL(3, 21, 8),
QT_MOC_LITERAL(4, 30, 6),
QT_MOC_LITERAL(5, 37, 7),
QT_MOC_LITERAL(6, 45, 7),
QT_MOC_LITERAL(7, 53, 5),
QT_MOC_LITERAL(8, 59, 12),
QT_MOC_LITERAL(9, 72, 2)
    },
    "nodeGraph\0evaluated\0\0evalList\0zoomIn\0"
    "zoomOut\0addNode\0addOp\0OpInterface*\0"
    "Op\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nodeGraph[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       4,    0,   42,    2, 0x0a,
       5,    0,   43,    2, 0x0a,
       6,    0,   44,    2, 0x0a,
       7,    1,   45,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,

       0        // eod
};

void nodeGraph::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        nodeGraph *_t = static_cast<nodeGraph *>(_o);
        switch (_id) {
        case 0: _t->evaluated((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->zoomIn(); break;
        case 2: _t->zoomOut(); break;
        case 3: _t->addNode(); break;
        case 4: _t->addOp((*reinterpret_cast< OpInterface*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (nodeGraph::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&nodeGraph::evaluated)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject nodeGraph::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_nodeGraph.data,
      qt_meta_data_nodeGraph,  qt_static_metacall, 0, 0}
};


const QMetaObject *nodeGraph::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nodeGraph::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_nodeGraph.stringdata))
        return static_cast<void*>(const_cast< nodeGraph*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int nodeGraph::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void nodeGraph::evaluated(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
