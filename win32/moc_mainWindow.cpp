/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created: Mon Aug 5 16:03:57 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../win32/MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CToolButton[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_CToolButton[] = {
    "CToolButton\0"
};

void CToolButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CToolButton::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CToolButton::staticMetaObject = {
    { &QToolButton::staticMetaObject, qt_meta_stringdata_CToolButton,
      qt_meta_data_CToolButton, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CToolButton::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CToolButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CToolButton::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CToolButton))
        return static_cast<void*>(const_cast< CToolButton*>(this));
    return QToolButton::qt_metacast(_clname);
}

int CToolButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x09,
      37,   11,   11,   11, 0x09,
      57,   11,   11,   11, 0x09,
      76,   11,   11,   11, 0x09,
      99,   11,   11,   11, 0x09,
     122,   11,   11,   11, 0x09,
     153,   11,   11,   11, 0x09,
     189,  182,   11,   11, 0x09,
     248,  243,   11,   11, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0OnBtnProjectionClicked()\0"
    "OnBtnPauseClicked()\0OnBtnExitClicked()\0"
    "OnBtnTakeShotClicked()\0OnBtnScanningClicked()\0"
    "OnDisplaySrFrameRateValue(int)\0"
    "OnDisplaySrQualityValue(int)\0reason\0"
    "OnSysTrayActivated(QSystemTrayIcon::ActivationReason)\0"
    "item\0OnListItemDoubleClicked(QListWidgetItem*)\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->OnBtnProjectionClicked(); break;
        case 1: _t->OnBtnPauseClicked(); break;
        case 2: _t->OnBtnExitClicked(); break;
        case 3: _t->OnBtnTakeShotClicked(); break;
        case 4: _t->OnBtnScanningClicked(); break;
        case 5: _t->OnDisplaySrFrameRateValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->OnDisplaySrQualityValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->OnSysTrayActivated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 8: _t->OnListItemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
