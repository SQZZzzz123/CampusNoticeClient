#include "MainWindow.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDateTime>
#include <QApplication>
#include <QThread>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setupConnections();

    // 初始化通知管理器
    noticeManager = new NoticeManager(this);

    // 初始化图表管理器和托盘管理器
    chartManager = new ChartManager(this);
    trayManager = new TrayManager(this);
    trayManager->initializeTray();

    // 连接信号槽
    connect(noticeManager, &NoticeManager::networkStatusChanged,
            this, &MainWindow::onNetworkStatusChanged);
    connect(noticeManager, &NoticeManager::cacheCountChanged,
            this, &MainWindow::onCacheCountChanged);
    // 连接新通知信号
    connect(noticeManager, &NoticeManager::newNoticesReceived,
            this, &MainWindow::onNewNoticesReceived);
    // 连接托盘管理器信号
    connect(trayManager, &TrayManager::notificationActivated,
            this, &MainWindow::onNotificationActivated);

    // 初始化模型
    sourceModel = new NoticeModel(this);
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(sourceModel);
    proxyModel->setFilterKeyColumn(-1); // Disable automatic filtering

    tableView->setModel(proxyModel);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setAlternatingRowColors(true);

    // 设置列宽
    tableView->setColumnWidth(0, 300); // 标题
    tableView->setColumnWidth(1, 80);  // 类别
    tableView->setColumnWidth(2, 150); // 发布时间
    tableView->setColumnWidth(3, 60);  // 已读状态

    // 加载初始数据
    loadNotices();

    // 启动定时轮询
    noticeManager->startPolling();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setWindowTitle("校园通知推送客户端");
    resize(900, 600);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    // Control panel
    controlPanel = new QWidget(centralWidget);
    controlLayout = new QHBoxLayout(controlPanel);

    categoryFilter = new QComboBox(controlPanel);
    categoryFilter->addItem("全部", "");
    categoryFilter->addItem("通知", "通知");
    categoryFilter->addItem("公告", "公告");
    categoryFilter->addItem("活动", "活动");
    categoryFilter->addItem("考试", "考试");

    searchBox = new QLineEdit(controlPanel);
    searchBox->setPlaceholderText("搜索标题...");

    refreshBtn = new QPushButton("手动刷新", controlPanel);
    markAllReadBtn = new QPushButton("标记全部已读", controlPanel);
    clearReadBtn = new QPushButton("清空已读通知", controlPanel);

    // 导出按钮
    exportSelectedBtn = new QPushButton("导出选中通知", controlPanel);
    exportAllBtn = new QPushButton("导出全部通知", controlPanel);

    // 统计图表按钮
    chartBtn = new QPushButton("统计图表", controlPanel);

    // 轮询间隔设置
    pollingIntervalLabel = new QLabel("轮询间隔:", controlPanel);
    pollingIntervalSpinBox = new QSpinBox(controlPanel);
    pollingIntervalSpinBox->setRange(1, 30);
    pollingIntervalSpinBox->setValue(5); // 默认5分钟
    pollingIntervalSpinBox->setSuffix(" 分钟");

    // 离线模式开关
    offlineModeCheckBox = new QCheckBox("离线模式", controlPanel);

    // 备份恢复按钮
    backupBtn = new QPushButton("备份数据", controlPanel);
    restoreBtn = new QPushButton("恢复数据", controlPanel);

    controlLayout->addWidget(new QLabel("类别:", controlPanel));
    controlLayout->addWidget(categoryFilter);
    controlLayout->addWidget(new QLabel("搜索:", controlPanel));
    controlLayout->addWidget(searchBox);
    controlLayout->addStretch();
    controlLayout->addWidget(exportSelectedBtn);
    controlLayout->addWidget(exportAllBtn);
    controlLayout->addWidget(chartBtn);
    controlLayout->addWidget(pollingIntervalLabel);
    controlLayout->addWidget(pollingIntervalSpinBox);
    controlLayout->addWidget(offlineModeCheckBox);
    controlLayout->addWidget(backupBtn);
    controlLayout->addWidget(restoreBtn);
    controlLayout->addWidget(refreshBtn);
    controlLayout->addWidget(markAllReadBtn);
    controlLayout->addWidget(clearReadBtn);

    mainLayout->addWidget(controlPanel);

    // Table view
    tableView = new QTableView(centralWidget);
    mainLayout->addWidget(tableView);

    // Status bar
    networkStatusLabel = new QLabel("就绪", this);
    cacheCountLabel = new QLabel("缓存: 0 条", this);

    // 导出进度条
    exportProgressBar = new QProgressBar(this);
    exportProgressBar->setVisible(false);
    exportProgressBar->setMaximum(100);
    exportProgressBar->setTextVisible(true);

    statusBar()->addWidget(networkStatusLabel);
    statusBar()->addWidget(cacheCountLabel);
    statusBar()->addPermanentWidget(exportProgressBar);
}

void MainWindow::setupConnections()
{
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::onRefreshClicked);
    connect(markAllReadBtn, &QPushButton::clicked, this, &MainWindow::onMarkAllAsReadClicked);
    connect(clearReadBtn, &QPushButton::clicked, this, &MainWindow::onClearReadClicked);
    connect(tableView, &QTableView::doubleClicked, this, &MainWindow::onNoticeDoubleClicked);
    connect(categoryFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onFilterChanged);
    connect(searchBox, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    // 导出功能连接
    connect(exportSelectedBtn, &QPushButton::clicked, this, &MainWindow::onExportSelectedClicked);
    connect(exportAllBtn, &QPushButton::clicked, this, &MainWindow::onExportAllClicked);
    // 统计图表功能连接
    connect(chartBtn, &QPushButton::clicked, this, &MainWindow::onShowChartClicked);
    // 轮询间隔设置连接
    connect(pollingIntervalSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::onPollingIntervalChanged);
    // 离线模式切换连接
    connect(offlineModeCheckBox, &QCheckBox::toggled, this, &MainWindow::onOfflineModeToggled);
    // 备份恢复功能连接
    connect(backupBtn, &QPushButton::clicked, this, &MainWindow::onBackupDataClicked);
    connect(restoreBtn, &QPushButton::clicked, this, &MainWindow::onRestoreDataClicked);
}

void MainWindow::loadNotices()
{
    // 从数据库加载通知
    sourceModel->loadData();
}

void MainWindow::onRefreshClicked()
{
    if (offlineModeCheckBox->isChecked()) {
        // 离线模式下只加载本地数据
        sourceModel->loadData();
        onCacheCountChanged(sourceModel->rowCount()); // 更新缓存计数
        onNetworkStatusChanged("离线模式 - 已刷新本地数据");
    } else {
        noticeManager->fetchNotices();
    }
}

void MainWindow::onMarkAllAsReadClicked()
{
    noticeManager->markAllAsRead();
    sourceModel->loadData(); // 重新加载数据以反映变化
}

void MainWindow::onClearReadClicked()
{
    noticeManager->clearReadNotices();
    sourceModel->loadData(); // 重新加载数据以反映变化
}

void MainWindow::onNoticeDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;

    // 获取原始模型索引
    QModelIndex sourceIndex = proxyModel->mapToSource(index);
    int row = sourceIndex.row();

    // 获取通知详情
    QSqlRecord record = sourceModel->record(row);
    QString title = record.value("title").toString();
    QString content = record.value("content").toString();
    QString category = record.value("category").toString();
    QString publishTime = record.value("publish_time").toString();

    // 创建详情对话框
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(title);
    dialog->resize(500, 400);

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    QTextEdit *textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    QString fullContent = QString("类别：%1\n发布时间：%2\n\n%3")
                              .arg(category).arg(publishTime).arg(content);
    textEdit->setPlainText(fullContent);

    layout->addWidget(textEdit);

    // 标记为已读
    int id = record.value("id").toInt();
    noticeManager->markAsRead(id);

    // 更新模型显示
    sourceModel->loadData();

    dialog->exec();
    dialog->deleteLater();
}

void MainWindow::onFilterChanged()
{
    QString filterCategory = categoryFilter->currentData().toString();
    if (filterCategory.isEmpty()) {
        proxyModel->setFilterFixedString("");
    } else {
        // 设置过滤器，只对类别列进行过滤
        proxyModel->setFilterKeyColumn(1); // 类别列
        proxyModel->setFilterFixedString(filterCategory);
    }
}

void MainWindow::onSearchTextChanged()
{
    QString searchText = searchBox->text();
    if (searchText.isEmpty()) {
        // 如果搜索框为空，恢复类别过滤
        onFilterChanged();
    } else {
        // 临时取消类别过滤，只进行搜索
        proxyModel->setFilterKeyColumn(-1); // 对所有列进行搜索
        proxyModel->setFilterFixedString(searchText);
    }
}

void MainWindow::onNetworkStatusChanged(const QString &status)
{
    networkStatusLabel->setText(status);

    // 如果是新通知相关的状态，可以触发托盘提醒
    if (status.contains("Successfully fetched") && status.contains("notices")) {
        // 这里可以添加新通知的处理逻辑
    }
}

void MainWindow::onCacheCountChanged(int count)
{
    cacheCountLabel->setText(QString("缓存: %1 条").arg(count));
}

// 新通知接收实现
void MainWindow::onNewNoticesReceived(const QList<Notice> &newNotices)
{
    if (!newNotices.isEmpty()) {
        // 更新状态栏
        onNetworkStatusChanged(QString("收到 %1 条新通知").arg(newNotices.size()));

        // 为每条新通知显示托盘提醒
        for (const Notice &notice : newNotices) {
            trayManager->showNotification(
                QString("新通知: %1").arg(notice.title),
                QString("类别: %1\n%2").arg(notice.category).arg(notice.content.left(100) + (notice.content.length() > 100 ? "..." : "")),
                notice.id
                );
        }

        // 重新加载通知列表以显示新通知
        sourceModel->loadData();
        onCacheCountChanged(sourceModel->rowCount());
    }
}

// 新增：导出功能实现
void MainWindow::onExportSelectedClicked()
{
    // 获取选中的行
    QModelIndexList selectedIndexes = tableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::information(this, "提示", "请先选择要导出的通知！");
        return;
    }

    // 显示进度条
    exportProgressBar->setVisible(true);
    exportProgressBar->setFormat("正在导出选中通知...");
    exportProgressBar->setValue(0);

    // 获取选中的通知数据
    QList<Notice> selectedNotices;
    for (const QModelIndex &proxyIndex : selectedIndexes) {
        QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex);
        int row = sourceIndex.row();
        if (row >= 0 && row < sourceModel->getNotices().size()) {
            selectedNotices.append(sourceModel->getNotices()[row]);
        }
    }

    // 选择导出路径
    QString fileName = QString("校园通知_选中_%1.csv")
                           .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "导出选中通知",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + fileName,
        "CSV Files (*.csv)"
        );

    if (filePath.isEmpty()) {
        exportProgressBar->setVisible(false);
        return;
    }

    // 在后台线程执行导出
    QFuture<void> future = QtConcurrent::run([this, selectedNotices, filePath]() {
        bool success = noticeManager->exportNoticesToCSV(selectedNotices, filePath);

        // 在主线程更新UI
        QMetaObject::invokeMethod(this, [this, success, filePath]() {
            exportProgressBar->setVisible(false);
            if (success) {
                QMessageBox::information(this, "导出成功",
                                         QString("选中通知已成功导出到:\n%1").arg(filePath));
            } else {
                QMessageBox::warning(this, "导出失败",
                                     QString("导出失败，请检查文件路径:\n%1").arg(filePath));
            }
        });
    });
}

void MainWindow::onExportAllClicked()
{
    // 显示进度条
    exportProgressBar->setVisible(true);
    exportProgressBar->setFormat("正在导出全部通知...");
    exportProgressBar->setValue(0);

    // 选择导出路径
    QString fileName = QString("校园通知_全部_%1.csv")
                           .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "导出全部通知",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + fileName,
        "CSV Files (*.csv)"
        );

    if (filePath.isEmpty()) {
        exportProgressBar->setVisible(false);
        return;
    }

    // 在后台线程执行导出
    QFuture<void> future = QtConcurrent::run([this, filePath]() {
        bool success = noticeManager->exportAllNoticesToCSV(filePath);

        // 在主线程更新UI
        QMetaObject::invokeMethod(this, [this, success, filePath]() {
            exportProgressBar->setVisible(false);
            if (success) {
                QMessageBox::information(this, "导出成功",
                                         QString("全部通知已成功导出到:\n%1").arg(filePath));
            } else {
                QMessageBox::warning(this, "导出失败",
                                     QString("导出失败，请检查文件路径:\n%1").arg(filePath));
            }
        });
    });
}

// 统计图表功能实现
void MainWindow::onShowChartClicked()
{
    chartManager->showChartDialog();
}

// 轮询间隔设置实现
void MainWindow::onPollingIntervalChanged()
{
    int intervalMinutes = pollingIntervalSpinBox->value();
    int intervalMs = intervalMinutes * 60 * 1000; // 转换为毫秒

    noticeManager->setPollingInterval(intervalMs);

    // 更新状态栏提示
    onNetworkStatusChanged(QString("轮询间隔已设置为 %1 分钟").arg(intervalMinutes));
}

// ：离线模式切换实现
void MainWindow::onOfflineModeToggled()
{
    bool isOffline = offlineModeCheckBox->isChecked();

    if (isOffline) {
        noticeManager->stopPolling(); // 停止网络轮询
        onNetworkStatusChanged("离线模式 - 仅加载本地缓存");
    } else {
        noticeManager->startPolling(); // 重新启动轮询
        noticeManager->fetchNotices(); // 立即获取一次数据
        onNetworkStatusChanged("在线模式 - 已启动网络轮询");
    }
}

// 备份数据功能实现
void MainWindow::onBackupDataClicked()
{
    QString backupDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/CampusNoticeBackups";
    QDir dir;
    if (!dir.exists(backupDir)) {
        dir.mkpath(backupDir);
    }

    QString fileName = QString("campus_notice_backup_%1.db")
                           .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QString backupPath = backupDir + "/" + fileName;

    // 在后台线程执行备份
    QFuture<bool> future = QtConcurrent::run([this, backupPath]() {
        return noticeManager->backupDatabase(backupPath);
    });

    // 等待备份完成并显示结果
    bool success = future.result();

    if (success) {
        QMessageBox::information(this, "备份成功",
                                 QString("数据库已成功备份到:\n%1").arg(backupPath));
    } else {
        QMessageBox::warning(this, "备份失败",
                             QString("数据库备份失败，请检查路径:\n%1").arg(backupPath));
    }
}

// 恢复数据功能实现
void MainWindow::onRestoreDataClicked()
{
    QString backupPath = QFileDialog::getOpenFileName(
        this,
        "选择备份文件",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/CampusNoticeBackups",
        "Database Files (*.db)"
        );

    if (backupPath.isEmpty()) {
        return;
    }

    // 确认恢复操作
    int ret = QMessageBox::question(this, "确认恢复",
                                    QString("确定要从以下文件恢复数据吗？\n此操作将覆盖当前数据库！\n\n%1").arg(backupPath));

    if (ret != QMessageBox::Yes) {
        return;
    }

    // 在后台线程执行恢复
    QFuture<bool> future = QtConcurrent::run([this, backupPath]() {
        return noticeManager->restoreDatabase(backupPath);
    });

    // 等待恢复完成并显示结果
    bool success = future.result();

    if (success) {
        // 重新加载数据
        sourceModel->loadData();
        onCacheCountChanged(sourceModel->rowCount());

        QMessageBox::information(this, "恢复成功",
                                 QString("数据库已成功从以下文件恢复:\n%1").arg(backupPath));
    } else {
        QMessageBox::warning(this, "恢复失败",
                             QString("数据库恢复失败，请检查文件:\n%1").arg(backupPath));
    }
}

// 托盘和通知提醒功能实现
void MainWindow::onNotificationActivated(int noticeId)
{
    if (noticeId == -2) { // 特殊值表示恢复主窗口
        showNormal();
        raise();
        activateWindow();
    } else if (noticeId > 0) {
        // 查找并显示通知详情
        for (int i = 0; i < sourceModel->getNotices().size(); ++i) {
            if (sourceModel->getNotices()[i].id == noticeId) {
                // 选中该行
                QModelIndex index = sourceModel->index(i, 0);
                QModelIndex proxyIndex = proxyModel->mapFromSource(index);

                tableView->scrollTo(proxyIndex);
                tableView->setCurrentIndex(proxyIndex);

                // 显示详情
                onNoticeDoubleClicked(proxyIndex);
                break;
            }
        }
    }
}
