#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_noticeModel(nullptr)
    , m_noticeUpdater(nullptr)
    , m_currentCategory(NoticeCategory::All)
    , m_currentNoticeId(-1)
{
    ui->setupUi(this);
    
    // 初始化各组件
    initModel();
    initNoticeUpdater();
    initUI();
    connectSignalsSlots();
    
    // 更新UI显示
    updateUI();
    
    // 立即更新一次通知
    m_noticeUpdater->updateNow();
}

MainWindow::~MainWindow()
{
    delete m_noticeModel;
    delete m_noticeUpdater;
    delete ui;
}

void MainWindow::initUI()
{
    // 设置窗口标题
    setWindowTitle("校园通知客户端");
    
    // 设置通知列表视图
    ui->noticeListView->setModel(m_noticeModel);
    
    // 设置通知列表视图的选择模式
    ui->noticeListView->setSelectionMode(QAbstractItemView::SingleSelection);
    
    // 设置通知列表视图的交替行颜色
    ui->noticeListView->setAlternatingRowColors(true);
    
    // 设置初始状态
    hideNoticeDetail();
}

void MainWindow::initModel()
{
    // 创建通知模型
    m_noticeModel = new NoticeModel(this);
}

void MainWindow::initNoticeUpdater()
{
    // 创建通知更新器
    m_noticeUpdater = new NoticeUpdater(this);
    
    // 初始化通知更新器（这里使用一个示例服务器地址，实际使用时需要修改）
    m_noticeUpdater->init("http://example.com/api");
    
    // 启动自动更新
    m_noticeUpdater->start();
}

void MainWindow::connectSignalsSlots()
{
    // 连接分类列表选择变化信号
    connect(ui->categoryListWidget, &QListWidget::currentItemChanged, this, &MainWindow::onCategorySelectionChanged);
    
    // 连接通知列表选择变化信号
    connect(ui->noticeListView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::onNoticeSelectionChanged);
    
    // 连接刷新按钮点击信号
    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshButtonClicked);
    
    // 连接全部已读按钮点击信号
    connect(ui->markAllReadButton, &QPushButton::clicked, this, &MainWindow::onMarkAllReadButtonClicked);
    
    // 连接通知更新器信号
    connect(m_noticeUpdater, &NoticeUpdater::updateStarted, this, &MainWindow::onUpdateStarted);
    connect(m_noticeUpdater, &NoticeUpdater::updateFinished, this, &MainWindow::onUpdateFinished);
    connect(m_noticeUpdater, &NoticeUpdater::updateFailed, this, &MainWindow::onUpdateFailed);
    
    // 连接通知模型信号
    connect(m_noticeModel, &NoticeModel::noticeCountChanged, this, &MainWindow::onNoticeCountChanged);
    connect(m_noticeModel, &NoticeModel::unreadCountChanged, this, &MainWindow::onUnreadCountChanged);
    
    // 连接菜单操作信号
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::on_actionExit_triggered);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::on_actionSettings_triggered);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::on_actionAbout_triggered);
}

void MainWindow::updateUI()
{
    updateNoticeList();
    updateUnreadCountDisplay();
    updateLastUpdateTime();
}

void MainWindow::updateNoticeList()
{
    // 从数据库获取通知列表
    QList<Notice> notices = m_noticeUpdater->getDatabaseManager()->getNotices(m_currentCategory);
    
    // 更新通知模型数据
    m_noticeModel->refreshData(notices);
}

void MainWindow::updateUnreadCountDisplay()
{
    // 获取当前分类的未读通知数量
    int unreadCount = m_noticeUpdater->getUnreadCount(m_currentCategory);
    
    // 更新未读数量显示
    ui->unreadCountLabel->setText(QString("(%1 条未读)").arg(unreadCount));
}

void MainWindow::updateLastUpdateTime()
{
    // 获取最后更新时间
    QDateTime lastUpdateTime = m_noticeUpdater->getLastUpdateTime();
    
    // 更新最后更新时间显示
    if (lastUpdateTime.isValid()) {
        ui->lastUpdateTimeLabel->setText(QString("最后更新：%1").arg(lastUpdateTime.toString("yyyy-MM-dd HH:mm:ss")));
    } else {
        ui->lastUpdateTimeLabel->setText("最后更新：从未");
    }
}

NoticeCategory MainWindow::getCurrentSelectedCategory()
{
    // 获取当前选中的行索引
    int currentRow = ui->categoryListWidget->currentRow();
    
    // 根据行索引返回对应的分类
    switch (currentRow) {
    case 0: // 全部通知
        return NoticeCategory::All;
    case 1: // 公告
        return NoticeCategory::Announcement;
    case 2: // 活动
        return NoticeCategory::Event;
    case 3: // 学术
        return NoticeCategory::Academic;
    case 4: // 就业
        return NoticeCategory::Job;
    case 5: // 其他
        return NoticeCategory::Other;
    default:
        return NoticeCategory::All;
    }
}

void MainWindow::showNoticeDetail(const Notice &notice)
{
    // 更新通知标题
    ui->noticeTitleLabel->setText(notice.title);
    
    // 更新发布时间
    ui->noticePublishTimeLabel->setText(notice.publishTime.toString("yyyy-MM-dd HH:mm:ss"));
    
    // 更新分类
    QString categoryText;
    switch (notice.category) {
    case NoticeCategory::Announcement:
        categoryText = "公告";
        break;
    case NoticeCategory::Event:
        categoryText = "活动";
        break;
    case NoticeCategory::Academic:
        categoryText = "学术";
        break;
    case NoticeCategory::Job:
        categoryText = "就业";
        break;
    case NoticeCategory::Other:
        categoryText = "其他";
        break;
    default:
        categoryText = "全部通知";
        break;
    }
    ui->noticeCategoryLabel->setText(categoryText);
    
    // 更新通知内容
    ui->noticeContentTextBrowser->setHtml(notice.content);
    
    // 标记为已读
    m_noticeUpdater->markNoticeAsRead(notice.id);
    m_noticeModel->markNoticeAsReadById(notice.id);
    
    // 更新未读数量显示
    updateUnreadCountDisplay();
    
    // 更新当前通知ID
    m_currentNoticeId = notice.id;
}

void MainWindow::hideNoticeDetail()
{
    // 重置通知详情显示
    ui->noticeTitleLabel->setText("请选择通知查看详情");
    ui->noticePublishTimeLabel->clear();
    ui->noticeCategoryLabel->clear();
    ui->noticeContentTextBrowser->setHtml("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n p, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; color:#808080;\">请在左侧列表中选择通知查看详情</span></p></body></html>");
    
    // 重置当前通知ID
    m_currentNoticeId = -1;
}

void MainWindow::onCategorySelectionChanged()
{
    // 获取当前选中的分类
    m_currentCategory = getCurrentSelectedCategory();
    
    // 过滤通知列表
    m_noticeModel->filterByCategory(m_currentCategory);
    
    // 更新通知列表
    updateNoticeList();
    
    // 更新未读数量显示
    updateUnreadCountDisplay();
    
    // 隐藏通知详情
    hideNoticeDetail();
}

void MainWindow::onNoticeSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    
    if (!current.isValid()) {
        // 没有选中项，隐藏通知详情
        hideNoticeDetail();
        return;
    }
    
    // 获取选中的通知
    Notice notice = m_noticeModel->getNotice(current.row());
    if (notice.id != -1) {
        // 显示通知详情
        showNoticeDetail(notice);
    }
}

void MainWindow::onRefreshButtonClicked()
{
    // 立即更新通知
    m_noticeUpdater->updateNow();
}

void MainWindow::onMarkAllReadButtonClicked()
{
    // 标记当前分类的所有通知为已读
    bool result = m_noticeUpdater->markAllNoticesAsRead(m_currentCategory);
    if (result) {
        // 更新通知模型
        m_noticeModel->markAllNoticesAsRead();
        
        // 更新UI
        updateUnreadCountDisplay();
        
        // 显示提示
        QMessageBox::information(this, "提示", "已标记所有通知为已读");
    }
}

void MainWindow::onUpdateStarted()
{
    // 更新状态栏
    ui->statusLabel->setText("正在更新通知...");
}

void MainWindow::onUpdateFinished(int newNoticeCount)
{
    // 更新状态栏
    ui->statusLabel->setText("就绪");
    
    // 更新最后更新时间
    updateLastUpdateTime();
    
    // 更新通知列表
    updateNoticeList();
    
    // 更新未读数量显示
    updateUnreadCountDisplay();
    
    // 如果有新增通知，显示提示
    if (newNoticeCount > 0) {
        QMessageBox::information(this, "提示", QString("已获取到 %1 条新通知").arg(newNoticeCount));
    }
}

void MainWindow::onUpdateFailed(const QString &errorString)
{
    // 更新状态栏
    ui->statusLabel->setText("更新失败");
    
    // 显示错误提示
    QMessageBox::warning(this, "更新失败", errorString);
}

void MainWindow::onNoticeCountChanged()
{
    // 更新通知列表
    updateNoticeList();
}

void MainWindow::onUnreadCountChanged()
{
    // 更新未读数量显示
    updateUnreadCountDisplay();
}

void MainWindow::on_actionExit_triggered()
{
    // 退出应用
    close();
}

void MainWindow::on_actionSettings_triggered()
{
    // 显示设置对话框（示例，实际需要实现）
    QMessageBox::information(this, "设置", "设置功能待实现");
}

void MainWindow::on_actionAbout_triggered()
{
    // 显示关于对话框
    QMessageBox::about(this, "关于", "校园通知客户端\n版本 1.0.0\n使用 Qt 6.9.2 开发");
}
