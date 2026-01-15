QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# 应用程序类型配置
# 默认编译为推送端
CONFIG += pusher

# 推送端配置
pusher {
    TARGET = CampusNotifier_Pusher
    DEFINES += APP_TYPE=AppType::Pusher
}

# 接收端配置
receiver {
    TARGET = CampusNotifier_Receiver
    DEFINES += APP_TYPE=AppType::Receiver
}

# 通用配置
HEADERS += \
    appconfig.h \
    databasehelper.h \
    mainwindow.h \
    networkmanager.h \
    notificationdetaildialog.h \
    notificationeditdialog.h \
    notificationitem.h

SOURCES += \
    databasehelper.cpp \
    main.cpp \
    mainwindow.cpp \
    networkmanager.cpp \
    notificationdetaildialog.cpp \
    notificationeditdialog.cpp

FORMS += \
    mainwindow.ui \
    notificationdetaildialog.ui \
    notificationeditdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
