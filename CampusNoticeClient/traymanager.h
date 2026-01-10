#ifndef TRAYMANAGER_H
#define TRAYMANAGER_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

// 自定义通知弹窗类
class NotificationPopup : public QWidget
{
    Q_OBJECT

public:
    explicit NotificationPopup(QWidget *parent = nullptr);
    void showNotification(const QString &title, const QString &message, int duration = 5000);

signals:
    void notificationClicked(int noticeId);

public slots:
    void closeNotification();

private slots:
    void onTimeout();
    void startFadeOut();

private:
    void initUI();

    QLabel *titleLabel;
    QLabel *messageLabel;
    QPushButton *closeButton;
    QTimer *timer;
    QPropertyAnimation *fadeAnimation;
    int noticeId;
};

class TrayManager : public QObject
{
    Q_OBJECT

public:
    explicit TrayManager(QObject *parent = nullptr);
    ~TrayManager();

    // 初始化系统托盘
    void initializeTray();

    // 显示通知提醒
    void showNotification(const QString &title, const QString &message, int noticeId = -1);

    // 设置是否启用弹窗提醒
    void setEnablePopupNotifications(bool enable);

    // 获取是否启用弹窗提醒
    bool isPopupNotificationsEnabled() const;

    // 闪烁托盘图标
    void flashTrayIcon(int times = 3);

signals:
    void notificationActivated(int noticeId);

public slots:
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    void createActions();
    void createContextMenu();

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;

    QAction *restoreAction;
    QAction *quitAction;

    NotificationPopup *notificationPopup;
    bool enablePopupNotifications;
    QTimer *flashTimer;
    int flashCount;
    int maxFlashCount;
    QIcon originalIcon;
    QIcon flashingIcon;
    QMutex mutex;
};

#endif // TRAYMANAGER_H
