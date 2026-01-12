#include "database_manager.h"
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
{
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

bool DatabaseManager::initDatabase()
{
    // 设置数据库连接
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    
    // 获取应用数据目录
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!QDir(dataDir).exists()) {
        QDir().mkpath(dataDir);
    }
    
    // 设置数据库文件路径
    QString dbPath = dataDir + "/campus_notice.db";
    m_database.setDatabaseName(dbPath);
    
    // 打开数据库并创建表
    if (openDatabase()) {
        return createNoticeTable();
    }
    return false;
}

bool DatabaseManager::openDatabase()
{
    if (!m_database.open()) {
        qDebug() << "Failed to open database:" << m_database.lastError().text();
        return false;
    }
    return true;
}

void DatabaseManager::closeDatabase()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

bool DatabaseManager::createNoticeTable()
{
    QSqlQuery query;
    QString createTableSql = QString("CREATE TABLE IF NOT EXISTS %1 (\n").arg(NOTICE_TABLE) +
                              QString("    %1 INTEGER PRIMARY KEY AUTOINCREMENT,\n").arg(ID_FIELD) +
                              QString("    %1 TEXT NOT NULL,\n").arg(TITLE_FIELD) +
                              QString("    %1 TEXT,\n").arg(CONTENT_FIELD) +
                              QString("    %1 DATETIME NOT NULL,\n").arg(PUBLISH_TIME_FIELD) +
                              QString("    %1 INTEGER NOT NULL,\n").arg(CATEGORY_FIELD) +
                              QString("    %1 TEXT,\n").arg(URL_FIELD) +
                              QString("    %1 INTEGER DEFAULT 0,\n").arg(IS_READ_FIELD) +
                              QString("    %1 DATETIME DEFAULT CURRENT_TIMESTAMP\n").arg(CACHE_TIME_FIELD) +
                              ");";
    
    if (!query.exec(createTableSql)) {
        qDebug() << "Failed to create notice table:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::insertOrUpdateNotice(const Notice &notice)
{
    QSqlQuery query;
    
    // 检查通知是否已存在
    query.prepare(QString("SELECT COUNT(*) FROM %1 WHERE %2 = ?").arg(NOTICE_TABLE).arg(ID_FIELD));
    query.addBindValue(notice.id);
    if (!query.exec() || !query.next()) {
        qDebug() << "Failed to check notice existence:" << query.lastError().text();
        return false;
    }
    
    int count = query.value(0).toInt();
    QString sql;
    
    if (count > 0) {
        // 更新通知
        sql = QString("UPDATE %1 SET %2 = ?, %3 = ?, %4 = ?, %5 = ?, %6 = ?, %7 = ? WHERE %8 = ?").arg(NOTICE_TABLE)
              .arg(TITLE_FIELD).arg(CONTENT_FIELD).arg(PUBLISH_TIME_FIELD).arg(CATEGORY_FIELD)
              .arg(URL_FIELD).arg(CACHE_TIME_FIELD).arg(ID_FIELD);
    } else {
        // 插入通知
        sql = QString("INSERT INTO %1 (%2, %3, %4, %5, %6, %7, %8) VALUES (?, ?, ?, ?, ?, ?, ?)").arg(NOTICE_TABLE)
              .arg(TITLE_FIELD).arg(CONTENT_FIELD).arg(PUBLISH_TIME_FIELD).arg(CATEGORY_FIELD)
              .arg(URL_FIELD).arg(CACHE_TIME_FIELD).arg(ID_FIELD);
    }
    
    query.prepare(sql);
    query.addBindValue(notice.title);
    query.addBindValue(notice.content);
    query.addBindValue(notice.publishTime.toString(Qt::ISODate));
    query.addBindValue(categoryToInt(notice.category));
    query.addBindValue(notice.url);
    query.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));
    
    if (count > 0) {
        query.addBindValue(notice.id);
    } else {
        query.addBindValue(notice.id);
    }
    
    if (!query.exec()) {
        qDebug() << "Failed to insert/update notice:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool DatabaseManager::batchInsertOrUpdateNotices(const QList<Notice> &notices)
{
    m_database.transaction();
    
    for (const Notice &notice : notices) {
        if (!insertOrUpdateNotice(notice)) {
            m_database.rollback();
            return false;
        }
    }
    
    return m_database.commit();
}

QList<Notice> DatabaseManager::getNotices(NoticeCategory category, int limit, int offset)
{
    QList<Notice> notices;
    QSqlQuery query;
    
    QString whereClause;
    if (category != NoticeCategory::All) {
        whereClause = QString(" WHERE %1 = %2").arg(CATEGORY_FIELD).arg(categoryToInt(category));
    }
    
    QString limitClause;
    if (limit > 0) {
        limitClause = QString(" LIMIT %1 OFFSET %2").arg(limit).arg(offset);
    }
    
    QString selectSql = QString("SELECT %1, %2, %3, %4, %5, %6, %7, %8 FROM %9").arg(ID_FIELD).arg(TITLE_FIELD)
                       .arg(CONTENT_FIELD).arg(PUBLISH_TIME_FIELD).arg(CATEGORY_FIELD).arg(URL_FIELD)
                       .arg(IS_READ_FIELD).arg(CACHE_TIME_FIELD).arg(NOTICE_TABLE) +
                       whereClause +
                       QString(" ORDER BY %1 DESC").arg(PUBLISH_TIME_FIELD) +
                       limitClause;
    
    if (!query.exec(selectSql)) {
        qDebug() << "Failed to get notices:" << query.lastError().text();
        return notices;
    }
    
    while (query.next()) {
        Notice notice;
        notice.id = query.value(0).toInt();
        notice.title = query.value(1).toString();
        notice.content = query.value(2).toString();
        notice.publishTime = QDateTime::fromString(query.value(3).toString(), Qt::ISODate);
        notice.category = intToCategory(query.value(4).toInt());
        notice.url = query.value(5).toString();
        notice.isRead = query.value(6).toBool();
        notice.cacheTime = QDateTime::fromString(query.value(7).toString(), Qt::ISODate);
        notices.append(notice);
    }
    
    return notices;
}

Notice DatabaseManager::getNoticeById(int id)
{
    Notice notice;
    notice.id = -1; // 初始化ID为-1，表示未找到
    
    QSqlQuery query;
    QString selectSql = QString("SELECT %1, %2, %3, %4, %5, %6, %7 FROM %8 WHERE %9 = ?").arg(TITLE_FIELD)
                       .arg(CONTENT_FIELD).arg(PUBLISH_TIME_FIELD).arg(CATEGORY_FIELD).arg(URL_FIELD)
                       .arg(IS_READ_FIELD).arg(CACHE_TIME_FIELD).arg(NOTICE_TABLE).arg(ID_FIELD);
    
    query.prepare(selectSql);
    query.addBindValue(id);
    
    if (!query.exec() || !query.next()) {
        return notice;
    }
    
    notice.id = id;
    notice.title = query.value(0).toString();
    notice.content = query.value(1).toString();
    notice.publishTime = QDateTime::fromString(query.value(2).toString(), Qt::ISODate);
    notice.category = intToCategory(query.value(3).toInt());
    notice.url = query.value(4).toString();
    notice.isRead = query.value(5).toBool();
    notice.cacheTime = QDateTime::fromString(query.value(6).toString(), Qt::ISODate);
    
    return notice;
}

bool DatabaseManager::markNoticeAsRead(int id)
{
    QSqlQuery query;
    QString updateSql = QString("UPDATE %1 SET %2 = 1 WHERE %3 = ?").arg(NOTICE_TABLE).arg(IS_READ_FIELD).arg(ID_FIELD);
    
    query.prepare(updateSql);
    query.addBindValue(id);
    
    if (!query.exec()) {
        qDebug() << "Failed to mark notice as read:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool DatabaseManager::markAllNoticesAsRead(NoticeCategory category)
{
    QSqlQuery query;
    QString whereClause;
    if (category != NoticeCategory::All) {
        whereClause = QString(" WHERE %1 = %2").arg(CATEGORY_FIELD).arg(categoryToInt(category));
    }
    
    QString updateSql = QString("UPDATE %1 SET %2 = 1").arg(NOTICE_TABLE).arg(IS_READ_FIELD) + whereClause;
    
    if (!query.exec(updateSql)) {
        qDebug() << "Failed to mark all notices as read:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool DatabaseManager::deleteOldNotices(int daysToKeep)
{
    QSqlQuery query;
    QDateTime cutoffTime = QDateTime::currentDateTime().addDays(-daysToKeep);
    
    QString deleteSql = QString("DELETE FROM %1 WHERE %2 < ?").arg(NOTICE_TABLE).arg(CACHE_TIME_FIELD);
    query.prepare(deleteSql);
    query.addBindValue(cutoffTime.toString(Qt::ISODate));
    
    if (!query.exec()) {
        qDebug() << "Failed to delete old notices:" << query.lastError().text();
        return false;
    }
    
    return true;
}

int DatabaseManager::getUnreadCount(NoticeCategory category)
{
    QSqlQuery query;
    QString whereClause;
    
    if (category != NoticeCategory::All) {
        whereClause = QString(" WHERE %1 = %2 AND %3 = 0").arg(CATEGORY_FIELD).arg(categoryToInt(category)).arg(IS_READ_FIELD);
    } else {
        whereClause = QString(" WHERE %1 = 0").arg(IS_READ_FIELD);
    }
    
    QString countSql = QString("SELECT COUNT(*) FROM %1").arg(NOTICE_TABLE) + whereClause;
    
    if (!query.exec(countSql) || !query.next()) {
        qDebug() << "Failed to get unread count:" << query.lastError().text();
        return 0;
    }
    
    return query.value(0).toInt();
}

NoticeCategory DatabaseManager::intToCategory(int categoryInt) const
{
    return static_cast<NoticeCategory>(categoryInt);
}

int DatabaseManager::categoryToInt(NoticeCategory category) const
{
    return static_cast<int>(category);
}
