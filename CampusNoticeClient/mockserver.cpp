// MockServer.cpp
#include "MockServer.h"
#include <QDateTime>

MockServer::MockServer(QObject *parent) : QObject(parent) {}

QByteArray MockServer::getMockNotices()
{
    QJsonArray noticesArray;
    // 模拟3条测试数据
    QJsonObject notice1;
    notice1["id"] = 1;
    notice1["title"] = "2025秋季学期期中教学检查通知";
    notice1["content"] = "第10周开展期中教学检查，请各院系做好课程安排核对、教案检查等工作。";
    notice1["category"] = "通知";
    notice1["publish_time"] = QDateTime::currentDateTime().addDays(-2).toString(Qt::ISODate);
    notice1["is_read"] = false;
    noticesArray.append(notice1);

    QJsonObject notice2;
    notice2["id"] = 2;
    notice2["title"] = "校园秋季运动会报名启动";
    notice2["content"] = "运动会将于11月15日-16日举行，报名截止10月30日，可通过校园APP报名。";
    notice2["category"] = "活动";
    notice2["publish_time"] = QDateTime::currentDateTime().addDays(-1).toString(Qt::ISODate);
    notice2["is_read"] = false;
    noticesArray.append(notice2);

    QJsonObject notice3;
    notice3["id"] = 3;
    notice3["title"] = "期末考试安排公告";
    notice3["content"] = "2025秋季学期期末考试将于第18周（12月23日起）举行，具体科目时间见教务网。";
    notice3["category"] = "公告";
    notice3["publish_time"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    notice3["is_read"] = false;
    noticesArray.append(notice3);

    QJsonDocument doc(noticesArray);
    return doc.toJson();
}
