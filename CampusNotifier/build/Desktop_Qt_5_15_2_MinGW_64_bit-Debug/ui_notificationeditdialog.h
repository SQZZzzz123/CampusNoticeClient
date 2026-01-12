/********************************************************************************
** Form generated from reading UI file 'notificationeditdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NOTIFICATIONEDITDIALOG_H
#define UI_NOTIFICATIONEDITDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NotificationEditDialog
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *idLabel;
    QLineEdit *idLineEdit;
    QLabel *titleLabel;
    QLineEdit *titleLineEdit;
    QLabel *categoryLabel;
    QComboBox *categoryComboBox;
    QLabel *dateLabel;
    QLineEdit *dateLineEdit;
    QLabel *contentLabel;
    QTextEdit *contentTextEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *NotificationEditDialog)
    {
        if (NotificationEditDialog->objectName().isEmpty())
            NotificationEditDialog->setObjectName(QString::fromUtf8("NotificationEditDialog"));
        NotificationEditDialog->resize(500, 400);
        verticalLayoutWidget = new QWidget(NotificationEditDialog);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 480, 380));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        idLabel = new QLabel(verticalLayoutWidget);
        idLabel->setObjectName(QString::fromUtf8("idLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, idLabel);

        idLineEdit = new QLineEdit(verticalLayoutWidget);
        idLineEdit->setObjectName(QString::fromUtf8("idLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, idLineEdit);

        titleLabel = new QLabel(verticalLayoutWidget);
        titleLabel->setObjectName(QString::fromUtf8("titleLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, titleLabel);

        titleLineEdit = new QLineEdit(verticalLayoutWidget);
        titleLineEdit->setObjectName(QString::fromUtf8("titleLineEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, titleLineEdit);

        categoryLabel = new QLabel(verticalLayoutWidget);
        categoryLabel->setObjectName(QString::fromUtf8("categoryLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, categoryLabel);

        categoryComboBox = new QComboBox(verticalLayoutWidget);
        categoryComboBox->addItem(QString());
        categoryComboBox->addItem(QString());
        categoryComboBox->addItem(QString());
        categoryComboBox->setObjectName(QString::fromUtf8("categoryComboBox"));

        formLayout->setWidget(2, QFormLayout::FieldRole, categoryComboBox);

        dateLabel = new QLabel(verticalLayoutWidget);
        dateLabel->setObjectName(QString::fromUtf8("dateLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, dateLabel);

        dateLineEdit = new QLineEdit(verticalLayoutWidget);
        dateLineEdit->setObjectName(QString::fromUtf8("dateLineEdit"));

        formLayout->setWidget(3, QFormLayout::FieldRole, dateLineEdit);

        contentLabel = new QLabel(verticalLayoutWidget);
        contentLabel->setObjectName(QString::fromUtf8("contentLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, contentLabel);

        contentTextEdit = new QTextEdit(verticalLayoutWidget);
        contentTextEdit->setObjectName(QString::fromUtf8("contentTextEdit"));

        formLayout->setWidget(4, QFormLayout::FieldRole, contentTextEdit);


        verticalLayout->addLayout(formLayout);

        buttonBox = new QDialogButtonBox(verticalLayoutWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(NotificationEditDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), NotificationEditDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), NotificationEditDialog, SLOT(reject()));
        QObject::connect(buttonBox, SIGNAL(accepted()), NotificationEditDialog, SLOT(on_buttonBox_accepted()));

        QMetaObject::connectSlotsByName(NotificationEditDialog);
    } // setupUi

    void retranslateUi(QDialog *NotificationEditDialog)
    {
        NotificationEditDialog->setWindowTitle(QCoreApplication::translate("NotificationEditDialog", "\347\274\226\350\276\221\351\200\232\347\237\245", nullptr));
        idLabel->setText(QCoreApplication::translate("NotificationEditDialog", "ID\357\274\232", nullptr));
        titleLabel->setText(QCoreApplication::translate("NotificationEditDialog", "\346\240\207\351\242\230\357\274\232", nullptr));
        titleLineEdit->setPlaceholderText(QCoreApplication::translate("NotificationEditDialog", "\350\257\267\350\276\223\345\205\245\351\200\232\347\237\245\346\240\207\351\242\230", nullptr));
        categoryLabel->setText(QCoreApplication::translate("NotificationEditDialog", "\345\210\206\347\261\273\357\274\232", nullptr));
        categoryComboBox->setItemText(0, QCoreApplication::translate("NotificationEditDialog", "\350\241\214\346\224\277", nullptr));
        categoryComboBox->setItemText(1, QCoreApplication::translate("NotificationEditDialog", "\345\255\246\346\234\257", nullptr));
        categoryComboBox->setItemText(2, QCoreApplication::translate("NotificationEditDialog", "\347\224\237\346\264\273", nullptr));

        dateLabel->setText(QCoreApplication::translate("NotificationEditDialog", "\346\227\245\346\234\237\357\274\232", nullptr));
        dateLineEdit->setPlaceholderText(QCoreApplication::translate("NotificationEditDialog", "\346\240\274\345\274\217\357\274\232YYYY-MM-DD", nullptr));
        contentLabel->setText(QCoreApplication::translate("NotificationEditDialog", "\345\206\205\345\256\271\357\274\232", nullptr));
        contentTextEdit->setPlaceholderText(QCoreApplication::translate("NotificationEditDialog", "\350\257\267\350\276\223\345\205\245\351\200\232\347\237\245\345\206\205\345\256\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NotificationEditDialog: public Ui_NotificationEditDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NOTIFICATIONEDITDIALOG_H
