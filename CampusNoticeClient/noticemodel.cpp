#include "NoticeModel.h"
#include <QColor>
#include <QBrush>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

NoticeModel::NoticeModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    // 设置表头
    m_headers << "ID" << "标题" << "类别" << "发布时间" << "已读状态";
}

int NoticeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_notices.size();
}

int NoticeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_headers.size(); // 5列：ID, 标题, 类别, 发布时间, 已读状态
}

QVariant NoticeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_notices.size())
        return QVariant();

    const Notice ¬ice = m_notices[index.row()];

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: // ID
            return notice.id;
        case 1: // 标题
            return notice.title;
        case 2: // 类别
            return notice.category;
        case 3: // 发布时间
            return notice.publishTime.toString("yyyy-MM-dd hh:mm:ss");
        case 4: // 已读状态
            return notice.isRead ? "已读" : "未读";
        default:
            return QVariant();
        }
    case Qt::EditRole:
        // 如果需要编辑功能，可以在这里返回可编辑的数据
        if (index.column() == 0) {
            return notice.id;
        }
        return QVariant();
    case Qt::TextAlignmentRole:
        if (index.column() == 0) { // ID居中
            return Qt::AlignCenter;
        } else if (index.column() == 4) { // 已读状态居中
            return Qt::AlignCenter;
        }
        return Qt::AlignLeft | Qt::AlignVCenter;
    case Qt::ForegroundRole:
        // 如果通知未读，使用不同颜色标识
        if (index.column() != 4 && !notice.isRead) { // 不是"已读状态"列且未读
            return QColor(0, 120, 215); // Windows蓝色
        }
        return QVariant();
    case Qt::CheckStateRole:
        if (index.column() == 4) { // 已读状态列
            return notice.isRead ? Qt::Checked : Qt::Unchecked;
        }
        return QVariant();
    default:
        return QVariant();
    }
}

QVariant NoticeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section >= 0 && section < m_headers.size()) {
            return m_headers[section];
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

void NoticeModel::setNotices(const QList<Notice> ¬ices)
{
    beginResetModel();
    m_notices = notices;
    endResetModel();
}

Notice NoticeModel::getNoticeAt(int row) const
{
    if (row >= 0 && row < m_notices.size()) {
        return m_notices[row];
    }
    return Notice();
}

void NoticeModel::updateNoticeReadStatus(int noticeId, bool isRead)
{
    for (int i = 0; i < m_notices.size(); ++i) {
        if (m_notices[i].id == noticeId) {
            m_notices[i].isRead = isRead;

            // 发射数据改变信号，更新特定行的第4列（已读状态）
            QModelIndex topLeft = index(i, 4);
            QModelIndex bottomRight = index(i, 4);
            emit dataChanged(topLeft, bottomRight, {Qt::DisplayRole, Qt::CheckStateRole});

            // 如果需要更新整个行的显示（包括颜色变化）
            topLeft = index(i, 0);
            bottomRight = index(i, 3);
            emit dataChanged(topLeft, bottomRight, {Qt::ForegroundRole});
            break;
        }
    }
}

void NoticeModel::loadData()
{
    // 连接到数据库
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
        return;
    }

    // 查询通知数据
    QSqlQuery query(db);
    QString queryString = "SELECT id, title, content, category, publish_time, is_read FROM notices ORDER BY publish_time DESC";
    if (!query.exec(queryString)) {
        qWarning() << "Failed to execute query:" << query.lastError().text();
        return;
    }

    // 清空现有数据
    beginResetModel();
    m_notices.clear();

    // 填充数据
    while (query.next()) {
        Notice notice;
        notice.id = query.value("id").toInt();
        notice.title = query.value("title").toString();
        notice.content = query.value("content").toString();
        notice.category = query.value("category").toString();
        notice.publishTime = query.value("publish_time").toDateTime();
        notice.isRead = query.value("is_read").toBool();

        m_notices.append(notice);
    }

    endResetModel();
}

QSqlRecord NoticeModel::record(int row) const
{
    if (row < 0 || row >= m_notices.size()) {
        return QSqlRecord(); // 返回空记录
    }

    // 创建一个模拟的QSqlRecord
    QSqlRecord record;

    // 添加字段和值
    record.append(createField("id", m_notices[row].id));
    record.append(createField("title", m_notices[row].title));
    record.append(createField("content", m_notices[row].content));
    record.append(createField("category", m_notices[row].category));
    record.append(createField("publish_time", m_notices[row].publishTime));
    record.append(createField("is_read", m_notices[row].isRead));

    return record;
}

// 辅助函数，创建QSqlField
QSqlField NoticeModel::createField(const QString &name, const QVariant &value) const
{
    QSqlField field(name, value.type());
    field.setValue(value);
    return field;
}
