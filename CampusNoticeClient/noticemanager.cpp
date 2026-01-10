#include "NoticeManager.h"
#include <QTimer>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QLoggingCategory>
#include <QCoreApplication>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>

Q_LOGGING_CATEGORY(noticeManager, "notice.manager")

NoticeManager::NoticeManager(QObject *parent)
    : QObject(parent)
    , networkManager(new QNetworkAccessManager(this))
    , pollingTimer(new QTimer(this))
    , serverUrl("http://mock.campus-notice.com/api/notices")
    , cacheCount(0)
{
    // 初始化数据库
    initializeDatabase();

    // 设置轮询定时器（5分钟）
    pollingTimer->setInterval(5 * 60 * 1000); // 5分钟
    connect(pollingTimer, &QTimer::timeout, this, &NoticeManager::fetchNotices);

    // 连接网络管理器的finished信号
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &NoticeManager::onNetworkReplyFinished);

    // 初始加载缓存数量
    updateCacheCount();
}

NoticeManager::~NoticeManager()
{
    stopPolling();
}

void NoticeManager::startPolling()
{
    if (!pollingTimer->isActive()) {
        qCInfo(noticeManager) << "Starting notice polling";
        pollingTimer->start();
        emit networkStatusChanged("Polling started");
    }
}

void NoticeManager::setPollingInterval(int intervalMs)
{
    if (pollingTimer->isActive()) {
        pollingTimer->stop();
        pollingTimer->setInterval(intervalMs);
        pollingTimer->start();
    } else {
        pollingTimer->setInterval(intervalMs);
    }

    qCInfo(noticeManager) << "Polling interval set to" << intervalMs << "ms";
}

void NoticeManager::stopPolling()
{
    if (pollingTimer->isActive()) {
        qCInfo(noticeManager) << "Stopping notice polling";
        pollingTimer->stop();
        emit networkStatusChanged("Polling stopped");
    }
}

void NoticeManager::fetchNotices()
{
    qCInfo(noticeManager) << "Fetching notices from server:" << serverUrl;
    emit networkStatusChanged("Fetching notices...");

    QNetworkRequest request;
    request.setUrl(QUrl(serverUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->get(request);
    // 这里不需要显式delete reply，因为onNetworkReplyFinished会处理
}

void NoticeManager::markAsRead(int id)
{
    QMutexLocker locker(&mutex);
    qCInfo(noticeManager) << "Marking notice as read:" << id;

    QSqlQuery query(database);
    query.prepare("UPDATE notices SET is_read = 1 WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        qCCritical(noticeManager) << "Failed to mark notice as read:" << query.lastError().text();
        return;
    }

    updateCacheCount();
    emit cacheCountChanged(cacheCount);
}

void NoticeManager::markAllAsRead()
{
    QMutexLocker locker(&mutex);
    qCInfo(noticeManager) << "Marking all notices as read";

    QSqlQuery query(database);
    query.prepare("UPDATE notices SET is_read = 1 WHERE is_read = 0");

    if (!query.exec()) {
        qCCritical(noticeManager) << "Failed to mark all notices as read:" << query.lastError().text();
        return;
    }

    updateCacheCount();
    emit cacheCountChanged(cacheCount);
}

void NoticeManager::clearReadNotices()
{
    QMutexLocker locker(&mutex);
    qCInfo(noticeManager) << "Clearing read notices";

    QSqlQuery query(database);
    query.prepare("DELETE FROM notices WHERE is_read = 1");

    if (!query.exec()) {
        qCCritical(noticeManager) << "Failed to clear read notices:" << query.lastError().text();
        return;
    }

    updateCacheCount();
    emit cacheCountChanged(cacheCount);
}

void NoticeManager::onNetworkReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        qCDebug(noticeManager) << "Received data from server, length:" << data.length();

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(data, &error);

        if (error.error != QJsonParseError::NoError) {
            qCWarning(noticeManager) << "Failed to parse JSON response:" << error.errorString();
            emit networkStatusChanged("Failed to parse server response");
            reply->deleteLater();
            return;
        }

        if (!doc.isArray()) {
            qCWarning(noticeManager) << "Server response is not an array";
            emit networkStatusChanged("Invalid server response format");
            reply->deleteLater();
            return;
        }

        QJsonArray noticesArray = doc.array();
        QList<Notice> notices;

        for (const auto &item : noticesArray) {
            if (!item.isObject()) continue;

            QJsonObject obj = item.toObject();
            Notice notice;
            notice.id = obj.value("id").toInt();
            notice.title = obj.value("title").toString();
            notice.content = obj.value("content").toString();
            notice.category = obj.value("category").toString();

            QString timeStr = obj.value("publish_time").toString();
            if (!timeStr.isEmpty()) {
                notice.publishTime = QDateTime::fromString(timeStr, Qt::ISODate);
            } else {
                notice.publishTime = QDateTime::currentDateTime();
            }

            notice.isRead = obj.value("is_read").toBool(false);
            notices.append(notice);
        }

        qCInfo(noticeManager) << "Parsed" << notices.size() << "notices from server";

        // 获取当前数据库中的通知ID列表，用于比较新通知
        QSet<int> currentIds;
        {
            QMutexLocker locker(&mutex);
            QSqlQuery query(database);
            query.prepare("SELECT id FROM notices");

            if (query.exec()) {
                while (query.next()) {
                    currentIds.insert(query.value(0).toInt());
                }
            }
        }

        // 找出新通知
        QList<Notice> newNotices;
        for (const Notice &notice : notices) {
            if (!currentIds.contains(notice.id)) {
                newNotices.append(notice);
            }
        }

        // 保存到数据库
        saveNoticesToDatabase(notices);

        // 如果有新通知，发送信号
        if (!newNotices.isEmpty()) {
            qCInfo(noticeManager) << "Found" << newNotices.size() << "new notices";
            emit newNoticesReceived(newNotices);
        }

        emit networkStatusChanged(QString("Successfully fetched %1 notices (%2 new)").arg(notices.size()).arg(newNotices.size()));
    } else {
        qCWarning(noticeManager) << "Network error:" << reply->errorString();
        emit networkStatusChanged(QString("Network error: %1").arg(reply->errorString()));
    }

    reply->deleteLater();
}

void NoticeManager::initializeDatabase()
{
    // 创建数据库连接
    database = QSqlDatabase::addDatabase("QSQLITE", "notice_db");

    // 设置数据库路径
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dbPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    database.setDatabaseName(dir.filePath("notices.db"));

    if (!database.open()) {
        qCCritical(noticeManager) << "Failed to open database:" << database.lastError().text();
        return;
    }

    // 创建表
    QSqlQuery query(database);
    bool success = query.exec("CREATE TABLE IF NOT EXISTS notices ("
                              "id INTEGER PRIMARY KEY,"
                              "title TEXT NOT NULL,"
                              "content TEXT NOT NULL,"
                              "category TEXT,"
                              "publish_time TEXT,"
                              "is_read INTEGER DEFAULT 0)");

    if (!success) {
        qCCritical(noticeManager) << "Failed to create notices table:" << query.lastError().text();
    } else {
        qCInfo(noticeManager) << "Database initialized successfully";
    }
}

void NoticeManager::saveNoticesToDatabase(const QList<Notice> &notices)
{
    QMutexLocker locker(&mutex);

    if (notices.isEmpty()) {
        return;
    }

    // 开始事务以提高性能
    QSqlQuery query(database);
    if (!database.transaction()) {
        qCCritical(noticeManager) << "Failed to start transaction:" << database.lastError().text();
        return;
    }

    // 准备插入语句
    query.prepare("INSERT OR REPLACE INTO notices "
                  "(id, title, content, category, publish_time, is_read) "
                  "VALUES (?, ?, ?, ?, ?, ?)");

    for (const Notice &notice : notices) {
        query.addBindValue(notice.id);
        query.addBindValue(notice.title);
        query.addBindValue(notice.content);
        query.addBindValue(notice.category);
        query.addBindValue(notice.publishTime.toString(Qt::ISODate));
        query.addBindValue(notice.isRead ? 1 : 0);

        if (!query.exec()) {
            qCWarning(noticeManager) << "Failed to insert notice:" << query.lastError().text();
            database.rollback();
            return;
        }
    }

    if (!database.commit()) {
        qCCritical(noticeManager) << "Failed to commit transaction:" << database.lastError().text();
        database.rollback();
    } else {
        qCInfo(noticeManager) << "Successfully saved" << notices.size() << "notices to database";
        updateCacheCount();
        emit cacheCountChanged(cacheCount);
    }
}

QList<Notice> NoticeManager::loadNoticesFromDatabase()
{
    QMutexLocker locker(&mutex);
    QList<Notice> notices;

    QSqlQuery query(database);
    query.prepare("SELECT id, title, content, category, publish_time, is_read FROM notices ORDER BY publish_time DESC");

    if (!query.exec()) {
        qCCritical(noticeManager) << "Failed to load notices from database:" << query.lastError().text();
        return notices;
    }

    while (query.next()) {
        Notice notice;
        notice.id = query.value(0).toInt();
        notice.title = query.value(1).toString();
        notice.content = query.value(2).toString();
        notice.category = query.value(3).toString();
        notice.publishTime = QDateTime::fromString(query.value(4).toString(), Qt::ISODate);
        notice.isRead = query.value(5).toBool();
        notices.append(notice);
    }

    qCInfo(noticeManager) << "Loaded" << notices.size() << "notices from database";
    return notices;
}

void NoticeManager::updateCacheCount()
{
    QMutexLocker locker(&mutex);

    QSqlQuery query(database);
    query.prepare("SELECT COUNT(*) FROM notices WHERE is_read = 0");

    if (!query.exec()) {
        qCCritical(noticeManager) << "Failed to get unread count:" << query.lastError().text();
        cacheCount = 0;
        return;
    }

    if (query.next()) {
        cacheCount = query.value(0).toInt();
    } else {
        cacheCount = 0;
    }

    qCInfo(noticeManager) << "Updated cache count:" << cacheCount;
}

// 新增：导出功能实现
bool NoticeManager::exportNoticesToCSV(const QList<Notice> &notices, const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCCritical(noticeManager) << "Failed to open file for export:" << filePath;
        return false;
    }

    QTextStream out(&file);
    // 设置UTF-8编码
    out.setCodec("UTF-8");

    // 写入CSV头部
    out << "ID,标题,类别,发布时间,已读状态,内容\n";

    // 写入数据
    for (const Notice &notice : notices) {
        // 转义内容中的逗号和换行符，以避免CSV格式问题
        QString escapedTitle = notice.title;
        escapedTitle.replace("\"", "\"\"");
        if (escapedTitle.contains(",") || escapedTitle.contains("\n") || escapedTitle.contains("\"")) {
            escapedTitle = "\"" + escapedTitle + "\"";
        }

        QString escapedCategory = notice.category;
        escapedCategory.replace("\"", "\"\"");
        if (escapedCategory.contains(",") || escapedCategory.contains("\n") || escapedCategory.contains("\"")) {
            escapedCategory = "\"" + escapedCategory + "\"";
        }

        QString escapedContent = notice.content;
        escapedContent.replace("\"", "\"\"");
        if (escapedContent.contains(",") || escapedContent.contains("\n") || escapedContent.contains("\"")) {
            escapedContent = "\"" + escapedContent + "\"";
        }

        out << QString("%1,%2,%3,%4,%5,%6\n")
                   .arg(notice.id)
                   .arg(escapedTitle)
                   .arg(escapedCategory)
                   .arg(notice.publishTime.toString("yyyy-MM-dd hh:mm:ss"))
                   .arg(notice.isRead ? "已读" : "未读")
                   .arg(escapedContent);
    }

    file.close();
    qCInfo(noticeManager) << "Successfully exported" << notices.size() << "notices to" << filePath;
    return true;
}

bool NoticeManager::exportAllNoticesToCSV(const QString &filePath)
{
    QMutexLocker locker(&mutex);

    QSqlQuery query(database);
    query.prepare("SELECT id, title, content, category, publish_time, is_read FROM notices ORDER BY publish_time DESC");

    if (!query.exec()) {
        qCCritical(noticeManager) << "Failed to load all notices for export:" << query.lastError().text();
        return false;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCCritical(noticeManager) << "Failed to open file for export:" << filePath;
        return false;
    }

    QTextStream out(&file);
    // 设置UTF-8编码
    out.setCodec("UTF-8");

    // 写入CSV头部
    out << "ID,标题,类别,发布时间,已读状态,内容\n";

    // 写入数据
    while (query.next()) {
        int id = query.value(0).toInt();
        QString title = query.value(1).toString();
        QString content = query.value(2).toString();
        QString category = query.value(3).toString();
        QDateTime publishTime = query.value(4).toDateTime();
        bool isRead = query.value(5).toBool();

        // 转义内容中的逗号和换行符，以避免CSV格式问题
        QString escapedTitle = title;
        escapedTitle.replace("\"", "\"\"");
        if (escapedTitle.contains(",") || escapedTitle.contains("\n") || escapedTitle.contains("\"")) {
            escapedTitle = "\"" + escapedTitle + "\"";
        }

        QString escapedCategory = category;
        escapedCategory.replace("\"", "\"\"");
        if (escapedCategory.contains(",") || escapedCategory.contains("\n") || escapedCategory.contains("\"")) {
            escapedCategory = "\"" + escapedCategory + "\"";
        }

        QString escapedContent = content;
        escapedContent.replace("\"", "\"\"");
        if (escapedContent.contains(",") || escapedContent.contains("\n") || escapedContent.contains("\"")) {
            escapedContent = "\"" + escapedContent + "\"";
        }

        out << QString("%1,%2,%3,%4,%5,%6\n")
                   .arg(id)
                   .arg(escapedTitle)
                   .arg(escapedCategory)
                   .arg(publishTime.toString("yyyy-MM-dd hh:mm:ss"))
                   .arg(isRead ? "已读" : "未读")
                   .arg(escapedContent);
    }

    file.close();
    qCInfo(noticeManager) << "Successfully exported all notices to" << filePath;
    return true;
}

// 新增：备份和恢复功能实现
bool NoticeManager::backupDatabase(const QString &backupPath)
{
    QMutexLocker locker(&mutex);

    // 确保备份目录存在
    QFileInfo fileInfo(backupPath);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qCCritical(noticeManager) << "Failed to create backup directory:" << dir.path();
            return false;
        }
    }

    // 关闭数据库连接
    if (database.isOpen()) {
        database.close();
    }

    // 复制数据库文件
    QString dbPath = database.databaseName();
    bool success = QFile::copy(dbPath, backupPath);

    // 重新打开数据库连接
    if (!database.open()) {
        qCCritical(noticeManager) << "Failed to reopen database after backup:" << database.lastError().text();
        return false;
    }

    if (success) {
        qCInfo(noticeManager) << "Database backed up successfully to" << backupPath;
    } else {
        qCCritical(noticeManager) << "Failed to backup database to" << backupPath;
    }

    return success;
}

bool NoticeManager::restoreDatabase(const QString &backupPath)
{
    QMutexLocker locker(&mutex);

    // 检查备份文件是否存在
    if (!QFile::exists(backupPath)) {
        qCCritical(noticeManager) << "Backup file does not exist:" << backupPath;
        return false;
    }

    // 关闭数据库连接
    if (database.isOpen()) {
        database.close();
    }

    // 复制备份文件到当前数据库位置
    QString dbPath = database.databaseName();
    QString tempPath = dbPath + ".tmp";

    // 先复制到临时文件
    bool copySuccess = QFile::copy(backupPath, tempPath);
    if (!copySuccess) {
        qCCritical(noticeManager) << "Failed to copy backup file to temp location:" << tempPath;
        // 重新打开原数据库
        database.open();
        return false;
    }

    // 删除原数据库文件
    QFile::remove(dbPath);

    // 将临时文件重命名为原数据库文件名
    bool renameSuccess = QFile::rename(tempPath, dbPath);
    if (!renameSuccess) {
        qCCritical(noticeManager) << "Failed to rename temp file to database file";
        // 恢复临时文件
        QFile::rename(tempPath, dbPath);
        // 重新打开原数据库
        database.open();
        return false;
    }

    // 重新打开数据库连接
    if (!database.open()) {
        qCCritical(noticeManager) << "Failed to reopen database after restore:" << database.lastError().text();
        return false;
    }

    qCInfo(noticeManager) << "Database restored successfully from" << backupPath;
    return true;
}
