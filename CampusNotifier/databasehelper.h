#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include <QSqlDatabase>
#include <QVector>
#include <QCoreApplication>
#include "notificationitem.h"

class DatabaseHelper {
public:
    DatabaseHelper();
    ~DatabaseHelper();

    bool initDatabase();
    void saveNotifications(const QVector<Notification>& list);
    QVector<Notification> getAllNotifications();

    // 新增方法：增加、修改、删除通知
    bool addNotification(const Notification& notification);
    bool updateNotification(const Notification& notification);
    bool deleteNotification(int id);
    // 获取最大ID，用于自动生成新ID
    int getMaxId();

private:
    QSqlDatabase database;
};

#endif
