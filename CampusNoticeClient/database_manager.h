#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>
#include "notice.h"

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    // 初始化数据库
    bool initDatabase();

    // 创建通知表
    bool createNoticeTable();

    // 插入或更新通知
    bool insertOrUpdateNotice(const Notice &notice);

    // 批量插入或更新通知
    bool batchInsertOrUpdateNotices(const QList<Notice> &notices);

    // 查询通知（按类别）
    QList<Notice> getNotices(NoticeCategory category = NoticeCategory::All, int limit = -1, int offset = 0);

    // 查询单个通知
    Notice getNoticeById(int id);

    // 更新通知为已读
    bool markNoticeAsRead(int id);

    // 更新所有通知为已读
    bool markAllNoticesAsRead(NoticeCategory category = NoticeCategory::All);

    // 删除旧通知（保留指定天数）
    bool deleteOldNotices(int daysToKeep = 30);

    // 获取未读通知数量
    int getUnreadCount(NoticeCategory category = NoticeCategory::All);

private:
    QSqlDatabase m_database;
    bool openDatabase();
    void closeDatabase();
    NoticeCategory intToCategory(int categoryInt) const;
    int categoryToInt(NoticeCategory category) const;
};

#endif // DATABASE_MANAGER_H
