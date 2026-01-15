#ifndef NOTIFICATIONEDITDIALOG_H
#define NOTIFICATIONEDITDIALOG_H

#include <QDialog>
#include "notificationitem.h"
#include "databasehelper.h"

namespace Ui {
class NotificationEditDialog;
}

class NotificationEditDialog : public QDialog {
    Q_OBJECT

public:
    explicit NotificationEditDialog(QWidget *parent = nullptr);
    ~NotificationEditDialog();

    void setNotification(const Notification& notification);
    Notification getNotification() const;

    void setIsAdding(bool isAdding);
    void setDatabaseHelper(DatabaseHelper* helper);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::NotificationEditDialog *ui;
    bool m_isAdding;
    DatabaseHelper* m_dbHelper; // 数据库助手指针，用于获取最大ID
};

#endif // NOTIFICATIONEDITDIALOG_H
