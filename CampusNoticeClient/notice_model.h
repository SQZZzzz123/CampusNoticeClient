#ifndef NOTICE_MODEL_H
#define NOTICE_MODEL_H

#include <QAbstractListModel>
#include <QList>
#include "notice.h"

class NoticeModel : public QAbstractListModel
{
    Q_OBJECT
public:
    // 定义数据角色
    enum NoticeRole {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        ContentRole,
        PublishTimeRole,
        CategoryRole,
        UrlRole,
        IsReadRole,
        CacheTimeRole
    };

    explicit NoticeModel(QObject *parent = nullptr);
    ~NoticeModel();

    // 重写QAbstractListModel的纯虚函数
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // 添加通知
    void addNotice(const Notice &notice);
    void addNotices(const QList<Notice> &notices);

    // 移除通知
    void removeNotice(int index);
    void removeAllNotices();

    // 获取通知列表
    QList<Notice> getNotices() const;

    // 获取单个通知
    Notice getNotice(int index) const;
    Notice getNoticeById(int id) const;

    // 更新通知
    void updateNotice(int index, const Notice &notice);
    void updateNoticeById(int id, const Notice &notice);

    // 标记通知为已读
    bool markNoticeAsRead(int index);
    bool markNoticeAsReadById(int id);
    bool markAllNoticesAsRead();

    // 按类别过滤通知
    void filterByCategory(NoticeCategory category);

    // 刷新模型数据
    void refreshData(const QList<Notice> &notices);

signals:
    // 通知数量变化信号
    void noticeCountChanged();

    // 未读通知数量变化信号
    void unreadCountChanged();

private:
    // 通知列表
    QList<Notice> m_notices;

    // 过滤后的通知列表
    QList<Notice> m_filteredNotices;

    // 当前过滤类别
    NoticeCategory m_currentCategory;

    // 获取未读通知数量
    int getUnreadCount() const;
};

#endif // NOTICE_MODEL_H
