#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "notice_model.h"
#include "notice_updater.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 分类列表选择变化
    void onCategorySelectionChanged();

    // 通知列表选择变化
    void onNoticeSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

    // 刷新按钮点击
    void onRefreshButtonClicked();

    // 全部已读按钮点击
    void onMarkAllReadButtonClicked();

    // 通知更新开始
    void onUpdateStarted();

    // 通知更新完成
    void onUpdateFinished(int newNoticeCount);

    // 通知更新失败
    void onUpdateFailed(const QString &errorString);

    // 通知数量变化
    void onNoticeCountChanged();

    // 未读通知数量变化
    void onUnreadCountChanged();

    // 菜单操作
    void on_actionExit_triggered();
    void on_actionSettings_triggered();
    void on_actionAbout_triggered();

private:
    // 初始化UI
    void initUI();

    // 初始化数据模型
    void initModel();

    // 初始化通知更新器
    void initNoticeUpdater();

    // 连接信号槽
    void connectSignalsSlots();

    // 更新UI显示
    void updateUI();

    // 更新未读通知数量显示
    void updateUnreadCountDisplay();

    // 更新通知列表
    void updateNoticeList();

    // 更新最后更新时间
    void updateLastUpdateTime();

    // 获取当前选中的分类
    NoticeCategory getCurrentSelectedCategory();

    // 显示通知详情
    void showNoticeDetail(const Notice &notice);

    // 隐藏通知详情
    void hideNoticeDetail();

    Ui::MainWindow *ui;
    NoticeModel *m_noticeModel;
    NoticeUpdater *m_noticeUpdater;
    NoticeCategory m_currentCategory;
    int m_currentNoticeId;
};
#endif // MAINWINDOW_H
