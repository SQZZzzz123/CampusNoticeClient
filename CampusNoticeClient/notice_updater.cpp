#include "notice_updater.h"
#include <QDebug>
#include <QDateTime>

NoticeUpdater::NoticeUpdater(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
    , m_httpManager(new HttpManager(this))
    , m_databaseManager(new DatabaseManager(this))
    , m_updateInterval(300000) // 默认5分钟
    , m_autoUpdate(true)
    , m_isUpdating(false)
{
    // 连接定时器信号
    connect(m_timer, &QTimer::timeout, this, &NoticeUpdater::onTimerTimeout);
    
    // 连接HTTP管理器信号
    connect(m_httpManager, &HttpManager::noticesReceived, this, &NoticeUpdater::onNoticesReceived);
    connect(m_httpManager, &HttpManager::requestFailed, this, &NoticeUpdater::onRequestFailed);
    connect(m_httpManager, &HttpManager::requestTimeout, this, &NoticeUpdater::onRequestTimeout);
    
    // 设置定时器为单次触发
    m_timer->setSingleShot(false);
}

NoticeUpdater::~NoticeUpdater()
{
    stop();
    delete m_timer;
    delete m_httpManager;
    delete m_databaseManager;
}

void NoticeUpdater::init(const QString &serverUrl, int updateInterval)
{
    m_serverUrl = serverUrl;
    m_updateInterval = updateInterval;
    
    // 初始化数据库
    if (!initDatabase()) {
        qDebug() << "Failed to initialize database";
        return;
    }
    
    // 设置HTTP管理器的服务器地址
    m_httpManager->setServerUrl(m_serverUrl);
    
    // 设置定时器间隔
    m_timer->setInterval(m_updateInterval);
}

bool NoticeUpdater::initDatabase()
{
    return m_databaseManager->initDatabase();
}

void NoticeUpdater::start()
{
    if (m_autoUpdate && !m_timer->isActive()) {
        m_timer->start();
    }
}

void NoticeUpdater::stop()
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }
}

void NoticeUpdater::updateNow()
{
    if (m_isUpdating) {
        return; // 已经在更新中，避免重复请求
    }
    
    m_isUpdating = true;
    emit updateStarted();
    
    // 发送请求获取最新通知
    m_httpManager->getNotices();
}

void NoticeUpdater::setUpdateInterval(int interval)
{
    m_updateInterval = interval;
    m_timer->setInterval(m_updateInterval);
}

int NoticeUpdater::getUpdateInterval() const
{
    return m_updateInterval;
}

void NoticeUpdater::setServerUrl(const QString &url)
{
    m_serverUrl = url;
    m_httpManager->setServerUrl(m_serverUrl);
}

QString NoticeUpdater::getServerUrl() const
{
    return m_serverUrl;
}

void NoticeUpdater::setAutoUpdate(bool autoUpdate)
{
    m_autoUpdate = autoUpdate;
    
    if (m_autoUpdate && !m_timer->isActive()) {
        m_timer->start();
    } else if (!m_autoUpdate && m_timer->isActive()) {
        m_timer->stop();
    }
}

bool NoticeUpdater::isAutoUpdate() const
{
    return m_autoUpdate;
}

DatabaseManager *NoticeUpdater::getDatabaseManager()
{
    return m_databaseManager;
}

HttpManager *NoticeUpdater::getHttpManager()
{
    return m_httpManager;
}

bool NoticeUpdater::isUpdating() const
{
    return m_isUpdating;
}

QDateTime NoticeUpdater::getLastUpdateTime() const
{
    return m_lastUpdateTime;
}

int NoticeUpdater::getUnreadCount(NoticeCategory category)
{
    return m_databaseManager->getUnreadCount(category);
}

bool NoticeUpdater::markNoticeAsRead(int id)
{
    bool result = m_databaseManager->markNoticeAsRead(id);
    if (result) {
        emit unreadCountChanged();
    }
    return result;
}

bool NoticeUpdater::markAllNoticesAsRead(NoticeCategory category)
{
    bool result = m_databaseManager->markAllNoticesAsRead(category);
    if (result) {
        emit unreadCountChanged();
    }
    return result;
}

bool NoticeUpdater::deleteOldNotices(int daysToKeep)
{
    bool result = m_databaseManager->deleteOldNotices(daysToKeep);
    if (result) {
        emit noticeCountChanged();
    }
    return result;
}

void NoticeUpdater::onTimerTimeout()
{
    updateNow();
}

void NoticeUpdater::onNoticesReceived(const QList<Notice> &notices)
{
    if (notices.isEmpty()) {
        m_isUpdating = false;
        m_lastUpdateTime = QDateTime::currentDateTime();
        emit updateFinished(0);
        return;
    }
    
    // 计算新增通知数量
    int newNoticeCount = calculateNewNoticeCount(notices);
    
    // 批量插入或更新通知
    if (m_databaseManager->batchInsertOrUpdateNotices(notices)) {
        m_isUpdating = false;
        m_lastUpdateTime = QDateTime::currentDateTime();
        
        // 发送更新完成信号
        emit updateFinished(newNoticeCount);
        
        // 发送通知数量变化信号
        emit noticeCountChanged();
        
        // 如果有新增通知，发送未读数量变化信号
        if (newNoticeCount > 0) {
            emit unreadCountChanged();
        }
    } else {
        m_isUpdating = false;
        emit updateFailed("Failed to save notices to database");
    }
}

void NoticeUpdater::onRequestFailed(const QString &errorString)
{
    m_isUpdating = false;
    emit updateFailed(errorString);
}

void NoticeUpdater::onRequestTimeout()
{
    m_isUpdating = false;
    emit updateFailed("Request timeout");
}

int NoticeUpdater::calculateNewNoticeCount(const QList<Notice> &notices)
{
    int newCount = 0;
    
    for (const Notice &notice : notices) {
        // 查询数据库中是否存在该通知
        Notice existingNotice = m_databaseManager->getNoticeById(notice.id);
        if (existingNotice.id == -1) {
            // 不存在，说明是新通知
            newCount++;
        }
    }
    
    return newCount;
}
