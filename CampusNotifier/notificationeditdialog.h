#ifndef NOTIFICATIONEDITDIALOG_H
#define NOTIFICATIONEDITDIALOG_H

#include <QDialog>
#include "notificationitem.h"

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

private slots:
    void on_buttonBox_accepted();

private:
    Ui::NotificationEditDialog *ui;
    bool m_isAdding;
};

#endif // NOTIFICATIONEDITDIALOG_H