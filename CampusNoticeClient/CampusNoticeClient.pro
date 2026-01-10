QT += core gui widgets network sql uitools concurrent charts
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

TARGET = CampusNoticeClient
TEMPLATE = app

# 头文件列表（确保所有.h文件都在）
HEADERS += \
    MainWindow.h \
    NoticeManager.h \
    NoticeModel.h \
    ChartManager.h \
    TrayManager.h \
    LogManager.h \
    MockServer.h

# 源文件列表（确保所有.cpp文件都在）
SOURCES += \
    main.cpp \
    MainWindow.cpp \
    NoticeManager.cpp \
    NoticeModel.cpp \
    ChartManager.cpp \
    TrayManager.cpp \
    LogManager.cpp \
    MockServer.cpp

# 禁用UI文件（纯代码UI，避免冲突）
# FORMS += MainWindow.ui

# 部署配置（Release模式输出）
CONFIG(release, debug|release): {
    DESTDIR = ./release
    OBJECTS_DIR = ./release/.obj
    MOC_DIR = ./release/.moc
    RCC_DIR = ./release/.rcc
} else {
    DESTDIR = ./debug
    OBJECTS_DIR = ./debug/.obj
    MOC_DIR = ./debug/.moc
    RCC_DIR = ./debug/.rcc
}

# Windows平台依赖（确保网络、数据库正常）
win32: LIBS += -lws2_32
