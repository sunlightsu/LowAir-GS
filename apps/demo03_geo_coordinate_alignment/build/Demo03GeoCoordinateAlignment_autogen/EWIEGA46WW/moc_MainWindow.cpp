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
    uint offsetsAndSizes[34];
    char stringdata0[11];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[11];
    char stringdata4[10];
    char stringdata5[14];
    char stringdata6[18];
    char stringdata7[12];
    char stringdata8[18];
    char stringdata9[17];
    char stringdata10[17];
    char stringdata11[16];
    char stringdata12[12];
    char stringdata13[6];
    char stringdata14[9];
    char stringdata15[4];
    char stringdata16[11];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MainWindow_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 12),  // "onLoadConfig"
        QT_MOC_LITERAL(24, 0),  // ""
        QT_MOC_LITERAL(25, 10),  // "onStartUdp"
        QT_MOC_LITERAL(36, 9),  // "onStopUdp"
        QT_MOC_LITERAL(46, 13),  // "onResetOrigin"
        QT_MOC_LITERAL(60, 17),  // "onClearTrajectory"
        QT_MOC_LITERAL(78, 11),  // "onResetView"
        QT_MOC_LITERAL(90, 17),  // "onFitToTrajectory"
        QT_MOC_LITERAL(108, 16),  // "onTakeScreenshot"
        QT_MOC_LITERAL(125, 16),  // "onValidateOrigin"
        QT_MOC_LITERAL(142, 15),  // "onStateReceived"
        QT_MOC_LITERAL(158, 11),  // "GeoUavState"
        QT_MOC_LITERAL(170, 5),  // "state"
        QT_MOC_LITERAL(176, 8),  // "onUdpLog"
        QT_MOC_LITERAL(185, 3),  // "msg"
        QT_MOC_LITERAL(189, 10)   // "onUdpError"
    },
    "MainWindow",
    "onLoadConfig",
    "",
    "onStartUdp",
    "onStopUdp",
    "onResetOrigin",
    "onClearTrajectory",
    "onResetView",
    "onFitToTrajectory",
    "onTakeScreenshot",
    "onValidateOrigin",
    "onStateReceived",
    "GeoUavState",
    "state",
    "onUdpLog",
    "msg",
    "onUdpError"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_MainWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   86,    2, 0x08,    1 /* Private */,
       3,    0,   87,    2, 0x08,    2 /* Private */,
       4,    0,   88,    2, 0x08,    3 /* Private */,
       5,    0,   89,    2, 0x08,    4 /* Private */,
       6,    0,   90,    2, 0x08,    5 /* Private */,
       7,    0,   91,    2, 0x08,    6 /* Private */,
       8,    0,   92,    2, 0x08,    7 /* Private */,
       9,    0,   93,    2, 0x08,    8 /* Private */,
      10,    0,   94,    2, 0x08,    9 /* Private */,
      11,    1,   95,    2, 0x08,   10 /* Private */,
      14,    1,   98,    2, 0x08,   12 /* Private */,
      16,    1,  101,    2, 0x08,   14 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, QMetaType::QString,   15,
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
        // method 'onLoadConfig'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onStartUdp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onStopUdp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onResetOrigin'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClearTrajectory'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onResetView'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFitToTrajectory'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTakeScreenshot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onValidateOrigin'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onStateReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const GeoUavState &, std::false_type>,
        // method 'onUdpLog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onUdpError'
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
        case 0: _t->onLoadConfig(); break;
        case 1: _t->onStartUdp(); break;
        case 2: _t->onStopUdp(); break;
        case 3: _t->onResetOrigin(); break;
        case 4: _t->onClearTrajectory(); break;
        case 5: _t->onResetView(); break;
        case 6: _t->onFitToTrajectory(); break;
        case 7: _t->onTakeScreenshot(); break;
        case 8: _t->onValidateOrigin(); break;
        case 9: _t->onStateReceived((*reinterpret_cast< std::add_pointer_t<GeoUavState>>(_a[1]))); break;
        case 10: _t->onUdpLog((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->onUdpError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
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
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
