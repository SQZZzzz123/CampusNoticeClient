#include "http_manager.h"
#include <QTimer>
#include <QDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QDateTime>

HttpManager::HttpManager(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_requestTimeout(5000) // 默认超时时间5秒
{
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &HttpManager::onReplyFinished);
}

HttpManager::~HttpManager()
{
    cancelAllRequests();
    delete m_networkManager;
}

void HttpManager::setServerUrl(const QString &url)
{
    m_serverUrl = url;
    // 确保URL以斜杠结尾
    if (!m_serverUrl.endsWith("/")) {
        m_serverUrl += "/";
    }
}

QString HttpManager::getServerUrl() const
{
    return m_serverUrl;
}

void HttpManager::getNotices()
{
    QString url = m_serverUrl + "notices";
    QNetworkRequest request(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QNetworkReply *reply = m_networkManager->get(request);
    m_replies.append(reply);
    
    // 设置超时定时器
    QTimer::singleShot(m_requestTimeout, this, &HttpManager::onRequestTimeout);
}

void HttpManager::getNoticeDetail(int id)
{
    QString url = m_serverUrl + QString("notices/%1").arg(id);
    QNetworkRequest request(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QNetworkReply *reply = m_networkManager->get(request);
    m_replies.append(reply);
    
    // 设置超时定时器
    QTimer::singleShot(m_requestTimeout, this, &HttpManager::onRequestTimeout);
}

void HttpManager::setRequestTimeout(int timeout)
{
    m_requestTimeout = timeout;
}

void HttpManager::onReplyFinished(QNetworkReply *reply)
{
    // 从列表中移除已完成的请求
    m_replies.removeAll(reply);
    
    if (reply->error() != QNetworkReply::NoError) {
        emit requestFailed(reply->errorString());
        reply->deleteLater();
        return;
    }
    
    // 读取响应数据
    QByteArray responseData = reply->readAll();
    
    // 解析JSON
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        emit requestFailed(QString("JSON parse error: %1").arg(parseError.errorString()));
        reply->deleteLater();
        return;
    }
    
    // 根据URL路径判断是通知列表还是单个通知
    QString url = reply->url().toString();
    if (url.endsWith("/notices")) {
        // 通知列表
        QList<Notice> notices = parseNoticeListJson(jsonDoc);
        emit noticesReceived(notices);
    } else {
        // 单个通知详情
        if (jsonDoc.isObject()) {
            Notice notice = parseNoticeJson(jsonDoc.object());
            emit noticeDetailReceived(notice);
        } else {
            emit requestFailed("Invalid JSON format for notice detail");
        }
    }
    
    reply->deleteLater();
}

void HttpManager::onRequestTimeout()
{
    if (!m_replies.isEmpty()) {
        cancelAllRequests();
        emit requestTimeout();
    }
}

QList<Notice> HttpManager::parseNoticeListJson(const QJsonDocument &jsonDoc)
{
    QList<Notice> notices;
    
    if (!jsonDoc.isObject()) {
        return notices;
    }
    
    QJsonObject rootObj = jsonDoc.object();
    if (rootObj.contains("data") && rootObj["data"].isArray()) {
        QJsonArray noticesArray = rootObj["data"].toArray();
        
        for (const QJsonValue &value : noticesArray) {
            if (value.isObject()) {
                Notice notice = parseNoticeJson(value.toObject());
                notices.append(notice);
            }
        }
    }
    
    return notices;
}

Notice HttpManager::parseNoticeJson(const QJsonObject &jsonObj)
{
    Notice notice;
    
    // 解析基本字段
    if (jsonObj.contains("id") && jsonObj["id"].isDouble()) {
        notice.id = jsonObj["id"].toInt();
    }
    
    if (jsonObj.contains("title") && jsonObj["title"].isString()) {
        notice.title = jsonObj["title"].toString();
    }
    
    if (jsonObj.contains("content") && jsonObj["content"].isString()) {
        notice.content = jsonObj["content"].toString();
    }
    
    // 解析发布时间
    if (jsonObj.contains("publish_time") && jsonObj["publish_time"].isString()) {
        QString publishTimeStr = jsonObj["publish_time"].toString();
        notice.publishTime = QDateTime::fromString(publishTimeStr, Qt::ISODate);
    }
    
    // 解析类别
    if (jsonObj.contains("category") && jsonObj["category"].isDouble()) {
        int categoryInt = jsonObj["category"].toInt();
        notice.category = static_cast<NoticeCategory>(categoryInt);
    }
    
    if (jsonObj.contains("url") && jsonObj["url"].isString()) {
        notice.url = jsonObj["url"].toString();
    }
    
    // 默认未读
    notice.isRead = false;
    
    // 设置缓存时间为当前时间
    notice.cacheTime = QDateTime::currentDateTime();
    
    return notice;
}

void HttpManager::cancelAllRequests()
{
    for (QNetworkReply *reply : m_replies) {
        if (reply->isRunning()) {
            reply->abort();
        }
        reply->deleteLater();
    }
    m_replies.clear();
}
