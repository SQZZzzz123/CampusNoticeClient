/********************************************************************************
** Form generated from reading UI file 'notificationdetaildialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NOTIFICATIONDETAILDIALOG_H
#define UI_NOTIFICATIONDETAILDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NotificationDetailDialog
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QHBoxLayout *horizontalLayout;
    QLabel *categoryLabel;
    QLabel *categoryValueLabel;
    QSpacerItem *horizontalSpacer;
    QLabel *dateLabel;
    QLabel *dateValueLabel;
    QFrame *frame;
    QLabel *contentLabel;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *NotificationDetailDialog)
    {
        if (NotificationDetailDialog->objectName().isEmpty())
            NotificationDetailDialog->setObjectName("NotificationDetailDialog");
        NotificationDetailDialog->resize(500, 400);
        verticalLayoutWidget = new QWidget(NotificationDetailDialog);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(10, 10, 480, 380));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        titleLabel = new QLabel(verticalLayoutWidget);
        titleLabel->setObjectName("titleLabel");
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        titleLabel->setFont(font);
        titleLabel->setWordWrap(true);

        verticalLayout->addWidget(titleLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        categoryLabel = new QLabel(verticalLayoutWidget);
        categoryLabel->setObjectName("categoryLabel");

        horizontalLayout->addWidget(categoryLabel);

        categoryValueLabel = new QLabel(verticalLayoutWidget);
        categoryValueLabel->setObjectName("categoryValueLabel");

        horizontalLayout->addWidget(categoryValueLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        dateLabel = new QLabel(verticalLayoutWidget);
        dateLabel->setObjectName("dateLabel");

        horizontalLayout->addWidget(dateLabel);

        dateValueLabel = new QLabel(verticalLayoutWidget);
        dateValueLabel->setObjectName("dateValueLabel");

        horizontalLayout->addWidget(dateValueLabel);


        verticalLayout->addLayout(horizontalLayout);

        frame = new QFrame(verticalLayoutWidget);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::HLine);
        frame->setFrameShadow(QFrame::Sunken);
        frame->setLineWidth(1);

        verticalLayout->addWidget(frame);

        contentLabel = new QLabel(verticalLayoutWidget);
        contentLabel->setObjectName("contentLabel");
        contentLabel->setWordWrap(true);
        contentLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

        verticalLayout->addWidget(contentLabel);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(verticalLayoutWidget);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(NotificationDetailDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, NotificationDetailDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, NotificationDetailDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(NotificationDetailDialog);
    } // setupUi

    void retranslateUi(QDialog *NotificationDetailDialog)
    {
        NotificationDetailDialog->setWindowTitle(QCoreApplication::translate("NotificationDetailDialog", "\351\200\232\347\237\245\350\257\246\346\203\205", nullptr));
        titleLabel->setText(QCoreApplication::translate("NotificationDetailDialog", "\351\200\232\347\237\245\346\240\207\351\242\230", nullptr));
        categoryLabel->setText(QCoreApplication::translate("NotificationDetailDialog", "\345\210\206\347\261\273\357\274\232", nullptr));
        categoryValueLabel->setText(QCoreApplication::translate("NotificationDetailDialog", "\345\210\206\347\261\273\345\200\274", nullptr));
        dateLabel->setText(QCoreApplication::translate("NotificationDetailDialog", "\346\227\245\346\234\237\357\274\232", nullptr));
        dateValueLabel->setText(QCoreApplication::translate("NotificationDetailDialog", "\346\227\245\346\234\237\345\200\274", nullptr));
        contentLabel->setText(QCoreApplication::translate("NotificationDetailDialog", "\351\200\232\347\237\245\345\206\205\345\256\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NotificationDetailDialog: public Ui_NotificationDetailDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NOTIFICATIONDETAILDIALOG_H
