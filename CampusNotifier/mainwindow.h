#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QListWidgetItem>
#include <QMap>
#include "appconfig.h"
#include "databasehelper.h"
#include "networkmanager.h"
#include "notificationitem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateUI(QVector<Notification> data);
    void onRefreshClicked();
    void filterByCategory(const QString& category);
    void onNotificationItemClicked(QListWidgetItem* item);
    void onAddBtnClicked();
    void onEditBtnClicked();
    void onDeleteBtnClicked();
    void onNotificationAdded(bool success);
    void onNotificationUpdated(bool success);
    void onNotificationDeleted(bool success);
    void onSyncCompleted();
    void syncWithServer();

private:
    Ui::MainWindow *ui;
    NetworkManager *netManager;
    DatabaseHelper *dbHelper;
    QTimer *refreshTimer;
    QVector<Notification> currentData;
    QMap<QListWidgetItem*, Notification> itemToNotificationMap;
};

#endif
