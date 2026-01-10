#include "ChartManager.h"
#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QStandardPaths>
#include <QDateTime>
#include <QMessageBox>
#include <QTextStream>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QProgressDialog>
#include <QTimer>
#include <QCoreApplication>
#include <QFileInfo>
#include <QTextCodec>
#include <QFileDialog>
#include <QTabWidget>
#include <QTextBrowser>
#include <QtConcurrent/QtConcurrent>

ChartManager::ChartManager(QObject *parent)
    : QObject(parent)
    , chartDialog(nullptr)
    , tabWidget(nullptr)
    , pieChartWidget(nullptr)
    , barChartWidget(nullptr)
    , exportBtn(nullptr)
    , closeBtn(nullptr)
    , statusLabel(nullptr)
    , exportThread(nullptr)
{
}

ChartManager::~ChartManager()
{
    if (chartDialog) {
        chartDialog->deleteLater();
    }
    if (exportThread) {
        exportThread->quit();
        exportThread->wait();
        delete exportThread;
    }
}

void ChartManager::showChartDialog()
{
    if (!chartDialog) {
        chartDialog = new QDialog(qobject_cast<QWidget*>(parent()));
        chartDialog->setWindowTitle("通知统计图表");
        chartDialog->resize(800, 600);

        QVBoxLayout *mainLayout = new QVBoxLayout(chartDialog);

        // 创建标签页
        tabWidget = new QTabWidget(chartDialog);

        // 饼图标签页 - 分类统计
        pieChartWidget = createCategoryStatsWidget();
        tabWidget->addTab(pieChartWidget, "分类统计");

        // 柱状图标签页 - 已读/未读统计
        barChartWidget = createReadStatusStatsWidget();
        tabWidget->addTab(barChartWidget, "已读/未读统计");

        mainLayout->addWidget(tabWidget);

        // 按钮布局
        QHBoxLayout *buttonLayout = new QHBoxLayout();

        exportBtn = new QPushButton("导出统计数据", chartDialog);
        closeBtn = new QPushButton("关闭", chartDialog);

        buttonLayout->addStretch();
        buttonLayout->addWidget(exportBtn);
        buttonLayout->addWidget(closeBtn);

        mainLayout->addLayout(buttonLayout);

        // 状态标签
        statusLabel = new QLabel(chartDialog);
        mainLayout->addWidget(statusLabel);

        // 连接信号槽
        connect(exportBtn, &QPushButton::clicked, [this]() {
            QString fileName = QFileDialog::getSaveFileName(
                chartDialog,
                "导出统计数据",
                QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
                    "/校园通知_统计_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".csv",
                "CSV Files (*.csv)"
                );

            if (!fileName.isEmpty()) {
                // 在后台线程导出数据
                if (exportThread && exportThread->isRunning()) {
                    statusLabel->setText("正在导出，请稍候...");
                    return;
                }

                // 创建一个新的线程来执行导出操作
                exportThread = QThread::create([this, fileName]() {
                    exportChartData(fileName);
                });

                connect(exportThread, &QThread::finished, exportThread, &QThread::deleteLater);
                connect(this, &ChartManager::exportProgress,
                        this, &ChartManager::onExportProgress);
                connect(this, &ChartManager::exportFinished,
                        this, &ChartManager::onExportFinished);

                exportThread->start();
            }
        });

        connect(closeBtn, &QPushButton::clicked, chartDialog, &QDialog::accept);

        // 连接导出相关信号槽
        connect(this, &ChartManager::exportProgress,
                this, &ChartManager::onExportProgress);
        connect(this, &ChartManager::exportFinished,
                this, &ChartManager::onExportFinished);
    }

    // 更新统计信息显示
    if (pieChartWidget) {
        delete pieChartWidget;
        pieChartWidget = createCategoryStatsWidget();
        tabWidget->insertTab(0, pieChartWidget, "分类统计");
        tabWidget->setCurrentIndex(0);
    }

    if (barChartWidget) {
        delete barChartWidget;
        barChartWidget = createReadStatusStatsWidget();
        tabWidget->insertTab(1, barChartWidget, "已读/未读统计");
        tabWidget->setCurrentIndex(1);
    }

    chartDialog->show();
    chartDialog->raise();
    chartDialog->activateWindow();
}

QWidget* ChartManager::createCategoryStatsWidget()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    QTextBrowser *textBrowser = new QTextBrowser();
    textBrowser->setStyleSheet("font-size: 14px;");

    QString html = "<h2>通知分类统计</h2>";
    html += "<table border='1' style='border-collapse: collapse; width: 100%;'>";
    html += "<tr style='background-color: #f0f0f0;'><th>分类</th><th>数量</th><th>占比</th></tr>";

    QMap<QString, int> stats = getCategoryStats();
    int total = 0;
    for (auto it = stats.constBegin(); it != stats.constEnd(); ++it) {
        total += it.value();
    }

    for (auto it = stats.constBegin(); it != stats.constEnd(); ++it) {
        double percentage = total > 0 ? (it.value() * 100.0 / total) : 0;
        html += QString("<tr><td>%1</td><td>%2</td><td>%.2f%%</td></tr>")
                    .arg(it.key()).arg(it.value()).arg(percentage);
    }

    html += "</table>";
    textBrowser->setHtml(html);

    layout->addWidget(textBrowser);
    return widget;
}

QWidget* ChartManager::createReadStatusStatsWidget()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    QTextBrowser *textBrowser = new QTextBrowser();
    textBrowser->setStyleSheet("font-size: 14px;");

    QPair<int, int> stats = getReadStatusStats();
    int total = stats.first + stats.second;
    double readPercentage = total > 0 ? (stats.first * 100.0 / total) : 0;
    double unreadPercentage = total > 0 ? (stats.second * 100.0 / total) : 0;

    QString html = "<h2>已读/未读通知统计</h2>";
    html += "<table border='1' style='border-collapse: collapse; width: 100%;'>";
    html += "<tr style='background-color: #f0f0f0;'><th>状态</th><th>数量</th><th>占比</th></tr>";
    html += QString("<tr><td>已读</td><td>%1</td><td>%.2f%%</td></tr>").arg(stats.first).arg(readPercentage);
    html += QString("<tr><td>未读</td><td>%1</td><td>%.2f%%</td></tr>").arg(stats.second).arg(unreadPercentage);
    html += QString("<tr><td><b>总计</b></td><td><b>%1</b></td><td><b>100.00%%</b></td></tr>").arg(total);
    html += "</table>";

    textBrowser->setHtml(html);

    layout->addWidget(textBrowser);
    return widget;
}

QMap<QString, int> ChartManager::getCategoryStats()
{
    QMap<QString, int> stats;

    QSqlDatabase db = QSqlDatabase::database("notice_db");
    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
        return stats;
    }

    QSqlQuery query(db);
    QString queryString = "SELECT category, COUNT(*) FROM notices GROUP BY category";

    if (!query.exec(queryString)) {
        qWarning() << "Failed to execute category stats query:" << query.lastError().text();
        return stats;
    }

    while (query.next()) {
        QString category = query.value(0).toString();
        int count = query.value(1).toInt();
        stats[category] = count;
    }

    return stats;
}

QPair<int, int> ChartManager::getReadStatusStats()
{
    QPair<int, int> stats;
    stats.first = 0; // 已读数量
    stats.second = 0; // 未读数量

    QSqlDatabase db = QSqlDatabase::database("notice_db");
    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
        return stats;
    }

    QSqlQuery query(db);
    QString queryString = "SELECT is_read, COUNT(*) FROM notices GROUP BY is_read";

    if (!query.exec(queryString)) {
        qWarning() << "Failed to execute read status stats query:" << query.lastError().text();
        return stats;
    }

    while (query.next()) {
        bool isRead = query.value(0).toBool();
        int count = query.value(1).toInt();

        if (isRead) {
            stats.first = count; // 已读
        } else {
            stats.second = count; // 未读
        }
    }

    return stats;
}

void ChartManager::exportChartData(const QString &filePath)
{
    // 在后台线程执行导出操作
    QtConcurrent::run([this, filePath]() {
        QMutexLocker locker(&mutex);

        emit exportProgress(0, "开始导出统计数据...");

        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            emit exportFinished(false, QString("无法打开文件: %1").arg(filePath));
            return;
        }

        QTextStream stream(&file);
        // 设置UTF-8编码
        stream.setCodec("UTF-8");

        // 写入分类统计
        stream << "分类统计\n";
        stream << "类别,数量,占比\n";

        QMap<QString, int> categoryStats = getCategoryStats();
        int totalCategory = 0;
        for (auto it = categoryStats.constBegin(); it != categoryStats.constEnd(); ++it) {
            totalCategory += it.value();
        }

        for (auto it = categoryStats.constBegin(); it != categoryStats.constEnd(); ++it) {
            double percentage = totalCategory > 0 ? (it.value() * 100.0 / totalCategory) : 0;
            stream << "\"" << it.key() << "\"," << it.value() << "," << QString::number(percentage, 'f', 2) << "%\n";
            emit exportProgress(30, QString("正在导出分类统计... (%1/%2)")
                                        .arg(std::distance(categoryStats.constBegin(), it)+1)
                                        .arg(categoryStats.size()));
            QCoreApplication::processEvents();
        }

        stream << "\n";

        // 写入已读/未读统计
        stream << "已读/未读统计\n";
        stream << "状态,数量,占比\n";
        QPair<int, int> readStats = getReadStatusStats();
        int totalRead = readStats.first + readStats.second;
        double readPercentage = totalRead > 0 ? (readStats.first * 100.0 / totalRead) : 0;
        double unreadPercentage = totalRead > 0 ? (readStats.second * 100.0 / totalRead) : 0;

        stream << "已读," << readStats.first << "," << QString::number(readPercentage, 'f', 2) << "%\n";
        stream << "未读," << readStats.second << "," << QString::number(unreadPercentage, 'f', 2) << "%\n";
        stream << "总计," << totalRead << ",100.00%\n";

        file.close();

        emit exportProgress(100, "导出完成!");
        emit exportFinished(true, QString("数据已导出到: %1").arg(filePath));
    });
}

void ChartManager::onExportProgress(int progress, const QString &message)
{
    if (statusLabel) {
        statusLabel->setText(message + QString(" (%1%)").arg(progress));
    }
}

void ChartManager::onExportFinished(bool success, const QString &message)
{
    if (statusLabel) {
        statusLabel->setText(message);

        QString title = success ? "导出成功" : "导出失败";
        QMessageBox::StandardButton btn = success ? QMessageBox::Information : QMessageBox::Warning;
        QMessageBox::information(chartDialog, title, message);
    }
}
