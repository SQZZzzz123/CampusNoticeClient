#ifndef HTTP_MANAGER_H
#define HTTP_MANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "notice.h"

class HttpManager : public QObject
{
    Q_OBJECT
public:
    explicit HttpManager(QObject *parent = nullptr);
    ~HttpManager();

    // 设置服务器地址
    void setServerUrl(const QString &url);

    // 获取服务器地址
    QString getServerUrl() const;

    // 发送GET请求获取通知列表
    void getNotices();

    // 发送GET请求获取单个通知详情
    void getNoticeDetail(int id);

    // 设置请求超时时间（毫秒）
    void setRequestTimeout(int timeout);

signals:
    // 通知列表获取成功信号
    void noticesReceived(const QList<Notice> &notices);

    // 单个通知详情获取成功信号
    void noticeDetailReceived(const Notice &notice);

    // 请求失败信号
    void requestFailed(const QString &errorString);

    // 请求超时信号
    void requestTimeout();

private slots:
    // 处理请求完成
    void onReplyFinished(QNetworkReply *reply);

    // 处理请求超时
    void onRequestTimeout();

private:
    // 解析通知列表JSON
    QList<Notice> parseNoticeListJson(const QJsonDocument &jsonDoc);

    // 解析单个通知JSON
    Notice parseNoticeJson(const QJsonObject &jsonObj);

    // 取消所有请求
    void cancelAllRequests();

    QNetworkAccessManager *m_networkManager;
    QString m_serverUrl;
    int m_requestTimeout;
    QList<QNetworkReply*> m_replies;
};

#endif // HTTP_MANAGER_H
