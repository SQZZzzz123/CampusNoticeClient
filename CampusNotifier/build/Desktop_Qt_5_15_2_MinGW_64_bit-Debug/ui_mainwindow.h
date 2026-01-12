/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *categoryCombo;
    QSpacerItem *horizontalSpacer;
    QPushButton *refreshBtn;
    QHBoxLayout *buttonLayout;
    QPushButton *addBtn;
    QPushButton *editBtn;
    QPushButton *deleteBtn;
    QSpacerItem *buttonSpacer;
    QListWidget *listWidget;
    QLabel *statusLabel;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(480, 640);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(10, 10, 10, 10);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        categoryCombo = new QComboBox(centralwidget);
        categoryCombo->addItem(QString());
        categoryCombo->addItem(QString());
        categoryCombo->addItem(QString());
        categoryCombo->addItem(QString());
        categoryCombo->setObjectName(QString::fromUtf8("categoryCombo"));
        categoryCombo->setMinimumSize(QSize(120, 0));

        horizontalLayout->addWidget(categoryCombo);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        refreshBtn = new QPushButton(centralwidget);
        refreshBtn->setObjectName(QString::fromUtf8("refreshBtn"));
        refreshBtn->setStyleSheet(QString::fromUtf8("QPushButton { padding: 5px 15px; font-weight: bold; }"));

        horizontalLayout->addWidget(refreshBtn);


        verticalLayout->addLayout(horizontalLayout);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        addBtn = new QPushButton(centralwidget);
        addBtn->setObjectName(QString::fromUtf8("addBtn"));
        addBtn->setStyleSheet(QString::fromUtf8("QPushButton { padding: 5px 15px; }"));

        buttonLayout->addWidget(addBtn);

        editBtn = new QPushButton(centralwidget);
        editBtn->setObjectName(QString::fromUtf8("editBtn"));
        editBtn->setStyleSheet(QString::fromUtf8("QPushButton { padding: 5px 15px; }"));

        buttonLayout->addWidget(editBtn);

        deleteBtn = new QPushButton(centralwidget);
        deleteBtn->setObjectName(QString::fromUtf8("deleteBtn"));
        deleteBtn->setStyleSheet(QString::fromUtf8("QPushButton { padding: 5px 15px; }"));

        buttonLayout->addWidget(deleteBtn);

        buttonSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(buttonSpacer);


        verticalLayout->addLayout(buttonLayout);

        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        QFont font;
        font.setPointSize(10);
        listWidget->setFont(font);
        listWidget->setAlternatingRowColors(true);
        listWidget->setSpacing(2);

        verticalLayout->addWidget(listWidget);

        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setStyleSheet(QString::fromUtf8("color: gray; font-size: 11px;"));

        verticalLayout->addWidget(statusLabel);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\240\241\345\233\255\351\200\232\347\237\245\346\216\250\351\200\201\345\256\242\346\210\267\347\253\257", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\345\210\206\347\261\273\347\255\233\351\200\211\357\274\232", nullptr));
        categoryCombo->setItemText(0, QCoreApplication::translate("MainWindow", "\345\205\250\351\203\250", nullptr));
        categoryCombo->setItemText(1, QCoreApplication::translate("MainWindow", "\350\241\214\346\224\277", nullptr));
        categoryCombo->setItemText(2, QCoreApplication::translate("MainWindow", "\345\255\246\346\234\257", nullptr));
        categoryCombo->setItemText(3, QCoreApplication::translate("MainWindow", "\347\224\237\346\264\273", nullptr));

        refreshBtn->setText(QCoreApplication::translate("MainWindow", "\345\220\214\346\255\245\346\234\200\346\226\260\351\200\232\347\237\245", nullptr));
        addBtn->setText(QCoreApplication::translate("MainWindow", "\345\242\236\345\212\240\351\200\232\347\237\245", nullptr));
        editBtn->setText(QCoreApplication::translate("MainWindow", "\344\277\256\346\224\271\351\200\232\347\237\245", nullptr));
        deleteBtn->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244\351\200\232\347\237\245", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "\345\260\261\347\273\252", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
