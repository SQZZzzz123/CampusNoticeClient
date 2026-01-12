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
    void fetchNotifications(); // 模拟抓取

signals:
    void dataReady(QVector<Notification> data);

private slots:
    void onFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager* manager;
};

#endif
