#include "networkmanager.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QTimer>
#include <QCoreApplication>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &NetworkManager::onFinished);
}

void NetworkManager::fetchNotifications() {
    // 模拟从服务器获取通知的网络请求
    // 在实际应用中，这里应该是一个真实的GET请求
    qDebug() << "正在从服务器获取通知...";
    
    // 模拟网络延迟
    QTimer::singleShot(500, [this]() {
        // 这里使用模拟数据，实际应用中应该解析服务器返回的JSON
        // 为了演示，我们直接返回一个空数组，实际应用中应该从服务器获取数据
        // 注意：在真实应用中，应该使用真实的网络请求
        
        // 由于没有真实服务器，我们使用一个特殊的模拟方式
        // 发送一个空信号，触发客户端重新加载数据
        // 实际应用中，这里应该发送从服务器获取到的通知数据
        emit syncCompleted();
    });
}

void NetworkManager::addNotificationToServer(const Notification& notification) {
    // 模拟向服务器添加通知
    qDebug() << "正在向服务器添加通知:" << notification.title;
    
    // 模拟网络延迟
    QTimer::singleShot(500, [this]() {
        qDebug() << "通知添加到服务器成功";
        emit notificationAdded(true);
        // 添加成功后，触发同步，让所有客户端更新
        fetchNotifications();
    });
}

void NetworkManager::updateNotificationOnServer(const Notification& notification) {
    // 模拟更新服务器上的通知
    qDebug() << "正在更新服务器上的通知:" << notification.title;
    
    // 模拟网络延迟
    QTimer::singleShot(500, [this]() {
        qDebug() << "通知更新到服务器成功";
        emit notificationUpdated(true);
        // 更新成功后，触发同步，让所有客户端更新
        fetchNotifications();
    });
}

void NetworkManager::deleteNotificationFromServer(int id) {
    // 模拟从服务器删除通知
    qDebug() << "正在从服务器删除通知，ID:" << id;
    
    // 模拟网络延迟
    QTimer::singleShot(500, [this]() {
        qDebug() << "通知从服务器删除成功";
        emit notificationDeleted(true);
        // 删除成功后，触发同步，让所有客户端更新
        fetchNotifications();
    });
}

void NetworkManager::onFinished(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QVector<Notification> data = parseNotificationsResponse(response);
        emit dataReady(data);
    } else {
        qDebug() << "网络请求错误:" << reply->errorString();
    }
    reply->deleteLater();
}

void NetworkManager::onNotificationOperationFinished(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &parseError);
        
        if (parseError.error == QJsonParseError::NoError) {
            QJsonObject obj = jsonDoc.object();
            bool success = obj["success"].toBool();
            
            // 根据请求类型发射不同的信号
            if (reply->operation() == QNetworkAccessManager::PostOperation) {
                emit notificationAdded(success);
            } else if (reply->operation() == QNetworkAccessManager::PutOperation) {
                emit notificationUpdated(success);
            } else if (reply->operation() == QNetworkAccessManager::DeleteOperation) {
                emit notificationDeleted(success);
            }
            
            if (success) {
                fetchNotifications(); // 同步更新
            }
        }
    } else {
        qDebug() << "通知操作失败:" << reply->errorString();
    }
    reply->deleteLater();
}

QVector<Notification> NetworkManager::parseNotificationsResponse(QByteArray response) {
    QVector<Notification> data;
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON解析错误:" << parseError.errorString();
        return data;
    }

    if (!jsonDoc.isArray()) {
        qDebug() << "JSON格式错误: 根节点不是数组";
        return data;
    }

    QJsonArray jsonArray = jsonDoc.array();
    
    for (const QJsonValue& value : jsonArray) {
        if (!value.isObject()) {
            qDebug() << "JSON格式错误: 数组元素不是对象";
            continue;
        }

        QJsonObject obj = value.toObject();
        // 检查必要字段是否存在
        if (!obj.contains("id") || !obj.contains("title") ||
            !obj.contains("category") || !obj.contains("date") ||
            !obj.contains("content")) {
            qDebug() << "JSON格式错误: 缺少必要字段";
            continue;
        }

        Notification item;
        item.id = obj["id"].toInt();
        item.title = obj["title"].toString();
        item.category = obj["category"].toString();
        item.date = obj["date"].toString();
        item.content = obj["content"].toString();
        data.append(item);
    }

    return data;
}
