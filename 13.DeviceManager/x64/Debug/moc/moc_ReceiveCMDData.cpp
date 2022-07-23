/****************************************************************************
** Meta object code from reading C++ file 'ReceiveCMDData.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/DeviceManager/Receiver/ReceiveCMDData.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReceiveCMDData.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ReceiveCMDData_t {
    QByteArrayData data[6];
    char stringdata0[53];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ReceiveCMDData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ReceiveCMDData_t qt_meta_stringdata_ReceiveCMDData = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ReceiveCMDData"
QT_MOC_LITERAL(1, 15, 11), // "receiverCMD"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 7), // "command"
QT_MOC_LITERAL(4, 36, 11), // "receiveData"
QT_MOC_LITERAL(5, 48, 4) // "init"

    },
    "ReceiveCMDData\0receiverCMD\0\0command\0"
    "receiveData\0init"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ReceiveCMDData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   32,    2, 0x0a /* Public */,
       5,    0,   33,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariant,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ReceiveCMDData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ReceiveCMDData *_t = static_cast<ReceiveCMDData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->receiverCMD((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 1: _t->receiveData(); break;
        case 2: _t->init(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ReceiveCMDData::*_t)(QVariant );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ReceiveCMDData::receiverCMD)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ReceiveCMDData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ReceiveCMDData.data,
      qt_meta_data_ReceiveCMDData,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ReceiveCMDData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ReceiveCMDData::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ReceiveCMDData.stringdata0))
        return static_cast<void*>(const_cast< ReceiveCMDData*>(this));
    return QObject::qt_metacast(_clname);
}

int ReceiveCMDData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ReceiveCMDData::receiverCMD(QVariant _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
