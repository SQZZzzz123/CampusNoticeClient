#ifndef NOTICEMANAGER_H
#define NOTICEMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QFuture>
#include <QFutureWatcher>

struct Notice {
    int id;
    QString title;
    QString content;
    QString category;
    QDateTime publishTime;
    bool isRead;
};

class NoticeManager : public QObject
{
    Q_OBJECT

public:
    explicit NoticeManager(QObject *parent = nullptr);
    ~NoticeManager();

    void startPolling();
    void stopPolling();
    void setPollingInterval(int intervalMs); // 新增：设置轮询间隔

    // 新增：导出功能
    bool exportNoticesToCSV(const QList<Notice> &notices, const QString &filePath);
    bool exportAllNoticesToCSV(const QString &filePath);

    // 新增：备份和恢复功能
    bool backupDatabase(const QString &backupPath);
    bool restoreDatabase(const QString &backupPath);

signals:
    void networkStatusChanged(const QString &status);
    void cacheCountChanged(int count);
    void newNoticesReceived(const QList<Notice> &newNotices);  // 新增：新通知到达信号

public slots:
    void fetchNotices();
    void markAsRead(int id);
    void markAllAsRead();
    void clearReadNotices();

private slots:
    void onNetworkReplyFinished(QNetworkReply *reply);

private:
    void initializeDatabase();
    void saveNoticesToDatabase(const QList<Notice> &notices);
    QList<Notice> loadNoticesFromDatabase();
    void updateCacheCount();

    QNetworkAccessManager *networkManager;
    QTimer *pollingTimer;
    QSqlDatabase database;
    QString serverUrl;
    int cacheCount;
    QMutex mutex;
};

#endif // NOTICEMANAGER_H
