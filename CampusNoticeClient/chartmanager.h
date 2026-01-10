#ifndef CHARTMANAGER_H
#define CHARTMANAGER_H

#include <QObject>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMap>
#include <QPair>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QTextBrowser>

class ChartManager : public QObject
{
    Q_OBJECT

public:
    explicit ChartManager(QObject *parent = nullptr);
    ~ChartManager();

    // 显示统计图表对话框（使用文本形式显示统计信息）
    void showChartDialog();

signals:
    void exportProgress(int progress, const QString &message);
    void exportFinished(bool success, const QString &message);

public slots:
    void exportChartData(const QString &filePath);

private slots:
    void onExportProgress(int progress, const QString &message);
    void onExportFinished(bool success, const QString &message);

private:
    // 创建文本形式的统计信息显示
    QWidget* createCategoryStatsWidget();
    QWidget* createReadStatusStatsWidget();

    // 从数据库获取统计数据
    QMap<QString, int> getCategoryStats();
    QPair<int, int> getReadStatusStats();

    QDialog *chartDialog;
    QTabWidget *tabWidget;
    QWidget *pieChartWidget;  // 使用普通QWidget显示统计信息
    QWidget *barChartWidget;  // 使用普通QWidget显示统计信息
    QPushButton *exportBtn;
    QPushButton *closeBtn;
    QLabel *statusLabel;
    QThread *exportThread;
};

#endif // CHARTMANAGER_H
