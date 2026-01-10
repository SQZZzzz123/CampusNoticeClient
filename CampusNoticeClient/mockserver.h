// MockServer.h
#ifndef MOCKSERVER_H
#define MOCKSERVER_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>

class MockServer : public QObject
{
    Q_OBJECT
public:
    explicit MockServer(QObject *parent = nullptr);
    QByteArray getMockNotices();
};

#endif // MOCKSERVER_H
