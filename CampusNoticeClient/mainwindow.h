#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QStatusBar>
#include <QLabel>
#include <QSplitter>
#include <QDialog>
#include <QTextEdit>
#include <QHeaderView>
#include <QSpinBox>
#include <QCheckBox>
#include <QProgressBar>

#include "NoticeModel.h"
#include "NoticeManager.h"
// 新增：图表管理器和托盘管理器头文件
#include "ChartManager.h"
#include "TrayManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRefreshClicked();
    void onMarkAllAsReadClicked();
    void onClearReadClicked();
    void onNoticeDoubleClicked(const QModelIndex &index);
    void onFilterChanged();
    void onSearchTextChanged();
    void onNetworkStatusChanged(const QString &status);
    void onCacheCountChanged(int count);
    // 新增：新通知接收槽函数
    void onNewNoticesReceived(const QList<Notice> &newNotices);
    // 新增：导出功能槽函数
    void onExportSelectedClicked();
    void onExportAllClicked();
    // 新增：统计图表功能槽函数
    void onShowChartClicked();
    // 新增：轮询间隔设置槽函数
    void onPollingIntervalChanged();
    // 新增：离线模式切换槽函数
    void onOfflineModeToggled();
    // 新增：备份和恢复功能槽函数
    void onBackupDataClicked();
    void onRestoreDataClicked();
    // 新增：托盘和通知提醒槽函数
    void onNotificationActivated(int noticeId);

private:
    void setupUI();
    void setupConnections();
    void loadNotices();

    // UI Components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;

    // Top control panel
    QWidget *controlPanel;
    QHBoxLayout *controlLayout;
    QComboBox *categoryFilter;
    QLineEdit *searchBox;
    QPushButton *refreshBtn;
    QPushButton *markAllReadBtn;
    QPushButton *clearReadBtn;
    // 新增：导出按钮
    QPushButton *exportSelectedBtn;
    QPushButton *exportAllBtn;
    // 新增：统计图表按钮
    QPushButton *chartBtn;
    // 新增：轮询间隔设置
    QLabel *pollingIntervalLabel;
    QSpinBox *pollingIntervalSpinBox;
    // 新增：离线模式开关
    QCheckBox *offlineModeCheckBox;
    // 新增：备份恢复按钮
    QPushButton *backupBtn;
    QPushButton *restoreBtn;

    // Main content area
    QTableView *tableView;
    NoticeModel *sourceModel;
    QSortFilterProxyModel *proxyModel;

    // Status bar
    QLabel *networkStatusLabel;
    QLabel *cacheCountLabel;
    // 新增：导出进度条
    QProgressBar *exportProgressBar;

    // Manager
    NoticeManager *noticeManager;
    // 新增：图表管理器和托盘管理器
    ChartManager *chartManager;
    TrayManager *trayManager;
};

#endif // MAINWINDOW_H
