/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionSettings;
    QAction *actionAbout;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QWidget *categoryWidget;
    QVBoxLayout *verticalLayout_3;
    QLabel *categoryLabel;
    QListWidget *categoryListWidget;
    QPushButton *refreshButton;
    QPushButton *markAllReadButton;
    QSpacerItem *verticalSpacer;
    QWidget *noticeListWidget;
    QVBoxLayout *verticalLayout_2;
    QWidget *noticeListHeaderWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *noticeListLabel;
    QLabel *unreadCountLabel;
    QSpacerItem *horizontalSpacer;
    QListView *noticeListView;
    QWidget *noticeDetailWidget;
    QVBoxLayout *verticalLayout;
    QLabel *noticeDetailLabel;
    QWidget *noticeInfoWidget;
    QFormLayout *formLayout;
    QLabel *titleLabel;
    QLabel *noticeTitleLabel;
    QLabel *publishTimeLabel;
    QLabel *noticePublishTimeLabel;
    QLabel *categoryDetailLabel;
    QLabel *noticeCategoryLabel;
    QTextBrowser *noticeContentTextBrowser;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QStatusBar *statusbar;
    QLabel *statusLabel;
    QLabel *lastUpdateTimeLabel;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 700);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName("actionSettings");
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName("horizontalLayout");
        categoryWidget = new QWidget(centralwidget);
        categoryWidget->setObjectName("categoryWidget");
        verticalLayout_3 = new QVBoxLayout(categoryWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName("verticalLayout_3");
        categoryLabel = new QLabel(categoryWidget);
        categoryLabel->setObjectName("categoryLabel");
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        categoryLabel->setFont(font);

        verticalLayout_3->addWidget(categoryLabel);

        categoryListWidget = new QListWidget(categoryWidget);
        new QListWidgetItem(categoryListWidget);
        new QListWidgetItem(categoryListWidget);
        new QListWidgetItem(categoryListWidget);
        new QListWidgetItem(categoryListWidget);
        new QListWidgetItem(categoryListWidget);
        new QListWidgetItem(categoryListWidget);
        categoryListWidget->setObjectName("categoryListWidget");
        categoryListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

        verticalLayout_3->addWidget(categoryListWidget);

        refreshButton = new QPushButton(categoryWidget);
        refreshButton->setObjectName("refreshButton");

        verticalLayout_3->addWidget(refreshButton);

        markAllReadButton = new QPushButton(categoryWidget);
        markAllReadButton->setObjectName("markAllReadButton");

        verticalLayout_3->addWidget(markAllReadButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        horizontalLayout->addWidget(categoryWidget);

        noticeListWidget = new QWidget(centralwidget);
        noticeListWidget->setObjectName("noticeListWidget");
        verticalLayout_2 = new QVBoxLayout(noticeListWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName("verticalLayout_2");
        noticeListHeaderWidget = new QWidget(noticeListWidget);
        noticeListHeaderWidget->setObjectName("noticeListHeaderWidget");
        horizontalLayout_2 = new QHBoxLayout(noticeListHeaderWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        noticeListLabel = new QLabel(noticeListHeaderWidget);
        noticeListLabel->setObjectName("noticeListLabel");
        noticeListLabel->setFont(font);

        horizontalLayout_2->addWidget(noticeListLabel);

        unreadCountLabel = new QLabel(noticeListHeaderWidget);
        unreadCountLabel->setObjectName("unreadCountLabel");

        horizontalLayout_2->addWidget(unreadCountLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_2->addWidget(noticeListHeaderWidget);

        noticeListView = new QListView(noticeListWidget);
        noticeListView->setObjectName("noticeListView");
        noticeListView->setSelectionMode(QAbstractItemView::SingleSelection);
        noticeListView->setAlternatingRowColors(true);

        verticalLayout_2->addWidget(noticeListView);


        horizontalLayout->addWidget(noticeListWidget);

        noticeDetailWidget = new QWidget(centralwidget);
        noticeDetailWidget->setObjectName("noticeDetailWidget");
        verticalLayout = new QVBoxLayout(noticeDetailWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        noticeDetailLabel = new QLabel(noticeDetailWidget);
        noticeDetailLabel->setObjectName("noticeDetailLabel");
        noticeDetailLabel->setFont(font);

        verticalLayout->addWidget(noticeDetailLabel);

        noticeInfoWidget = new QWidget(noticeDetailWidget);
        noticeInfoWidget->setObjectName("noticeInfoWidget");
        formLayout = new QFormLayout(noticeInfoWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName("formLayout");
        titleLabel = new QLabel(noticeInfoWidget);
        titleLabel->setObjectName("titleLabel");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, titleLabel);

        noticeTitleLabel = new QLabel(noticeInfoWidget);
        noticeTitleLabel->setObjectName("noticeTitleLabel");
        QFont font1;
        font1.setBold(true);
        noticeTitleLabel->setFont(font1);
        noticeTitleLabel->setWordWrap(true);

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, noticeTitleLabel);

        publishTimeLabel = new QLabel(noticeInfoWidget);
        publishTimeLabel->setObjectName("publishTimeLabel");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, publishTimeLabel);

        noticePublishTimeLabel = new QLabel(noticeInfoWidget);
        noticePublishTimeLabel->setObjectName("noticePublishTimeLabel");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, noticePublishTimeLabel);

        categoryDetailLabel = new QLabel(noticeInfoWidget);
        categoryDetailLabel->setObjectName("categoryDetailLabel");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, categoryDetailLabel);

        noticeCategoryLabel = new QLabel(noticeInfoWidget);
        noticeCategoryLabel->setObjectName("noticeCategoryLabel");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, noticeCategoryLabel);


        verticalLayout->addWidget(noticeInfoWidget);

        noticeContentTextBrowser = new QTextBrowser(noticeDetailWidget);
        noticeContentTextBrowser->setObjectName("noticeContentTextBrowser");

        verticalLayout->addWidget(noticeContentTextBrowser);


        horizontalLayout->addWidget(noticeDetailWidget);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 2);
        horizontalLayout->setStretch(2, 3);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1000, 26));
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName("menu_2");
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName("menu_3");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        statusLabel = new QLabel(statusbar);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        lastUpdateTimeLabel = new QLabel(statusbar);
        lastUpdateTimeLabel->setObjectName("lastUpdateTimeLabel");
        lastUpdateTimeLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_3->menuAction());
        menu->addAction(actionExit);
        menu_2->addAction(actionSettings);
        menu_3->addAction(actionAbout);

        retranslateUi(MainWindow);

        categoryListWidget->setCurrentRow(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\240\241\345\233\255\351\200\232\347\237\245\345\256\242\346\210\267\347\253\257", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
        actionSettings->setText(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216", nullptr));
        categoryLabel->setText(QCoreApplication::translate("MainWindow", "\351\200\232\347\237\245\345\210\206\347\261\273", nullptr));

        const bool __sortingEnabled = categoryListWidget->isSortingEnabled();
        categoryListWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = categoryListWidget->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("MainWindow", "\345\205\250\351\203\250\351\200\232\347\237\245", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = categoryListWidget->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("MainWindow", "\345\205\254\345\221\212", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = categoryListWidget->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("MainWindow", "\346\264\273\345\212\250", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = categoryListWidget->item(3);
        ___qlistwidgetitem3->setText(QCoreApplication::translate("MainWindow", "\345\255\246\346\234\257", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = categoryListWidget->item(4);
        ___qlistwidgetitem4->setText(QCoreApplication::translate("MainWindow", "\345\260\261\344\270\232", nullptr));
        QListWidgetItem *___qlistwidgetitem5 = categoryListWidget->item(5);
        ___qlistwidgetitem5->setText(QCoreApplication::translate("MainWindow", "\345\205\266\344\273\226", nullptr));
        categoryListWidget->setSortingEnabled(__sortingEnabled);

        refreshButton->setText(QCoreApplication::translate("MainWindow", "\345\210\267\346\226\260", nullptr));
        markAllReadButton->setText(QCoreApplication::translate("MainWindow", "\345\205\250\351\203\250\345\267\262\350\257\273", nullptr));
        noticeListLabel->setText(QCoreApplication::translate("MainWindow", "\351\200\232\347\237\245\345\210\227\350\241\250", nullptr));
        unreadCountLabel->setText(QCoreApplication::translate("MainWindow", "(0 \346\235\241\346\234\252\350\257\273)", nullptr));
        noticeDetailLabel->setText(QCoreApplication::translate("MainWindow", "\351\200\232\347\237\245\350\257\246\346\203\205", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainWindow", "\346\240\207\351\242\230\357\274\232", nullptr));
        noticeTitleLabel->setText(QCoreApplication::translate("MainWindow", "\350\257\267\351\200\211\346\213\251\351\200\232\347\237\245\346\237\245\347\234\213\350\257\246\346\203\205", nullptr));
        publishTimeLabel->setText(QCoreApplication::translate("MainWindow", "\345\217\221\345\270\203\346\227\266\351\227\264\357\274\232", nullptr));
        noticePublishTimeLabel->setText(QString());
        categoryDetailLabel->setText(QCoreApplication::translate("MainWindow", "\345\210\206\347\261\273\357\274\232", nullptr));
        noticeCategoryLabel->setText(QString());
        noticeContentTextBrowser->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
" p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; color:#808080;\">\350\257\267\345\234\250\345\267\246\344\276\247\345\210\227\350\241\250\344\270\255\351\200\211\346\213\251\351\200\232\347\237\245\346\237\245\347\234\213\350\257\246\346\203\205</span></p></body></html>", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        menu_2->setTitle(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
        menu_3->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "\345\260\261\347\273\252", nullptr));
        lastUpdateTimeLabel->setText(QCoreApplication::translate("MainWindow", "\346\234\200\345\220\216\346\233\264\346\226\260\357\274\232\344\273\216\346\234\252", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
