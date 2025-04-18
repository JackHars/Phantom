/****************************************************************************
** Meta object code from reading C++ file 'map_search.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "map_search.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'map_search.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MapSearchDialog_t {
    QByteArrayData data[13];
    char stringdata0[159];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MapSearchDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MapSearchDialog_t qt_meta_stringdata_MapSearchDialog = {
    {
QT_MOC_LITERAL(0, 0, 15), // "MapSearchDialog"
QT_MOC_LITERAL(1, 16, 12), // "resultChosen"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 16), // "SearchResultItem"
QT_MOC_LITERAL(4, 47, 6), // "result"
QT_MOC_LITERAL(5, 54, 19), // "onSearchTextChanged"
QT_MOC_LITERAL(6, 74, 16), // "onResultSelected"
QT_MOC_LITERAL(7, 91, 3), // "row"
QT_MOC_LITERAL(8, 95, 15), // "onSearchResults"
QT_MOC_LITERAL(9, 111, 7), // "success"
QT_MOC_LITERAL(10, 119, 25), // "QVector<SearchResultItem>"
QT_MOC_LITERAL(11, 145, 7), // "results"
QT_MOC_LITERAL(12, 153, 5) // "query"

    },
    "MapSearchDialog\0resultChosen\0\0"
    "SearchResultItem\0result\0onSearchTextChanged\0"
    "onResultSelected\0row\0onSearchResults\0"
    "success\0QVector<SearchResultItem>\0"
    "results\0query"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MapSearchDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   37,    2, 0x08 /* Private */,
       6,    1,   38,    2, 0x08 /* Private */,
       8,    3,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 10, QMetaType::QString,    9,   11,   12,

       0        // eod
};

void MapSearchDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MapSearchDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->resultChosen((*reinterpret_cast< SearchResultItem(*)>(_a[1]))); break;
        case 1: _t->onSearchTextChanged(); break;
        case 2: _t->onResultSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onSearchResults((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QVector<SearchResultItem>(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< SearchResultItem >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<SearchResultItem> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MapSearchDialog::*)(SearchResultItem );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapSearchDialog::resultChosen)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MapSearchDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_MapSearchDialog.data,
    qt_meta_data_MapSearchDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MapSearchDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MapSearchDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MapSearchDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int MapSearchDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void MapSearchDialog::resultChosen(SearchResultItem _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
