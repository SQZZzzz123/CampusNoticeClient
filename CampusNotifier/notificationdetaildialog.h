#ifndef NOTIFICATIONDETAILDIALOG_H
#define NOTIFICATIONDETAILDIALOG_H

#include <QDialog>
#include "notificationitem.h"

namespace Ui {
class NotificationDetailDialog;
}

class NotificationDetailDialog : public QDialog {
    Q_OBJECT

public:
    explicit NotificationDetailDialog(QWidget *parent = nullptr);
    ~NotificationDetailDialog();
    
    void setNotification(const Notification& notification);

private:
    Ui::NotificationDetailDialog *ui;
};

#endif // NOTIFICATIONDETAILDIALOG_H