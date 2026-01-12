#include "notificationdetaildialog.h"
#include "ui_notificationdetaildialog.h"

NotificationDetailDialog::NotificationDetailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NotificationDetailDialog)
{
    ui->setupUi(this);
}

NotificationDetailDialog::~NotificationDetailDialog()
{
    delete ui;
}

void NotificationDetailDialog::setNotification(const Notification& notification)
{
    ui->titleLabel->setText(notification.title);
    ui->categoryValueLabel->setText(notification.category);
    ui->dateValueLabel->setText(notification.date);
    ui->contentLabel->setText(notification.content);
}