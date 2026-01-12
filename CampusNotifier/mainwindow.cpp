#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databasehelper.h"
#include "notificationdetaildialog.h"
#include "notificationeditdialog.h"
#include <QThread>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // 初始化数据库助手
    dbHelper = new DatabaseHelper();
    dbHelper->initDatabase();

    // 初始化网络模块与多线程
    netManager = new NetworkManager();
    QThread* thread = new QThread(this);
    netManager->moveToThread(thread);
    thread->start();

    // 加载缓存
    updateUI(dbHelper->getAllNotifications());

    // 信号槽
    connect(ui->refreshBtn, &QPushButton::clicked, this, &MainWindow::onRefreshClicked);
    connect(netManager, &NetworkManager::dataReady, this, &MainWindow::updateUI);
    connect(ui->categoryCombo, &QComboBox::currentTextChanged, this, &MainWindow::filterByCategory);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::onNotificationItemClicked);
    // 增加、修改、删除按钮信号槽
    connect(ui->addBtn, &QPushButton::clicked, this, &MainWindow::onAddBtnClicked);
    connect(ui->editBtn, &QPushButton::clicked, this, &MainWindow::onEditBtnClicked);
    connect(ui->deleteBtn, &QPushButton::clicked, this, &MainWindow::onDeleteBtnClicked);

    // 定时器：每10分钟轮询一次
    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &MainWindow::onRefreshClicked);
    refreshTimer->start(600000);

    
}

void MainWindow::onRefreshClicked() {
    QMetaObject::invokeMethod(netManager, "fetchNotifications", Qt::QueuedConnection);
}

void MainWindow::updateUI(QVector<Notification> data) {
    currentData = data;
    dbHelper->saveNotifications(data); // 存入缓存
    filterByCategory(ui->categoryCombo->currentText());
}

void MainWindow::filterByCategory(const QString& category) {
    ui->listWidget->clear();
    itemToNotificationMap.clear();
    
    for (const auto& notification : currentData) {
        if (category == "全部" || notification.category == category) {
            QString itemText = QString("[%1] %2 (%3)").arg(notification.category, notification.title, notification.date);
            QListWidgetItem* listItem = new QListWidgetItem(itemText);
            ui->listWidget->addItem(listItem);
            itemToNotificationMap[listItem] = notification;
        }
    }
}

void MainWindow::onAddBtnClicked() {
    NotificationEditDialog dialog(this);
    dialog.setIsAdding(true);
    
    if (dialog.exec() == QDialog::Accepted) {
        Notification notification = dialog.getNotification();
        
        if (dbHelper->addNotification(notification)) {
            QMessageBox::information(this, "成功", "通知添加成功");
            // 更新UI
            updateUI(dbHelper->getAllNotifications());
        } else {
            QMessageBox::warning(this, "失败", "通知添加失败");
        }
    }
}

void MainWindow::onEditBtnClicked() {
    QListWidgetItem* selectedItem = ui->listWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "警告", "请先选择要修改的通知");
        return;
    }
    
    if (!itemToNotificationMap.contains(selectedItem)) {
        QMessageBox::warning(this, "错误", "无法获取通知信息");
        return;
    }
    
    Notification notification = itemToNotificationMap[selectedItem];
    NotificationEditDialog dialog(this);
    dialog.setIsAdding(false);
    dialog.setNotification(notification);
    
    if (dialog.exec() == QDialog::Accepted) {
        Notification updatedNotification = dialog.getNotification();
        
        if (dbHelper->updateNotification(updatedNotification)) {
            QMessageBox::information(this, "成功", "通知修改成功");
            // 更新UI
            updateUI(dbHelper->getAllNotifications());
        } else {
            QMessageBox::warning(this, "失败", "通知修改失败");
        }
    }
}

void MainWindow::onDeleteBtnClicked() {
    QListWidgetItem* selectedItem = ui->listWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "警告", "请先选择要删除的通知");
        return;
    }
    
    if (!itemToNotificationMap.contains(selectedItem)) {
        QMessageBox::warning(this, "错误", "无法获取通知信息");
        return;
    }
    
    Notification notification = itemToNotificationMap[selectedItem];
    
    if (QMessageBox::question(this, "确认", QString("确定要删除通知 '%1' 吗？").arg(notification.title), 
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        
        if (dbHelper->deleteNotification(notification.id)) {
            QMessageBox::information(this, "成功", "通知删除成功");
            // 更新UI
            updateUI(dbHelper->getAllNotifications());
        } else {
            QMessageBox::warning(this, "失败", "通知删除失败");
        }
    }
}

void MainWindow::onNotificationItemClicked(QListWidgetItem* item) {
    if (itemToNotificationMap.contains(item)) {
        Notification notification = itemToNotificationMap[item];
        NotificationDetailDialog dialog(this);
        dialog.setNotification(notification);
        dialog.exec();
    }
}

MainWindow::~MainWindow() {
    // 停止定时器
    if (refreshTimer) {
        refreshTimer->stop();
        delete refreshTimer;
        refreshTimer = nullptr;
    }
    
    // 清理网络管理器和线程
    if (netManager) {
        // 确保线程正确退出
        netManager->thread()->quit();
        netManager->thread()->wait();
        
        delete netManager;
        netManager = nullptr;
    }
    
    // 清理数据库助手
    if (dbHelper) {
        delete dbHelper;
        dbHelper = nullptr;
    }
    
    delete ui;
}
