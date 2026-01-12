#include "networkmanager.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &NetworkManager::onFinished);
}

void NetworkManager::fetchNotifications() {
    // 当前无真实网络API，返回空列表避免覆盖现有数据库数据
    // 实际开发中替换为真实网络请求
    QVector<Notification> emptyData;
    emit dataReady(emptyData);
}

void NetworkManager::onFinished(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        // 解析 JSON 并发送信号
        QByteArray response = reply->readAll();
        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &parseError);
        
        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "JSON解析错误:" << parseError.errorString();
            reply->deleteLater();
            return;
        }
        
        if (!jsonDoc.isArray()) {
            qDebug() << "JSON格式错误: 根节点不是数组";
            reply->deleteLater();
            return;
        }
        
        QJsonArray jsonArray = jsonDoc.array();
        QVector<Notification> data;
        
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
        
        emit dataReady(data);
    } else {
        qDebug() << "网络请求错误:" << reply->errorString();
    }
    reply->deleteLater();
}
