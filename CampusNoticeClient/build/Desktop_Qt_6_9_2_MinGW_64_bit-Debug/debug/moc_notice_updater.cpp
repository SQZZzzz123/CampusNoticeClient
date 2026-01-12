/****************************************************************************
** Meta object code from reading C++ file 'notice_updater.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../notice_updater.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'notice_updater.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13NoticeUpdaterE_t {};
} // unnamed namespace

template <> constexpr inline auto NoticeUpdater::qt_create_metaobjectdata<qt_meta_tag_ZN13NoticeUpdaterE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "NoticeUpdater",
        "updateStarted",
        "",
        "updateFinished",
        "newNoticeCount",
        "updateFailed",
        "errorString",
        "noticeCountChanged",
        "unreadCountChanged",
        "onTimerTimeout",
        "onNoticesReceived",
        "QList<Notice>",
        "notices",
        "onRequestFailed",
        "onRequestTimeout"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'updateStarted'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'updateFinished'
        QtMocHelpers::SignalData<void(int)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 4 },
        }}),
        // Signal 'updateFailed'
        QtMocHelpers::SignalData<void(const QString &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 6 },
        }}),
        // Signal 'noticeCountChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'unreadCountChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onTimerTimeout'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onNoticesReceived'
        QtMocHelpers::SlotData<void(const QList<Notice> &)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Slot 'onRequestFailed'
        QtMocHelpers::SlotData<void(const QString &)>(13, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 6 },
        }}),
        // Slot 'onRequestTimeout'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<NoticeUpdater, qt_meta_tag_ZN13NoticeUpdaterE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject NoticeUpdater::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13NoticeUpdaterE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13NoticeUpdaterE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13NoticeUpdaterE_t>.metaTypes,
    nullptr
} };

void NoticeUpdater::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<NoticeUpdater *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->updateStarted(); break;
        case 1: _t->updateFinished((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->updateFailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->noticeCountChanged(); break;
        case 4: _t->unreadCountChanged(); break;
        case 5: _t->onTimerTimeout(); break;
        case 6: _t->onNoticesReceived((*reinterpret_cast< std::add_pointer_t<QList<Notice>>>(_a[1]))); break;
        case 7: _t->onRequestFailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->onRequestTimeout(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (NoticeUpdater::*)()>(_a, &NoticeUpdater::updateStarted, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoticeUpdater::*)(int )>(_a, &NoticeUpdater::updateFinished, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoticeUpdater::*)(const QString & )>(_a, &NoticeUpdater::updateFailed, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoticeUpdater::*)()>(_a, &NoticeUpdater::noticeCountChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoticeUpdater::*)()>(_a, &NoticeUpdater::unreadCountChanged, 4))
            return;
    }
}

const QMetaObject *NoticeUpdater::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NoticeUpdater::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13NoticeUpdaterE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NoticeUpdater::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void NoticeUpdater::updateStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NoticeUpdater::updateFinished(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void NoticeUpdater::updateFailed(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void NoticeUpdater::noticeCountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void NoticeUpdater::unreadCountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
