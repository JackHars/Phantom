/****************************************************************************
** Meta object code from reading C++ file 'geocoder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "geocoder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'geocoder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Geocoder_t {
    QByteArrayData data[20];
    char stringdata0[234];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Geocoder_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Geocoder_t qt_meta_stringdata_Geocoder = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Geocoder"
QT_MOC_LITERAL(1, 9, 12), // "searchResult"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 7), // "success"
QT_MOC_LITERAL(4, 31, 19), // "QMapbox::Coordinate"
QT_MOC_LITERAL(5, 51, 10), // "coordinate"
QT_MOC_LITERAL(6, 62, 4), // "name"
QT_MOC_LITERAL(7, 67, 7), // "address"
QT_MOC_LITERAL(8, 75, 13), // "searchResults"
QT_MOC_LITERAL(9, 89, 23), // "QVector<LocationResult>"
QT_MOC_LITERAL(10, 113, 7), // "results"
QT_MOC_LITERAL(11, 121, 5), // "query"
QT_MOC_LITERAL(12, 127, 12), // "errorMessage"
QT_MOC_LITERAL(13, 140, 13), // "tokenVerified"
QT_MOC_LITERAL(14, 154, 5), // "valid"
QT_MOC_LITERAL(15, 160, 7), // "message"
QT_MOC_LITERAL(16, 168, 20), // "handleSearchResponse"
QT_MOC_LITERAL(17, 189, 14), // "QNetworkReply*"
QT_MOC_LITERAL(18, 204, 5), // "reply"
QT_MOC_LITERAL(19, 210, 23) // "handleTokenVerification"

    },
    "Geocoder\0searchResult\0\0success\0"
    "QMapbox::Coordinate\0coordinate\0name\0"
    "address\0searchResults\0QVector<LocationResult>\0"
    "results\0query\0errorMessage\0tokenVerified\0"
    "valid\0message\0handleSearchResponse\0"
    "QNetworkReply*\0reply\0handleTokenVerification"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Geocoder[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   44,    2, 0x06 /* Public */,
       8,    4,   53,    2, 0x06 /* Public */,
       8,    3,   62,    2, 0x26 /* Public | MethodCloned */,
      13,    2,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    1,   74,    2, 0x08 /* Private */,
      19,    1,   77,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 4, QMetaType::QString, QMetaType::QString,    3,    5,    6,    7,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 9, QMetaType::QString, QMetaType::QString,    3,   10,   11,   12,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 9, QMetaType::QString,    3,   10,   11,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   14,   15,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, 0x80000000 | 17,   18,

       0        // eod
};

void Geocoder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Geocoder *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->searchResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QMapbox::Coordinate(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 1: _t->searchResults((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QVector<LocationResult>(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 2: _t->searchResults((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QVector<LocationResult>(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 3: _t->tokenVerified((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->handleSearchResponse((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 5: _t->handleTokenVerification((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Geocoder::*)(bool , QMapbox::Coordinate , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Geocoder::searchResult)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Geocoder::*)(bool , const QVector<LocationResult> & , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Geocoder::searchResults)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Geocoder::*)(bool , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Geocoder::tokenVerified)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Geocoder::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_Geocoder.data,
    qt_meta_data_Geocoder,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Geocoder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Geocoder::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Geocoder.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Geocoder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Geocoder::searchResult(bool _t1, QMapbox::Coordinate _t2, QString _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Geocoder::searchResults(bool _t1, const QVector<LocationResult> & _t2, const QString & _t3, const QString & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 3
void Geocoder::tokenVerified(bool _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
