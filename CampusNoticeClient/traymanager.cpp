#include "TrayManager.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QStyle>
#include <QTimer>
#include <QDateTime>
#include <QPainter>
#include <QPixmap>

// NotificationPopup 实现
NotificationPopup::NotificationPopup(QWidget *parent)
    : QWidget(parent, Qt::ToolTip | Qt::FramelessWindowHint)
    , titleLabel(nullptr)
    , messageLabel(nullptr)
    , closeButton(nullptr)
    , timer(nullptr)
    , fadeAnimation(nullptr)
    , noticeId(-1)
{
    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    initUI();

    // 设置定时器自动关闭
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &NotificationPopup::onTimeout);

    // 设置淡出动画
    fadeAnimation = new QPropertyAnimation(this, "windowOpacity");
    fadeAnimation->setDuration(500);
    fadeAnimation->setStartValue(1.0);
    fadeAnimation->setEndValue(0.0);
    connect(fadeAnimation, &QPropertyAnimation::finished, this, &NotificationPopup::closeNotification);
}

void NotificationPopup::initUI()
{
    setFixedSize(300, 120);
    setStyleSheet(
        "QWidget {"
        "    background-color: rgba(255, 255, 255, 0.95);"
        "    border: 1px solid #ccc;"
        "    border-radius: 8px;"
        "    padding: 10px;"
        "}"
        "QLabel {"
        "    color: #333;"
        "}"
        "QPushButton {"
        "    background-color: #f0f0f0;"
        "    border: 1px solid #ccc;"
        "    border-radius: 4px;"
        "    padding: 4px 8px;"
        "    min-width: 20px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #e0e0e0;"
        "}"
        );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(5);

    // 标题和消息布局
    QHBoxLayout *topLayout = new QHBoxLayout();

    QVBoxLayout *textLayout = new QVBoxLayout();
    titleLabel = new QLabel(this);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    titleLabel->setWordWrap(true);
    titleLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    messageLabel = new QLabel(this);
    messageLabel->setStyleSheet("font-size: 11px; color: #666;");
    messageLabel->setWordWrap(true);
    messageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    textLayout->addWidget(titleLabel);
    textLayout->addWidget(messageLabel);
    textLayout->addStretch();

    topLayout->addLayout(textLayout);

    // 关闭按钮
    closeButton = new QPushButton("×", this);
    closeButton->setFixedSize(20, 20);
    closeButton->setStyleSheet(
        "QPushButton {"
        "    font-weight: bold;"
        "    font-size: 14px;"
        "    border: none;"
        "    background: transparent;"
        "}"
        "QPushButton:hover {"
        "    background-color: #ff6b6b;"
        "    color: white;"
        "    border-radius: 10px;"
        "}"
        );

    connect(closeButton, &QPushButton::clicked, this, &NotificationPopup::closeNotification);

    topLayout->addWidget(closeButton);
    topLayout->setAlignment(closeButton, Qt::AlignTop);

    mainLayout->addLayout(topLayout);

    hide();
}

void NotificationPopup::showNotification(const QString &title, const QString &message, int duration)
{
    titleLabel->setText(title);
    messageLabel->setText(message);

    // 计算位置 - 右下角
    QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    QPoint pos(screenGeometry.right() - width() - 20, screenGeometry.bottom() - height() - 20);
    move(pos);

    show();
    raise();
    activateWindow();

    // 启动定时器
    timer->start(duration);
}

void NotificationPopup::closeNotification()
{
    hide();
    timer->stop();
}

void NotificationPopup::onTimeout()
{
    startFadeOut();
}

void NotificationPopup::startFadeOut()
{
    fadeAnimation->start();
}

// TrayManager 实现
TrayManager::TrayManager(QObject *parent)
    : QObject(parent)
    , trayIcon(nullptr)
    , trayMenu(nullptr)
    , restoreAction(nullptr)
    , quitAction(nullptr)
    , notificationPopup(nullptr)
    , enablePopupNotifications(true)
    , flashTimer(nullptr)
    , flashCount(0)
    , maxFlashCount(0)
{
    notificationPopup = new NotificationPopup();
    connect(notificationPopup, &NotificationPopup::notificationClicked,
            this, &TrayManager::notificationActivated);

    flashTimer = new QTimer(this);
    flashTimer->setSingleShot(true);
    connect(flashTimer, &QTimer::timeout, [this]() {
        if (flashCount < maxFlashCount) {
            // 切换图标
            if (trayIcon->icon().cacheKey() == originalIcon.cacheKey()) {
                trayIcon->setIcon(flashingIcon);
            } else {
                trayIcon->setIcon(originalIcon);
            }
            flashCount++;
            flashTimer->start(500); // 闪烁间隔500ms
        } else {
            // 恢复原始图标
            trayIcon->setIcon(originalIcon);
        }
    });
}

TrayManager::~TrayManager()
{
    if (trayIcon) {
        trayIcon->hide();
        trayIcon->deleteLater();
    }
    if (notificationPopup) {
        notificationPopup->deleteLater();
    }
}

void TrayManager::initializeTray()
{
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        // 创建系统托盘图标
        QIcon icon = QApplication::style()->standardIcon(QStyle::SP_ComputerIcon);
        originalIcon = icon;

        // 创建闪烁图标（稍微修改一下）
        QPixmap pixmap = icon.pixmap(32, 32);
        QPainter painter(&pixmap);
        painter.setPen(QPen(QColor(255, 0, 0, 128), 2));
        painter.drawRect(0, 0, pixmap.width()-1, pixmap.height()-1);
        flashingIcon = QIcon(pixmap);

        trayIcon = new QSystemTrayIcon(icon, this);

        createActions();
        createContextMenu();

        trayIcon->setContextMenu(trayMenu);
        trayIcon->setToolTip("校园通知推送客户端");

        connect(trayIcon, &QSystemTrayIcon::activated,
                this, &TrayManager::onTrayIconActivated);

        trayIcon->show();
    }
}

void TrayManager::createActions()
{
    restoreAction = new QAction("显示主窗口", this);
    connect(restoreAction, &QAction::triggered, [this]() {
        emit notificationActivated(-2); // 特殊值表示恢复主窗口
    });

    quitAction = new QAction("退出", this);
    connect(quitAction, &QAction::triggered, [this]() {
        QApplication::quit();
    });
}

void TrayManager::createContextMenu()
{
    trayMenu = new QMenu();
    trayMenu->addAction(restoreAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);
}

void TrayManager::showNotification(const QString &title, const QString &message, int noticeId)
{
    if (enablePopupNotifications) {
        notificationPopup->noticeId = noticeId;
        notificationPopup->showNotification(title, message);
    }

    // 显示系统托盘气泡提示
    if (trayIcon && trayIcon->supportsMessages()) {
        trayIcon->showMessage("新通知", title, QSystemTrayIcon::Information, 3000);
    }

    // 闪烁托盘图标
    flashTrayIcon();
}

void TrayManager::setEnablePopupNotifications(bool enable)
{
    QMutexLocker locker(&mutex);
    enablePopupNotifications = enable;
}

bool TrayManager::isPopupNotificationsEnabled() const
{
    QMutexLocker locker(&mutex);
    return enablePopupNotifications;
}

void TrayManager::flashTrayIcon(int times)
{
    if (!trayIcon || !originalIcon.isNull()) {
        return;
    }

    flashCount = 0;
    maxFlashCount = times * 2; // 每次闪烁包含亮暗两个状态

    flashTimer->start(500);
}

void TrayManager::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        emit notificationActivated(-2); // 恢复主窗口
        break;
    default:
        break;
    }
}
