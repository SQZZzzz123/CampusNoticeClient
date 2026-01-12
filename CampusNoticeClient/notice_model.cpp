#include "notice_model.h"
#include <QDebug>

NoticeModel::NoticeModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_currentCategory(NoticeCategory::All)
{
}

NoticeModel::~NoticeModel()
{
}

int NoticeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_filteredNotices.size();
}

QVariant NoticeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_filteredNotices.size()) {
        return QVariant();
    }

    const Notice &notice = m_filteredNotices.at(index.row());

    switch (role) {
    case IdRole:
        return notice.id;
    case TitleRole:
        return notice.title;
    case ContentRole:
        return notice.content;
    case PublishTimeRole:
        return notice.publishTime;
    case CategoryRole:
        return static_cast<int>(notice.category);
    case UrlRole:
        return notice.url;
    case IsReadRole:
        return notice.isRead;
    case CacheTimeRole:
        return notice.cacheTime;
    case Qt::DisplayRole:
        return notice.title;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> NoticeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[TitleRole] = "title";
    roles[ContentRole] = "content";
    roles[PublishTimeRole] = "publishTime";
    roles[CategoryRole] = "category";
    roles[UrlRole] = "url";
    roles[IsReadRole] = "isRead";
    roles[CacheTimeRole] = "cacheTime";
    return roles;
}

void NoticeModel::addNotice(const Notice &notice)
{
    beginInsertRows(QModelIndex(), m_notices.size(), m_notices.size());
    m_notices.append(notice);
    endInsertRows();

    // 如果符合当前过滤条件，添加到过滤列表
    if (m_currentCategory == NoticeCategory::All || m_currentCategory == notice.category) {
        beginInsertRows(QModelIndex(), m_filteredNotices.size(), m_filteredNotices.size());
        m_filteredNotices.append(notice);
        endInsertRows();

        // 发送通知数量变化信号
        emit noticeCountChanged();

        // 如果是未读通知，发送未读数量变化信号
        if (!notice.isRead) {
            emit unreadCountChanged();
        }
    }
}

void NoticeModel::addNotices(const QList<Notice> &notices)
{
    if (notices.isEmpty()) {
        return;
    }

    // 批量添加到通知列表
    beginInsertRows(QModelIndex(), m_notices.size(), m_notices.size() + notices.size() - 1);
    m_notices.append(notices);
    endInsertRows();

    // 过滤并添加到过滤列表
    QList<Notice> filteredNewNotices;
    for (const Notice &notice : notices) {
        if (m_currentCategory == NoticeCategory::All || m_currentCategory == notice.category) {
            filteredNewNotices.append(notice);
        }
    }

    if (!filteredNewNotices.isEmpty()) {
        beginInsertRows(QModelIndex(), m_filteredNotices.size(), m_filteredNotices.size() + filteredNewNotices.size() - 1);
        m_filteredNotices.append(filteredNewNotices);
        endInsertRows();

        // 发送通知数量变化信号
        emit noticeCountChanged();

        // 检查是否有未读通知
        bool hasUnread = false;
        for (const Notice &notice : filteredNewNotices) {
            if (!notice.isRead) {
                hasUnread = true;
                break;
            }
        }
        if (hasUnread) {
            emit unreadCountChanged();
        }
    }
}

void NoticeModel::removeNotice(int index)
{
    if (index < 0 || index >= m_filteredNotices.size()) {
        return;
    }

    // 获取要删除的通知
    Notice notice = m_filteredNotices.at(index);

    // 从过滤列表中删除
    beginRemoveRows(QModelIndex(), index, index);
    m_filteredNotices.removeAt(index);
    endRemoveRows();

    // 从通知列表中删除
    m_notices.removeAll(notice);

    // 发送通知数量变化信号
    emit noticeCountChanged();

    // 如果是未读通知，发送未读数量变化信号
    if (!notice.isRead) {
        emit unreadCountChanged();
    }
}

void NoticeModel::removeAllNotices()
{
    if (m_filteredNotices.isEmpty()) {
        return;
    }

    beginRemoveRows(QModelIndex(), 0, m_filteredNotices.size() - 1);
    m_filteredNotices.clear();
    m_notices.clear();
    endRemoveRows();

    // 发送信号
    emit noticeCountChanged();
    emit unreadCountChanged();
}

QList<Notice> NoticeModel::getNotices() const
{
    return m_notices;
}

Notice NoticeModel::getNotice(int index) const
{
    if (index < 0 || index >= m_filteredNotices.size()) {
        Notice invalidNotice;
        invalidNotice.id = -1;
        return invalidNotice;
    }
    return m_filteredNotices.at(index);
}

Notice NoticeModel::getNoticeById(int id) const
{
    for (const Notice &notice : m_notices) {
        if (notice.id == id) {
            return notice;
        }
    }

    Notice invalidNotice;
    invalidNotice.id = -1;
    return invalidNotice;
}

void NoticeModel::updateNotice(int index, const Notice &notice)
{
    if (index < 0 || index >= m_filteredNotices.size()) {
        return;
    }

    // 更新过滤列表中的通知
    Notice oldNotice = m_filteredNotices.at(index);
    m_filteredNotices[index] = notice;

    // 更新通知列表中的通知
    int originalIndex = m_notices.indexOf(oldNotice);
    if (originalIndex >= 0) {
        m_notices[originalIndex] = notice;
    }

    // 发送数据变化信号
    emit dataChanged(index(index), index(index));

    // 如果已读状态发生变化，发送未读数量变化信号
    if (oldNotice.isRead != notice.isRead) {
        emit unreadCountChanged();
    }
}

void NoticeModel::updateNoticeById(int id, const Notice &notice)
{
    // 查找通知在过滤列表中的索引
    int filteredIndex = -1;
    for (int i = 0; i < m_filteredNotices.size(); ++i) {
        if (m_filteredNotices.at(i).id == id) {
            filteredIndex = i;
            break;
        }
    }

    // 查找通知在通知列表中的索引
    int originalIndex = -1;
    for (int i = 0; i < m_notices.size(); ++i) {
        if (m_notices.at(i).id == id) {
            originalIndex = i;
            break;
        }
    }

    if (originalIndex >= 0) {
        Notice oldNotice = m_notices.at(originalIndex);
        m_notices[originalIndex] = notice;

        if (filteredIndex >= 0) {
            m_filteredNotices[filteredIndex] = notice;
            emit dataChanged(index(filteredIndex), index(filteredIndex));
        } else {
            // 如果通知不在过滤列表中，但已读状态变化，仍需发送未读数量变化信号
            if (oldNotice.isRead != notice.isRead) {
                emit unreadCountChanged();
            }
        }
    }
}

bool NoticeModel::markNoticeAsRead(int index)
{
    if (index < 0 || index >= m_filteredNotices.size()) {
        return false;
    }

    Notice notice = m_filteredNotices.at(index);
    if (notice.isRead) {
        return true; // 已经是已读状态
    }

    // 更新为已读
    notice.isRead = true;
    updateNotice(index, notice);
    return true;
}

bool NoticeModel::markNoticeAsReadById(int id)
{
    Notice notice = getNoticeById(id);
    if (notice.id == -1 || notice.isRead) {
        return false;
    }

    // 更新为已读
    notice.isRead = true;
    updateNoticeById(id, notice);
    return true;
}

bool NoticeModel::markAllNoticesAsRead()
{
    bool hasChanged = false;

    // 更新过滤列表中的所有通知
    for (int i = 0; i < m_filteredNotices.size(); ++i) {
        Notice &notice = m_filteredNotices[i];
        if (!notice.isRead) {
            notice.isRead = true;
            hasChanged = true;

            // 更新通知列表中的对应通知
            int originalIndex = m_notices.indexOf(notice);
            if (originalIndex >= 0) {
                m_notices[originalIndex].isRead = true;
            }
        }
    }

    if (hasChanged) {
        // 发送数据变化信号
        emit dataChanged(index(0), index(m_filteredNotices.size() - 1));
        
        // 发送未读数量变化信号
        emit unreadCountChanged();
    }

    return hasChanged;
}

void NoticeModel::filterByCategory(NoticeCategory category)
{
    m_currentCategory = category;

    // 重新过滤通知列表
    beginResetModel();
    m_filteredNotices.clear();
    
    for (const Notice &notice : m_notices) {
        if (category == NoticeCategory::All || category == notice.category) {
            m_filteredNotices.append(notice);
        }
    }
    endResetModel();

    // 发送通知数量变化信号
    emit noticeCountChanged();

    // 发送未读数量变化信号
    emit unreadCountChanged();
}

void NoticeModel::refreshData(const QList<Notice> &notices)
{
    // 清空现有数据
    beginResetModel();
    m_notices.clear();
    m_filteredNotices.clear();
    
    // 添加新数据
    m_notices = notices;
    
    // 过滤数据
    for (const Notice &notice : m_notices) {
        if (m_currentCategory == NoticeCategory::All || m_currentCategory == notice.category) {
            m_filteredNotices.append(notice);
        }
    }
    endResetModel();

    // 发送信号
    emit noticeCountChanged();
    emit unreadCountChanged();
}

int NoticeModel::getUnreadCount() const
{
    int count = 0;
    for (const Notice &notice : m_filteredNotices) {
        if (!notice.isRead) {
            count++;
        }
    }
    return count;
}
