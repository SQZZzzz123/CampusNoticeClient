// LogManager.h
#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>

class LogManager : public QObject
{
    Q_OBJECT
public:
    explicit LogManager(QObject *parent = nullptr);
    ~LogManager();
    static void writeLog(const QString &type, const QString &content);

private:
    static QFile *logFile;
    static QMutex logMutex;
};

#endif // LOGMANAGER_H
