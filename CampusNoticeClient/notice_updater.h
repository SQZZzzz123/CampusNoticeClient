#ifndef NOTICE_UPDATER_H
#define NOTICE_UPDATER_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include "http_manager.h"
#include "database_manager.h"

class NoticeUpdater : public QObject
{
    Q_OBJECT
public:
    explicit NoticeUpdater(QObject *parent = nullptr);
    ~NoticeUpdater();

    // 初始化更新器
    void init(const QString &serverUrl, int updateInterval = 300000); // 默认5分钟

    // 启动更新器
    void start();

    // 停止更新器
    void stop();

    // 立即更新通知
    void updateNow();

    // 设置更新间隔（毫秒）
    void setUpdateInterval(int interval);

    // 获取更新间隔
    int getUpdateInterval() const;

    // 设置服务器地址
    void setServerUrl(const QString &url);

    // 获取服务器地址
    QString getServerUrl() const;

    // 设置是否自动更新
    void setAutoUpdate(bool autoUpdate);

    // 获取是否自动更新
    bool isAutoUpdate() const;

    // 获取数据库管理器
    DatabaseManager *getDatabaseManager();

    // 获取HTTP管理器
    HttpManager *getHttpManager();

    // 获取当前是否正在更新
    bool isUpdating() const;

    // 获取最后更新时间
    QDateTime getLastUpdateTime() const;

    // 获取未读通知数量
    int getUnreadCount(NoticeCategory category = NoticeCategory::All);

    // 标记通知为已读
    bool markNoticeAsRead(int id);

    // 标记所有通知为已读
    bool markAllNoticesAsRead(NoticeCategory category = NoticeCategory::All);

    // 删除旧通知
    bool deleteOldNotices(int daysToKeep = 30);

signals:
    // 更新开始信号
    void updateStarted();

    // 更新完成信号
    void updateFinished(int newNoticeCount);

    // 更新失败信号
    void updateFailed(const QString &errorString);

    // 通知数量变化信号
    void noticeCountChanged();

    // 未读通知数量变化信号
    void unreadCountChanged();

private slots:
    // 处理定时器超时，执行更新
    void onTimerTimeout();

    // 处理通知接收成功
    void onNoticesReceived(const QList<Notice> &notices);

    // 处理通知接收失败
    void onRequestFailed(const QString &errorString);

    // 处理请求超时
    void onRequestTimeout();

private:
    // 初始化数据库
    bool initDatabase();

    // 计算新增通知数量
    int calculateNewNoticeCount(const QList<Notice> &notices);

    QTimer *m_timer;
    HttpManager *m_httpManager;
    DatabaseManager *m_databaseManager;
    QString m_serverUrl;
    int m_updateInterval;
    bool m_autoUpdate;
    bool m_isUpdating;
    QDateTime m_lastUpdateTime;
    QThread m_workerThread;
};

#endif // NOTICE_UPDATER_H
