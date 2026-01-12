#include "notificationeditdialog.h"
#include "ui_notificationeditdialog.h"
#include <QMessageBox>

NotificationEditDialog::NotificationEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NotificationEditDialog),
    m_isAdding(false)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &NotificationEditDialog::on_buttonBox_accepted);
}

NotificationEditDialog::~NotificationEditDialog()
{
    delete ui;
}

void NotificationEditDialog::setNotification(const Notification& notification)
{
    ui->idLineEdit->setText(QString::number(notification.id));
    ui->titleLineEdit->setText(notification.title);
    
    // 设置分类
    int categoryIndex = ui->categoryComboBox->findText(notification.category);
    if (categoryIndex >= 0) {
        ui->categoryComboBox->setCurrentIndex(categoryIndex);
    }
    
    ui->dateLineEdit->setText(notification.date);
    ui->contentTextEdit->setText(notification.content);
}

Notification NotificationEditDialog::getNotification() const
{
    Notification notification;
    notification.id = ui->idLineEdit->text().toInt();
    notification.title = ui->titleLineEdit->text();
    notification.category = ui->categoryComboBox->currentText();
    notification.date = ui->dateLineEdit->text();
    notification.content = ui->contentTextEdit->toPlainText();
    
    return notification;
}

void NotificationEditDialog::setIsAdding(bool isAdding)
{
    m_isAdding = isAdding;
    
    if (m_isAdding) {
        setWindowTitle("添加通知");
        ui->idLineEdit->setEnabled(true); // 添加时允许修改ID
    } else {
        setWindowTitle("编辑通知");
        ui->idLineEdit->setEnabled(false); // 编辑时不允许修改ID
    }
}

void NotificationEditDialog::on_buttonBox_accepted()
{
    // 验证输入
    if (ui->idLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入ID");
        return;
    }
    
    if (ui->titleLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入标题");
        return;
    }
    
    if (ui->dateLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入日期");
        return;
    }
    
    if (ui->contentTextEdit->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入内容");
        return;
    }
    
    accept();
}