#ifndef NOTICE_H
#define NOTICE_H

#include <QDateTime>

// 通知类别枚举
enum class NoticeCategory {
    All = 0,       // 全部
    Announcement,  // 公告
    Event,         // 活动
    Academic,      // 学术
    Job,           // 就业
    Other          // 其他
};

// 通知结构体
struct Notice {
    int id;                     // 通知ID
    QString title;              // 标题
    QString content;            // 内容
    QDateTime publishTime;      // 发布时间
    NoticeCategory category;    // 类别
    QString url;                // 来源URL
    bool isRead;                // 是否已读
    QDateTime cacheTime;        // 缓存时间
};

// 数据库表名和字段名常量
const QString NOTICE_TABLE = "notices";
const QString ID_FIELD = "id";
const QString TITLE_FIELD = "title";
const QString CONTENT_FIELD = "content";
const QString PUBLISH_TIME_FIELD = "publish_time";
const QString CATEGORY_FIELD = "category";
const QString URL_FIELD = "url";
const QString IS_READ_FIELD = "is_read";
const QString CACHE_TIME_FIELD = "cache_time";

#endif // NOTICE_H
