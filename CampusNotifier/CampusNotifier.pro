QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    databasehelper.cpp \
    main.cpp \
    mainwindow.cpp \
    networkmanager.cpp \
    notificationdetaildialog.cpp \
    notificationeditdialog.cpp

HEADERS += \
    databasehelper.h \
    mainwindow.h \
    networkmanager.h \
    notificationitem.h \
    notificationdetaildialog.h \
    notificationeditdialog.h

FORMS += \
    mainwindow.ui \
    notificationdetaildialog.ui \
    notificationeditdialog.ui
