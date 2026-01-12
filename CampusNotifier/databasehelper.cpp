#include "databasehelper.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

DatabaseHelper::DatabaseHelper() {
    // 构造函数
}

DatabaseHelper::~DatabaseHelper() {
    // 析构函数，关闭数据库连接
    if (database.isOpen()) {
        database.close();
    }
}

bool DatabaseHelper::initDatabase()
{
    // 添加 SQLite 驱动
    database = QSqlDatabase::addDatabase("QSQLITE");
    
    // 设置数据库文件地址为指定路径
    QString aFile = "B:/QtCode/CampusNotifier/build/CampsDB.db";
    database.setDatabaseName(aFile);
    qDebug() << "Database path:" << aFile;

    if (!database.open())
    {
        qDebug() << "failed to open database:" << database.lastError().text();
        return false;
    }
    else
    {
        qDebug() << "open database is ok" << database.connectionName() << "at" << aFile;
        
        // 自动创建表结构（防止数据库文件是空的）
        QSqlQuery query;
        QString createTableSql = "CREATE TABLE IF NOT EXISTS notifications ("
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                 "title TEXT, "
                                 "category TEXT, "
                                 "date TEXT, "
                                 "content TEXT)";
        if (!query.exec(createTableSql)) {
            qDebug() << "创建表失败:" << query.lastError().text();
        }
        
        // 检查数据库是否为空，添加初始数据
        if (!query.exec("SELECT COUNT(*) FROM notifications")) {
            qDebug() << "查询数据失败:" << query.lastError().text();
        } else if (query.next() && query.value(0).toInt() == 0) {
            qDebug() << "Database is empty, adding initial data...";
            
            // 添加初始通知数据
            QVector<Notification> initialData;
            initialData.append({101, "关于放假的通知", "行政", "2025-10-01", "国庆放假7天"});
            initialData.append({102, "讲座：AI未来", "学术", "2025-10-05", "地点：报告厅"});
            initialData.append({103, "食堂新菜品推出", "生活", "2025-10-03", "食堂三楼新增特色菜品"});
            initialData.append({104, "艺术展通知", "艺术", "2025-10-10", "图书馆将举办秋季艺术展"});
            initialData.append({105, "运动会通知", "体育", "2025-10-15", "秋季运动会即将举行"});
            initialData.append({106, "奖学金申请", "学术", "2025-10-20", "年度奖学金开始申请"});
            initialData.append({107, "宿舍检查", "生活", "2025-10-25", "月底进行宿舍安全检查"});
            initialData.append({108, "讲座：计算机科学", "学术", "2025-10-30", "计算机科学前沿讲座"});
            initialData.append({109, "校园招聘会", "行政", "2025-11-05", "秋季校园招聘会"});
            initialData.append({110, "新生欢迎会", "行政", "2025-11-10", "新生欢迎会安排"});
            
            // 开启事务插入初始数据
            if (!database.transaction()) {
                qDebug() << "开启事务失败:" << database.lastError().text();
            } else {
                if (!query.prepare("INSERT INTO notifications (id, title, category, date, content) VALUES (?, ?, ?, ?, ?)")) {
                    qDebug() << "准备插入语句失败:" << query.lastError().text();
                    database.rollback();
                } else {
                    for (const auto& item : initialData) {
                        query.bindValue(0, item.id);
                        query.bindValue(1, item.title);
                        query.bindValue(2, item.category);
                        query.bindValue(3, item.date);
                        query.bindValue(4, item.content);
                        
                        if (!query.exec()) {
                            qDebug() << "插入初始数据失败:" << query.lastError().text();
                            database.rollback();
                            break;
                        }
                    }
                    
                    if (database.commit()) {
                        qDebug() << "Initial data added successfully";
                    } else {
                        qDebug() << "提交初始数据失败:" << database.lastError().text();
                    }
                }
            }
        }
        
        return true;
    }
}

void DatabaseHelper::saveNotifications(const QVector<Notification>& list)
{
    if (!database.isOpen()) {
        qDebug() << "database is not open";
        return;
    }

    QSqlQuery query;
    // 开启事务，提高写入性能
    if (!database.transaction()) {
        qDebug() << "开启事务失败:" << database.lastError().text();
        return;
    }
    
    // 批量插入或更新数据，保留现有通知
    if (!query.prepare("INSERT OR REPLACE INTO notifications (id, title, category, date, content) VALUES (:id, :title, :category, :date, :content)")) {
        qDebug() << "准备插入语句失败:" << query.lastError().text();
        database.rollback();
        return;
    }
    
    for (const auto& item : list) {
        query.bindValue(":id", item.id);
        query.bindValue(":title", item.title);
        query.bindValue(":category", item.category);
        query.bindValue(":date", item.date);
        query.bindValue(":content", item.content);
        
        if (!query.exec()) {
            qDebug() << "插入数据失败:" << query.lastError().text();
            database.rollback();
            return;
        }
    }
    
    // 提交事务
    if (!database.commit()) {
        qDebug() << "提交事务失败:" << database.lastError().text();
        database.rollback();
    }
}

QVector<Notification> DatabaseHelper::getAllNotifications()
{
    QVector<Notification> result;
    
    if (!database.isOpen()) {
        qDebug() << "database is not open";
        return result;
    }
    
    QSqlQuery query;
    if (!query.exec("SELECT id, title, category, date, content FROM notifications")) {
        qDebug() << "查询数据失败:" << query.lastError().text();
        return result;
    }
    
    while (query.next()) {
        Notification item;
        item.id = query.value(0).toInt();
        item.title = query.value(1).toString();
        item.category = query.value(2).toString();
        item.date = query.value(3).toString();
        item.content = query.value(4).toString();
        result.append(item);
    }
    
    return result;
}

bool DatabaseHelper::addNotification(const Notification& notification)
{
    if (!database.isOpen()) {
        qDebug() << "database is not open";
        return false;
    }
    
    QSqlQuery query;
    QString sql = "INSERT INTO notifications (id, title, category, date, content) VALUES (:id, :title, :category, :date, :content)";
    
    if (!query.prepare(sql)) {
        qDebug() << "准备插入语句失败:" << query.lastError().text();
        return false;
    }
    
    query.bindValue(":id", notification.id);
    query.bindValue(":title", notification.title);
    query.bindValue(":category", notification.category);
    query.bindValue(":date", notification.date);
    query.bindValue(":content", notification.content);
    
    if (!query.exec()) {
        qDebug() << "插入数据失败:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool DatabaseHelper::updateNotification(const Notification& notification)
{
    if (!database.isOpen()) {
        qDebug() << "database is not open";
        return false;
    }
    
    QSqlQuery query;
    QString sql = "UPDATE notifications SET title = :title, category = :category, date = :date, content = :content WHERE id = :id";
    
    if (!query.prepare(sql)) {
        qDebug() << "准备更新语句失败:" << query.lastError().text();
        return false;
    }
    
    query.bindValue(":title", notification.title);
    query.bindValue(":category", notification.category);
    query.bindValue(":date", notification.date);
    query.bindValue(":content", notification.content);
    query.bindValue(":id", notification.id);
    
    if (!query.exec()) {
        qDebug() << "更新数据失败:" << query.lastError().text();
        return false;
    }
    
    return query.numRowsAffected() > 0;
}

bool DatabaseHelper::deleteNotification(int id)
{
    if (!database.isOpen()) {
        qDebug() << "database is not open";
        return false;
    }
    
    QSqlQuery query;
    QString sql = "DELETE FROM notifications WHERE id = :id";
    
    if (!query.prepare(sql)) {
        qDebug() << "准备删除语句失败:" << query.lastError().text();
        return false;
    }
    
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        qDebug() << "删除数据失败:" << query.lastError().text();
        return false;
    }
    
    return query.numRowsAffected() > 0;
}
