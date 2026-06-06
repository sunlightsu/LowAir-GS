/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../MainWindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_MainWindow_t {
    uint offsetsAndSizes[32];
    char stringdata0[11];
    char stringdata1[10];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[10];
    char stringdata5[11];
    char stringdata6[15];
    char stringdata7[16];
    char stringdata8[8];
    char stringdata9[11];
    char stringdata10[14];
    char stringdata11[9];
    char stringdata12[9];
    char stringdata13[10];
    char stringdata14[10];
    char stringdata15[4];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MainWindow_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 9),  // "openModel"
        QT_MOC_LITERAL(21, 0),  // ""
        QT_MOC_LITERAL(22, 11),  // "reloadModel"
        QT_MOC_LITERAL(34, 9),  // "resetView"
        QT_MOC_LITERAL(44, 10),  // "fitToModel"
        QT_MOC_LITERAL(55, 14),  // "takeScreenshot"
        QT_MOC_LITERAL(70, 15),  // "toggleWireframe"
        QT_MOC_LITERAL(86, 7),  // "checked"
        QT_MOC_LITERAL(94, 10),  // "toggleBBox"
        QT_MOC_LITERAL(105, 13),  // "onModelLoaded"
        QT_MOC_LITERAL(119, 8),  // "uint32_t"
        QT_MOC_LITERAL(128, 8),  // "vertices"
        QT_MOC_LITERAL(137, 9),  // "triangles"
        QT_MOC_LITERAL(147, 9),  // "appendLog"
        QT_MOC_LITERAL(157, 3)   // "msg"
    },
    "MainWindow",
    "openModel",
    "",
    "reloadModel",
    "resetView",
    "fitToModel",
    "takeScreenshot",
    "toggleWireframe",
    "checked",
    "toggleBBox",
    "onModelLoaded",
    "uint32_t",
    "vertices",
    "triangles",
    "appendLog",
    "msg"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_MainWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x08,    1 /* Private */,
       3,    0,   69,    2, 0x08,    2 /* Private */,
       4,    0,   70,    2, 0x08,    3 /* Private */,
       5,    0,   71,    2, 0x08,    4 /* Private */,
       6,    0,   72,    2, 0x08,    5 /* Private */,
       7,    1,   73,    2, 0x08,    6 /* Private */,
       9,    1,   76,    2, 0x08,    8 /* Private */,
      10,    2,   79,    2, 0x08,   10 /* Private */,
      14,    1,   84,    2, 0x08,   13 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 11,   12,   13,
    QMetaType::Void, QMetaType::QString,   15,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.offsetsAndSizes,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_MainWindow_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'openModel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'reloadModel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'resetView'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'fitToModel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'takeScreenshot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggleWireframe'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'toggleBBox'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onModelLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint32_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint32_t, std::false_type>,
        // method 'appendLog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->openModel(); break;
        case 1: _t->reloadModel(); break;
        case 2: _t->resetView(); break;
        case 3: _t->fitToModel(); break;
        case 4: _t->takeScreenshot(); break;
        case 5: _t->toggleWireframe((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 6: _t->toggleBBox((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 7: _t->onModelLoaded((*reinterpret_cast< std::add_pointer_t<uint32_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<uint32_t>>(_a[2]))); break;
        case 8: _t->appendLog((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
