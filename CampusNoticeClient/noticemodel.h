#ifndef NOTICEMODEL_H
#define NOTICEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QVariant>
#include <QDateTime>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlField>

// 通知数据结构
struct Notice {
    int id;
    QString title;
    QString content;
    QString category;
    QDateTime publishTime;
    bool isRead;

    Notice() : id(0), isRead(false) {}
    Notice(int _id, const QString &_title, const QString &_content,
           const QString &_category, const QDateTime &_publishTime, bool _isRead = false)
        : id(_id), title(_title), content(_content),
        category(_category), publishTime(_publishTime), isRead(_isRead) {}
};

class NoticeModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit NoticeModel(QObject *parent = nullptr);

    // QAbstractTableModel接口实现
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // 设置数据的方法
    void setNotices(const QList<Notice> ¬ices);
    const QList<Notice>& getNotices() const { return m_notices; }

    // 获取特定索引的通知
    Notice getNoticeAt(int row) const;

    // 更新通知的已读状态
    void updateNoticeReadStatus(int noticeId, bool isRead);

    // 从数据库加载数据
    void loadData();

    // 获取指定行的数据库记录
    QSqlRecord record(int row) const;

private:
    // 辅助函数，创建QSqlField
    QSqlField createField(const QString &name, const QVariant &value) const;

    QList<Notice> m_notices;
    QStringList m_headers;
};

#endif // NOTICEMODEL_H
