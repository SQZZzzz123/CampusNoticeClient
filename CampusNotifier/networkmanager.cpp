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
    // 实际开发中替换为学校 API 地址
    QUrl url("http://api.example.com/notifications");
    manager->get(QNetworkRequest(url));

    // 模拟测试数据（如果没有真实接口，手动触发信号）
    QVector<Notification> mockData;
    mockData.append({101, "关于放假的通知", "行政", "2025-10-01", "国庆放假7天"});
    mockData.append({102, "讲座：AI未来", "学术", "2025-10-05", "地点：报告厅"});
    mockData.append({103, "食堂新菜品推出", "生活", "2025-10-03", "食堂三楼新增特色菜品"});
    emit dataReady(mockData);
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
