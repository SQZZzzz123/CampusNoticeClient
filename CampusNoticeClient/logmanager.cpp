// LogManager.cpp
#include "LogManager.h"
#include <QStandardPaths>
#include <QDir>

QMutex LogManager::logMutex;
QFile *LogManager::logFile = nullptr;

LogManager::LogManager(QObject *parent) : QObject(parent)
{
    if (!logFile) {
        QString logPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir dir(logPath);
        if (!dir.exists()) dir.mkpath(".");
        QString logFileName = logPath + "/notice_client_log_" + QDateTime::currentDateTime().toString("yyyyMMdd") + ".txt";
        logFile = new QFile(logFileName);
        if (!logFile->open(QIODevice::Append | QIODevice::Text)) {
            delete logFile;
            logFile = nullptr;
        }
    }
}

LogManager::~LogManager()
{
    if (logFile) {
        logFile->close();
        delete logFile;
        logFile = nullptr;
    }
}

void LogManager::writeLog(const QString &type, const QString &content)
{
    if (!logFile) return;
    QMutexLocker locker(&logMutex);
    QTextStream out(logFile);
    QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    out << "[" << timeStr << "] [" << type << "] " << content << "\n";
    out.flush();
}
