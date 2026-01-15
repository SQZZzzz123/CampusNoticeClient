#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "notificationitem.h"

class NetworkManager : public QObject {
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

signals:
    void dataReady(QVector<Notification> data);
    void notificationAdded(bool success);
    void notificationUpdated(bool success);
    void notificationDeleted(bool success);
    void syncCompleted();

public slots:
    void fetchNotifications(); // 从服务器获取通知
    void addNotificationToServer(const Notification& notification); // 添加通知到服务器
    void updateNotificationOnServer(const Notification& notification); // 更新服务器上的通知
    void deleteNotificationFromServer(int id); // 从服务器删除通知

private slots:
    void onFinished(QNetworkReply* reply);
    void onNotificationOperationFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager* manager;
    QVector<Notification> parseNotificationsResponse(QByteArray response);
};

#endif
