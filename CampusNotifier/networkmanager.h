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

public slots:
    void fetchNotifications(); // 模拟抓取

private slots:
    void onFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager* manager;
};

#endif
