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

    // 根据应用程序类型设置窗口标题
    if (AppConfig::isPusher()) {
        this->setWindowTitle("校园通知 - 推送端");
    } else if (AppConfig::isReceiver()) {
        this->setWindowTitle("校园通知 - 接收端");
    }

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

    // 根据应用程序类型设置UI
    if (AppConfig::isReceiver()) {
        // 接收端：隐藏添加、修改、删除按钮
        ui->addBtn->hide();
        ui->editBtn->hide();
        ui->deleteBtn->hide();
    }

    // 信号槽
    connect(ui->refreshBtn, &QPushButton::clicked, this, &MainWindow::onRefreshClicked);
    connect(netManager, &NetworkManager::dataReady, this, &MainWindow::updateUI);
    connect(ui->categoryCombo, &QComboBox::currentTextChanged, this, &MainWindow::filterByCategory);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::onNotificationItemClicked);
    
    // 只有推送端才连接增删改信号槽
    if (AppConfig::isPusher()) {
        connect(ui->addBtn, &QPushButton::clicked, this, &MainWindow::onAddBtnClicked);
        connect(ui->editBtn, &QPushButton::clicked, this, &MainWindow::onEditBtnClicked);
        connect(ui->deleteBtn, &QPushButton::clicked, this, &MainWindow::onDeleteBtnClicked);
    }
    
    // 网络操作回调信号槽
    connect(netManager, &NetworkManager::notificationAdded, this, &MainWindow::onNotificationAdded);
    connect(netManager, &NetworkManager::notificationUpdated, this, &MainWindow::onNotificationUpdated);
    connect(netManager, &NetworkManager::notificationDeleted, this, &MainWindow::onNotificationDeleted);
    connect(netManager, &NetworkManager::syncCompleted, this, &MainWindow::onSyncCompleted);

    // 定时器：每1分钟轮询一次，确保数据同步
    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &MainWindow::onRefreshClicked);
    refreshTimer->start(60000);


}

void MainWindow::onRefreshClicked() {
    QMetaObject::invokeMethod(netManager, "fetchNotifications", Qt::QueuedConnection);
}

void MainWindow::updateUI(QVector<Notification> data) {
    dbHelper->saveNotifications(data); // 存入缓存
    currentData = dbHelper->getAllNotifications(); // 重新加载所有数据，确保包含所有通知
    filterByCategory(ui->categoryCombo->currentText());
}

void MainWindow::filterByCategory(const QString& category) {
    ui->listWidget->clear();
    itemToNotificationMap.clear();

    for (const auto& notification : currentData) {
        if (category == "全部" || notification.category == category) {
            QString itemText = QString("[%1] ID:%2 - %3 (%4)").arg(notification.category).arg(notification.id).arg(notification.title).arg(notification.date);
            QListWidgetItem* listItem = new QListWidgetItem(itemText);
            ui->listWidget->addItem(listItem);
            itemToNotificationMap[listItem] = notification;
        }
    }
}

void MainWindow::onAddBtnClicked() {
    NotificationEditDialog dialog(this);
    dialog.setDatabaseHelper(dbHelper); // 设置数据库助手
    dialog.setIsAdding(true);

    if (dialog.exec() == QDialog::Accepted) {
        Notification notification = dialog.getNotification();

        if (dbHelper->addNotification(notification)) {
            QMessageBox::information(this, "成功", "通知添加成功");
            // 更新UI
            updateUI(dbHelper->getAllNotifications());
            // 同步到服务器
            QMetaObject::invokeMethod(netManager, "addNotificationToServer", 
                                     Qt::QueuedConnection, Q_ARG(Notification, notification));
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
            // 同步到服务器
            QMetaObject::invokeMethod(netManager, "updateNotificationOnServer", 
                                     Qt::QueuedConnection, Q_ARG(Notification, updatedNotification));
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
            // 同步到服务器
            QMetaObject::invokeMethod(netManager, "deleteNotificationFromServer", 
                                     Qt::QueuedConnection, Q_ARG(int, notification.id));
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

void MainWindow::onNotificationAdded(bool success) {
    if (!success) {
        QMessageBox::warning(this, "同步失败", "通知添加到服务器失败");
    }
}

void MainWindow::onNotificationUpdated(bool success) {
    if (!success) {
        QMessageBox::warning(this, "同步失败", "通知更新到服务器失败");
    }
}

void MainWindow::onNotificationDeleted(bool success) {
    if (!success) {
        QMessageBox::warning(this, "同步失败", "通知删除到服务器失败");
    }
}

void MainWindow::onSyncCompleted() {
    // 同步完成，重新加载本地数据
    // 实际应用中，这里应该从服务器获取最新数据并更新本地数据库
    syncWithServer();
}

void MainWindow::syncWithServer() {
    // 模拟从服务器同步数据到本地
    qDebug() << "正在与服务器同步数据...";
    // 实际应用中，这里应该从服务器获取最新数据并更新本地数据库
    // 由于没有真实服务器，我们只需要重新加载本地数据即可
    updateUI(dbHelper->getAllNotifications());
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
