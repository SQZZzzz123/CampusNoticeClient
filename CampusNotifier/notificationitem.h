#ifndef NOTIFICATIONITEM_H
#define NOTIFICATIONITEM_H

#include <QString>

struct Notification {
    int id;
    QString title;
    QString category;
    QString date;
    QString content;
};

#endif // NOTIFICATIONITEM_H
